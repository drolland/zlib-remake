#!/usr/bin/python

file = open("out.txt","wb")

list_int = []
for j in range(0,16):
	for x in range(0,255):
		list_int.append(x)

bits = bytes(list_int)
print(bits)
	
print('i love you')
file.write(bits)
file.close()
