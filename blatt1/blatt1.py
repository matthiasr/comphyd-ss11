#!/usr/bin/env python
# -*- coding: utf-8 -*-
from rk import RK_Solver
from math import exp,log

euler = RK_Solver(None,[1],None)
heun = RK_Solver([[1]],[0.5,0.5],[0,1])
rk4 = RK_Solver([[0.5],[0,0.5],[0,0,1]],[1./6.,1./3.,1./3.,1./6.],[0.0,0.5,0.5,1])

# Aufgabe 1
f = lambda x,y:-2*x*y*y
exact = lambda x: 1./(1+x*x)
x0 = 0
y0 = 1
emax = dict(((euler,{}),(heun,{}),(rk4,{})))

for h in [0.1,0.01,0.001,0.0001]:
    N = int(1/h)
    xx = [x0+n*h for n in xrange(N+1)]

    for s in [euler, heun, rk4]:
        s.solve(f,x0,y0,h,N,exact)
        emax[s][h] = max(abs(d) for d in s.deltas)

    print """set term 'pdf' size 21cm,29.7cm
set output 'aufgabe1-""" + str(h) + """.pdf'
set multiplot layout 2,1

set title 'Aufgabe1 - Results for h = """ + str(h) + """'
plot '-' with lines title "Exact", '-' with lines title 'Euler', '-' with lines title "Heun", '-' with lines title "Runge-Kutta(4)" """
    for i in xrange(N+1):
        print xx[i], exact(xx[i])
    print "e"
    for i in xrange(N+1):
        print xx[i], euler.results[i]
    print "e"
    for i in xrange(N+1):
        print xx[i], heun.results[i]
    print "e"
    for i in xrange(N+1):
        print xx[i], rk4.results[i]
    print "e"

    print """
set title 'Aufgabe 1 - Errors for h = """ + str(h) + """'
plot '-' with lines title 'Euler', '-' with lines title "Heun", '-' with lines title "Runge-Kutta(4)" """
    for i in xrange(N+1):
        print xx[i], euler.deltas[i]
    print "e"
    for i in xrange(N+1):
        print xx[i], heun.deltas[i]
    print "e"
    for i in xrange(N+1):
        print xx[i], rk4.deltas[i]
    print "e"

    print """


unset multiplot
unset output
"""

print """set term pdf size 29.7cm,21cm
set output "aufgabe1-errors.pdf"
set title "Aufgabe 1 - Maximum Errors"

set xlabel "log(1/h)"
set ylabel "max(abs(\Delta y))"

plot '-' with points title "Euler", '-' with points title "Heun", '-' with points title "Runge-Kutta(4)" """
for h,e in sorted(emax[euler].iteritems()):
    print log(1/h), log(e)
print "e"
for h,e in sorted(emax[heun].iteritems()):
    print log(1/h), log(e)
print "e"
for h,e in sorted(emax[rk4].iteritems()):
    print log(1/h), log(e)
print "e"

print """unset output"""

# Aufgabe 2
f = lambda x,y:-1000*y
exact = lambda x: exp(-1000*x)
x0 = 0
y0 = 1

