#!/usr/bin/env python
from rk import RK_Solver
from math import exp
from sys import argv,exit

def usage():
    print "Usage:\n\t" + argv[0] + " <Aufgabennummer> <h>"
    exit()

if len(argv) != 3:
    usage()

if argv[1] == "1":
    f = lambda x,y:-2*x*y*y
    exact = lambda x: 1./(1+x*x)
    x0 = 0
    y0 = 1
elif argv[1] == "2":    
    f = lambda x,y:-1000*y
    exact = lambda x: exp(-1000*x)
    x0 = 0
    y0 = 1
else:
    usage()

try:
    h = float(argv[2])
except ValueError:
    usage()

euler = RK_Solver(None,[1],None)
heun = RK_Solver([[1]],[0.5,0.5],[0,1])
rk4 = RK_Solver([[0.5],[0,0.5],[0,0,1]],[1./6.,1./3.,1./3.,1./6.],[0.0,0.5,0.5,1])

N = int(1/h)
xx = [x0+n*h for n in xrange(N+1)]

for s in (euler, heun, rk4):
    s.solve(f,x0,y0,h,N,exact)

for i in xrange(N+1):
    print xx[i], exact(xx[i]), euler.results[i], heun.results[i], rk4.results[i]
