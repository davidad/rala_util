#! /usr/bin/python

import sys, os

if len(sys.argv) != 2:
    print "usage: %s experiment" % sys.argv[0]
    raise SystemExit

to_capture = { }

openfile = open( sys.argv[1] )

n = 1

while os.path.exists( "results%s" % n ):
    n += 1

os.mkdir("results%s" % n)
os.chdir("results%s" % n)

for fullline in openfile:
      # Do some basic parsing typical of many scriping languages
    fullline = fullline.strip().split("#")[0]
    if not fullline: continue

    line = fullline.split(" ")
    cmd, args = line[0], line[1:]

    if cmd == "binary":
        try:
            if len(args) > 2:
                outputfile = args[2]
            else:
                outputfile = "capture_%s_%s" % ( args[0], args[1] )
            to_capture[ "%s,%s" % (args[0], args[1]) ] = open( outputfile, "w" )
        except KeyboardInterrupt:
            print "Invalid syntax: %s" % fullline
            print "Should use: capture <x> <y> [file]"
            raise SystemExit

    else:
        print "Unknown command: %s" % cmd
        raise SystemExit

while True:
    line = sys.stdin.readline().strip()

    if not line:
        break

    if line[0] == "{" and line[-1] == "}":
        line = line[1:-1]
        xy, notification = line.split(";")

        if len(notification) == 2 and notification[0] in ("N","S","E","W"):
            if xy in to_capture:
                if notification[1] in ("1","0"):
                    to_capture[xy].write( notification[1] )

sys.stderr.write("Done.\n")
sys.stderr.close()

for openfile in to_capture.values():
    openfile.write("\n")
    openfile.close()

