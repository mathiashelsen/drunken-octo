#!/usr/bin/python
import subprocess 

def f(x):
    p = subprocess.Popen(['./drunken-octo', str(x[0]), str(x[1]), str(x[2]), str(x[3]), str(x[4])], stdout=subprocess.PIPE) 
    p.wait()
    out, err= p.communicate()
    return float(out)

print f([10.0, 10.0, 1.0, 1.0, 1.0])
