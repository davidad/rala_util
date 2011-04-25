#This should be run like "./rala_util --cell-size=5 test.ru"

#new context, like in (La)TeX
{

#translate: "x,y;"
  -2,-2;

#This next part is more complicated.  We open a new context, and then define a
#parellel transform on it.  Parallel transforms are bracketed by [] and trump
#the current local transform (but they take on the current global transform).
#A special type of parallel transform is a range.  A range is like a translate
#with iteration.  X and Y iterate independently (one or both).  The syntax is
# xmin:xmax*xstep,ymin:ymax*ystep
#but the min and max are multiplied by the step, unlike in python (so it's
#-1:1*30, not -30:30*30).
  {[-1:1*30,-1:1*30;]

#Now we open another context, and do what I call the "pinwheel transform":
#pass-through, plus rotate 90, plus rotate 180, plus rotate 270.
#Each transform in brackets is summed, and that sum is multiplied by all
#the possibilities from the previous parallel transforms.
#This line also introduces flip/rotate notation.  Flip/rotate transforms
#begin with ~, and the second character determines the transform. I chose
#these second characters to be vaguely graphically indicative:
# ~' rotate 90
# ~- rotate 180
# ~, rotate 270
# ~| flip X
# ~_ flip Y
# ~/ swap X and Y (flip along Y=X)
# ~\ swap+negate X and Y (flip along Y=-X)
    {[][~'][~-][~,]

#Now we simply move out a bit along the X axis to see the pinwheel effect:
      {3,0;

#Now we implement an XY range by multiplying an X range and a Y range, just
#to test that it works.
        {[0:2*4,0;]
          {[0,0:2*4;]

#And inside each of those 4x4 blocks, we now put our "unit cell", an XOR
#in 0,0, 1,0, 0,1, and 2,2.
	    {
              [0:1,0;]
	      [0,1;]
	      [2,2;]
	      x E0
            }

#Now we close all the contexts.  This is not actually necessary, since
#the cells we want have already been drawn, but it makes editing in vim
#easier, and helps rala_util clean up its memory.
          }
        }
      }
    }
  }
}
