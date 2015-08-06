#!/usr/bin/python

file = open("out.txt","wb")

list_int = []
for x in range(0,255):
	list_int.append(x)

bits = bytes(list_int)
print(bits)
	
file.write(bits)
file.close()
