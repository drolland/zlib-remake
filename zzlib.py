#!/usr/bin/python

import zlib

bytes = str.encode("aaa")
string = zlib.compress(bytes)
print(string)

f = open('test.z', 'wb')
f.write(string)
f.close()
