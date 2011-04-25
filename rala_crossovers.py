#! /usr/bin/python

import sys, os

openfile = open(sys.argv[1])
outputfile = open(sys.argv[2], "w")

data = openfile.read().replace(" ","").replace("\n","")
data = data.split("}")

openfile.close()

wires = { }

def merge_spec( a, b ):
    assert len(a) == 3
    assert len(b) == 3
    return "C%s%s" % (a[1:], b[1:])

for statement in data:
    if not statement:
        continue
    if statement[0] != "{":
        print "Weird:", statement
        continue
    statement = statement[1:]
    xy, spec = statement.split(";")
    typeof = spec[0]
    if typeof != "w":
        outputfile.write("{"+statement+"}")
        continue

    x, y = xy.split(",")
    xy = (int(x), int(y))

    if xy in wires:
        wires[xy] = merge_spec(spec, wires[xy])
    else:
        wires[xy] = spec

for loc, spec in wires.iteritems():
    outputfile.write( "{%s,%s;%s}" % (loc[0], loc[1], spec) )

outputfile.close()

