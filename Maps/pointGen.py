#!/usr/bin/python3
import random
import sys

n = 8
k = 3
metric = 0
maximium = 1000
minimium = 0

f = open('./test.txt', 'w')
f.write( str(n) + '\n')
f.write( str(k) + '\n')
f.write( str(metric) + '\n')

i = 0
while i<n:
	f.write( str(random.randint( minimium, maximium )) + " " + str(random.randint( minimium, maximium )) +  " 0" + '\n')
	i = i + 1
	
f.close()
