#include <iostream>

#include "binutils.h"
////////////////////////////////////////////////////////////////////////////////
//
//SIMD Cell Update
//
//******************************************************************************
//
//All cell values are stored in bit vectors (one per field)
//All of the inputs/output from a direction are loaded from either a seperate row of the array or with a left or right shift (+ some stuff to handle the edges)
//Cells are updated in blocks (size based on maximum word width of processor).
//Some processing power is wasted updating cells that are not yet ready but only a fraction of the cells need to be ready for a net win.
//If desired the update method could attempt to prioritize blocks based on the number of cells needing updates.
//Cell updates are effectivly idempotent in that cells will not change state during an update unless they are ready to fire.
//Stem cells that are being reconfigured are handled out of ban
//Wire stem cells are treated as wires 
//
////////////////////////////////////////////////////////////////////////////////

#define DEBUG(var)  std::cout << std::setw(10) << #var << "[" << std::setw(2) << i << "]:" << bin<big_word>(var) << std::endl;
template <typename big_word>
class cell_patch
{
    private:
        const static unsigned int BPW = sizeof(big_word)*8;
       
        ////////////////////////////////////////////////////////////////////////////////
        //
        //Cell Configuration
        //******************************************************************************
        //
        //The following fields to not normally change except when stem cells are being used to reconfigure things
        ////////////////////////////////////////////////////////////////////////////////
        big_word ef1[BPW]; //gate can fire with one input (wire/not. Doesn't include crossover)
        big_word ef2[BPW]; //gate can fire with two inputs (and, nand, or, xor, copy, delete)
        big_word cross[BPW]; //cell is a crossover

        big_word tf_pcl[BPW]; //cell is in {or, xor, delete, wire, not}
        big_word tf_pch[BPW]; //cell is in {or, and, nand}
        big_word inv_t[BPW]; //cell is in {not, nand}

        big_word ctrl_e[BPW]; //cell is copy or delete with control from east 
        big_word ctrl_n[BPW]; //cell is copy or delete with control from north
        big_word ctrl_w[BPW]; //cell is copy or delete with control from west
        big_word ctrl_s[BPW]; //cell is copy or delete with control from south 

        big_word copy_e[BPW]; //cell is a copy with data from east 
        big_word copy_n[BPW]; //cell is a copy with data from north 
        big_word copy_w[BPW]; //cell is a copy with data from west 
        big_word copy_s[BPW]; //cell is a copy with data from south 
        big_word is_cpy[BPW]; // = copy_e | copy_n | copy_w | copy_s//cell is a copy cell

        big_word is_del[BPW]; //cell is a delete cell

        //TODO check changing these to mask inputs instead of outputs 
        big_word out_e[BPW]; //cell output to the east
        big_word out_n[BPW]; //cell output to the north
        big_word out_w[BPW]; //cell output to the west
        big_word out_s[BPW]; //cell output to the south

        ////////////////////////////////////////////////////////////////////////////////
        //
        //Cell State
        //******************************************************************************
        //
        //The following fields reflect the current state of inputs and outputs of cells
        ////////////////////////////////////////////////////////////////////////////////

        //which inputs have tokens
        big_word ve[BPW]; //valid flag for inputs from the east
        big_word vn[BPW]; //valid flag for inputs from the north
        big_word vw[BPW]; //valid flag for inputs from the west
        big_word vs[BPW]; //valid flag for inputs from the south

        //which inputs have true tokens
        //IF AN INPUT DOES NOT HAVE A TOKEN THIS MUST BE 0!
        big_word te[BPW]; //value flag inputs from the east
        big_word tn[BPW]; //value flag for inputs from the north
        big_word tw[BPW]; //value flag for inputs from the west
        big_word ts[BPW]; //value flag for inputs from the south

        //which outputs are full
        //out_e_empty = ~e.vw //output to the east is empty and ready for a new token
        //out_n_empty = ~n.vs //output to the north is empty and ready for a new token
        //out_w_empty = ~w.ve //output to the west is empty and ready for a new token
        //out_s_empty = ~s.vn //output to the south is empty and ready for a new token

        
    public:
        //this function counts the number of bits set in each position of 4 different bit vectors
        //the result is returned as a two bit vector with the low and high bits of the count
        //this function assumes that at most 2 bits will be set in each quartet
        //if overflow is not an issue this could probably be slightly cleaner
        //static void popcount(big_word& pch, big_word& pcl, const big_word& a, const big_word& b, const big_word& c, const big_word& d)
        //{
        //    pcl = a^b^c^d;
        //    pch = (a|b|c|d)^pcl;
        //}

        ////////////////////////////////////////////////////////////////////////////////
        //
        //Compute update values
        //******************************************************************************
        // These functions compute which cells will fire and what they will output
        //
        // token_in
        ////////////////////////////////////////////////////////////////////////////////
        void update_word(
                  unsigned int i, //index of configuration/state word in patch
                  big_word& ove, //old output (1 full, 0 empty) for e/n/w/s and destination for new result
                  big_word& ovn,
                  big_word& ovw,
                  big_word& ovs,
                  big_word& eve, //emit valid for e/n/w/s
                  big_word& evn,
                  big_word& evw,
                  big_word& evs,
                  big_word& ete, //emit token value for e/n/w/s
                  big_word& etn,
                  big_word& etw,
                  big_word& ets)
        {

            //count the number of inputs that have a token
            //ive/n/s/w will only be set for at most the number of inputs of the cell (which is <= 2)
            big_word vpcl =  ve[i]^vn[i]^vw[i]^vs[i]; //sum mod 2
            big_word vpch = (ve[i]|vn[i]|vw[i]|vs[i])^vpcl; //(sum >=1) xor (sum mod 2) == (sum in {2, 4})
            big_word vew = ve[i] | vw[i]; //is there an east or west input? (for crossover)
            big_word vns = vn[i] | vs[i]; //is there a north or south input? (for crossover)

            //same thing for the number of true tokens
            big_word tpcl =  te[i]^tn[i]^tw[i]^ts[i]; //sum mod 2
            big_word tpch = (te[i]|tn[i]|tw[i]|ts[i])^tpcl; //(sum >=1) xor (sum mod 2) == (sum in {2, 4})
            big_word tew = te[i]|tw[i]; //is there an east or west true? (for crossover)
            big_word tns = tn[i]|ts[i]; //is there a north or south true? (for crossover)

            //compute which outputs are empty
            big_word aof = ove | ovn | ovw | ovs; //any output are full
            big_word ew_of = ove | ovw; //east/west outputs full (for crossover)
            big_word ns_of = ovn | ovs; //north/south outputs full (for crossover)

            //logic value computed
            //AND checks high bit of population count, XOR checks low bit, OR is both
            //inv handles NOT/NAND (and NOR/XNOR if we wanted)
            big_word logic = ((tpcl & tf_pcl[i]) | (tpch & tf_pch[i])) ^ inv_t[i];

            //control value for copy/delete
            big_word control = (te[i] & ctrl_e[i]) | (tn[i] & ctrl_n[i]) | (tw[i] & ctrl_w[i]) | (ts[i] & ctrl_s[i]);

            //data value for copy/delete
            //there are two valid tokens so xor pcl with control to recover data
            big_word data = tpcl ^ control;

            //check if we will not emit a token because of delete
            big_word deleted = is_del[i] & control; 

            //is the cell trying to fire?
            //cell can fire if outputs are empty and there are enough tokens to fire
            //can also fire if token would be deleted
            big_word fire = (~aof | deleted) & ((ef1[i] & vpcl) | (ef2[i] & vpch));

            //cell will also fire if it is a crossover, but will fire different directions seperately
            big_word fire_ew = fire | (cross[i] & vew & ~ew_of);
            big_word fire_ns = fire | (cross[i] & vns & ~ns_of);

            //is the cell emitting a token?
            //need to make sure that it wasn't deleted
            big_word emit_v = fire & ~deleted;
            
            //crossovers are special
            big_word emit_vew = emit_v | (cross[i] & fire_ew);
            big_word emit_vns = emit_v | (cross[i] & fire_ns);

            //token to emit
            //use either the logic or copy token
            //false tokens if no value should be emitted
            big_word emit_t = (logic | (data & is_cpy[i])) & emit_v;
            
            //crossovers are still special
            big_word emit_tew = emit_t | (cross[i] & emit_vew & tew);
            big_word emit_tns = emit_t | (cross[i] & emit_vns & tns);

            //figure out where we are consuming tokens from
            big_word consume_e = fire_ew & ~(control & copy_e[i]);
            big_word consume_n = fire_ns & ~(control & copy_n[i]);
            big_word consume_w = fire_ew & ~(control & copy_w[i]);
            big_word consume_s = fire_ns & ~(control & copy_s[i]);

            //consume inputs
            ve[i] &= ~consume_e;
            te[i] &= ~consume_e;
            
            vn[i] &= ~consume_n;
            tn[i] &= ~consume_n;

            vw[i] &= ~consume_w;
            tw[i] &= ~consume_w;

            vs[i] &= ~consume_s;
            ts[i] &= ~consume_s;

            //propagate tokens to outputs
            eve = emit_vew & out_e[i];
            ete = emit_tew & out_e[i];

            evn = emit_vns & out_n[i];
            etn = emit_tns & out_n[i];
            
            evw = emit_vew & out_w[i];
            etw = emit_tew & out_w[i];
            
            evs = emit_vns & out_s[i];
            ets = emit_tns & out_s[i];

        }

        void update_patch(
                  big_word& ove, //output valid (1 full, 0 empty) for e/n/w/s and destination for new result
                  big_word& ovn, //...word with (southern) input register for cells just north of patch
                  big_word& ovw, //   "    "    (eastern)  "     "        "   "     "    west  "  "
                  big_word& ovs,
                  big_word& ote, //output token (1 true, 0 other) for e/n/w/s and destination for new result
                  big_word& otn,
                  big_word& otw,
                  big_word& ots)
        {
                 
        
            big_word temp_ove;
            big_word temp_ote;
            big_word temp_ovw;
            big_word temp_otw;

            big_word emit_ve;
            big_word emit_te;
            big_word emit_vn;
            big_word emit_tn;
            big_word emit_vw;
            big_word emit_tw;
            big_word emit_vs;
            big_word emit_ts;
            
            for(unsigned int i = 0; i < BPW; i++)
            {
                //get state of outputs that we would write into when emmiting east
                temp_ove = vw[i]<<1 | (1 & ove>>i)>>0;
                temp_ovw = ve[i]>>1 | (1 & ovw>>i)<<(BPW-1);
                temp_ote = tw[i]<<1 | (1 & ote>>i)>>0;
                temp_otw = te[i]>>1 | (1 & otw>>i)<<(BPW-1);

                //update the word
                if(i == 0) {
                    update_word(i, temp_ove, ovn, temp_ovw, vn[i+1],
                                   emit_ve, emit_vn, emit_vw, emit_vs,
                                   emit_te, emit_tn, emit_tw, emit_ts);
                    ovn |= emit_vn;
                    vn[i+1] |= emit_vs;
                }
                else if(i == BPW - 1) {
                    update_word(i, temp_ove, vs[i-1], temp_ovw, ovs,
                                   emit_ve, emit_vn, emit_vw, emit_vs,
                                   emit_te, emit_tn, emit_tw, emit_ts);
                    vs[i-1] |= emit_vn;
                    ovs |= emit_vs;
                }
                else {
                    update_word(i, temp_ove, vs[i-1], temp_ovw, vn[i+1],
                                   emit_ve, emit_vn, emit_vw, emit_vs,
                                   emit_te, emit_tn, emit_tw, emit_ts);
                    vs[i-1] |= emit_vn;
                    vn[i+1] |= emit_vs;
                }

                ve[i] |= emit_vw<<1;
                vw[i] |= emit_ve>>1;
                
                ove |= (1 & (emit_ve         ))<<i;
                ovw |= (1 & (emit_vw>>(BPW-1)))<<i;
                ote |= (1 & (emit_te         ))<<i;
                otw |= (1 & (emit_tw>>(BPW-1)))<<i;

            }
        }
        std::ostream& py_dump(std::ostream& out) const;
        std::ostream& bin_dump(std::ostream& out) const;
        std::istream& bin_read(std::istream& in);
        
        std::ostream& ascii_dump_vs(std::ostream& out) const
        {
            out << "****HAS_TOKEN****\n";
            for(unsigned int i = 0; i < BPW; i++)
            {   
                out << bin<big_word>(ve[i]|vn[i]|vw[i]|vs[i]) << std::endl;
                //out << bin<big_word>(ve[i]) << std::endl;
            }            
        }

        void clear()
        {
            for(unsigned int i = 0; i < BPW; i++)
            {
                ef1[i] = 0; //gate can fire with one input (wire/not. Doesn't include crossover)
                ef2[i] = 0; //gate can fire with two inputs (and, nand, or, xor, copy, delete)
                cross[i] = 0; //cell is a crossover

                tf_pcl[i] = 0; //cell is in {or, xor, delete, wire, not}
                tf_pch[i] = 0; //cell is in {or, and, nand}
                inv_t[i] = 0; //cell is in {not, nand}

                ctrl_e[i] = 0; //cell is copy or delete with control from east 
                ctrl_n[i] = 0; //cell is copy or delete with control from north
                ctrl_w[i] = 0; //cell is copy or delete with control from west
                ctrl_s[i] = 0; //cell is copy or delete with control from south 

                copy_e[i] = 0; //cell is a copy with data from east 
                copy_n[i] = 0; //cell is a copy with data from north 
                copy_w[i] = 0; //cell is a copy with data from west 
                copy_s[i] = 0; //cell is a copy with data from south 
                is_cpy[i] = 0; // = copy_e | copy_n | copy_w | copy_s//cell is a copy cell

                is_del[i] = 0; //cell is a delete cell

                out_e[i] = 0; //cell output to the east
                out_n[i] = 0; //cell output to the north
                out_w[i] = 0; //cell output to the west
                out_s[i] = 0; //cell output to the south

                //which inputs have tokens
                ve[i] = 0; //valid flag for inputs from the east
                vn[i] = 0; //valid flag for inputs from the north
                vw[i] = 0; //valid flag for inputs from the west
                vs[i] = 0; //valid flag for inputs from the south

                //which inputs have true tokens
                //IF AN INPUT DOES NOT HAVE A TOKEN THIS MUST BE 0!
                te[i] = 0; //value flag inputs from the east
                tn[i] = 0; //value flag for inputs from the north
                tw[i] = 0; //value flag for inputs from the west
                ts[i] = 0; //value
            }
        }

        void make_wires_e_to_w()
        {
            for(unsigned int i = 0; i < BPW; i++)
            {
                ef1[i] = ~0; //gate can fire with one input (wire/not. Doesn't include crossover)
                ef2[i] = 0; //gate can fire with two inputs (and, nand, or, xor, copy, delete)
                cross[i] = 0; //cell is a crossover

                tf_pcl[i] = ~0; //cell is in {or, xor, delete, wire, not}
                tf_pch[i] = 0; //cell is in {or, and, nand}
                inv_t[i] = 0; //cell is in {not, nand}

                ctrl_e[i] = 0; //cell is copy or delete with control from east 
                ctrl_n[i] = 0; //cell is copy or delete with control from north
                ctrl_w[i] = 0; //cell is copy or delete with control from west
                ctrl_s[i] = 0; //cell is copy or delete with control from south 

                copy_e[i] = 0; //cell is a copy with data from east 
                copy_n[i] = 0; //cell is a copy with data from north 
                copy_w[i] = 0; //cell is a copy with data from west 
                copy_s[i] = 0; //cell is a copy with data from south 
                is_cpy[i] = 0; // = copy_e | copy_n | copy_w | copy_s//cell is a copy cell

                is_del[i] = 0; //cell is a delete cell

                out_e[i] = 0; //cell outputs to the east
                out_n[i] = 0; //cell outputs to the north
                out_w[i] = ~0; //cell outputs to the west
                out_s[i] = 0; //cell outputs to the south

                //which inputs have tokens
                ve[i] = 0x4040; //valid flag for inputs from the east
                vn[i] = 0; //valid flag for inputs from the north
                vw[i] = 0; //valid flag for inputs from the west
                vs[i] = 0; //valid flag for inputs from the south

                //which inputs have true tokens
                //IF AN INPUT DOES NOT HAVE A TOKEN THIS MUST BE 0!
                te[i] = 0; //value flag inputs from the east
                tn[i] = 0; //value flag for inputs from the north
                tw[i] = 0; //value flag for inputs from the west
                ts[i] = 0; //value
            }
        }
};
