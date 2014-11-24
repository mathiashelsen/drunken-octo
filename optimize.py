#!/usr/bin/python
import subprocess 
import scipy.optimize

def f(x):
    p = subprocess.Popen(['./drunken-octo', str(x[0]), str(x[1]), str(x[2]), str(x[3]), str(x[4])], stdout=subprocess.PIPE) 
    p.wait()
    out, err= p.communicate()
    return float(out)

x0 = [1.0, 1.0, 1.0, 1.0, 1.0]
res = scipy.optimize.basinhopping(f, x0)
#print res['x']
print res
res = scipy.optimize.minimize(f, x0, method='Nelder-Mead', tol=1e-6)
print res
