#! /usr/bin/python

import sys
print "\n".join( "%s: %s gates, %s wires" % ((path,)+((lambda x : (x.count("}"), x.count(";w")))(open(path).read()))) for path in sys.argv[1:] )
