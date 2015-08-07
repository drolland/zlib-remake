#!/usr/bin/python

from pylab import *
import re

file = open("graph2.txt","r")

r = re.compile(" ")

x = []
y = []

for line in file:
	sp = r.split(line.rstrip('\n'))
	x.append(int(sp[0]))
	y.append(float(sp[1]))

plot(x,y)
show()

