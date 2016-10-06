#!/usr/bin/python3

import fileinput
from sys import exit

a = dict()
for line in fileinput.input():
    line=line.rstrip()
    if line in a:
        print("Dubbletter")
        exit()
    else:
        a[line] = None
print("Ej dubblett")
