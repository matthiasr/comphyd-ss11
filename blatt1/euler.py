#!/usr/bin/env python

SMALL = 1E-6

# Implicit Euler solver
class Implicit_Euler:
    # results from last calculation
    x0 = 0
    y0 = 0
    h = 0
    N = 0
    results = []


    # if available: exact solution and extra info
    # on last calculation
    exact = None
    deltas = []

    def solve(self, f, x0, y0, h, N, exact=None):
        self.x0 = x0
        self.y0 = y0
        self.h = h
        self.N = N
        self.exact = exact
        if exact: self.deltas = [0]

        self.results = [y0]
        for n in xrange(N):
            x = x0 + n*h
            old_y = self.results[n]
            self.results.append( self.findzero(lambda y: f(x,y)-old_y, old_y) )
            if exact: self.deltas.append(self.results[-1]-exact(x+h))
        return self.results[-1]

    def findzero(self, f, x, deriv=None):
        if deriv == None:
            deriv = lambda x: (f(x+SMALL)-f(x-SMALL))/(2*SMALL)

        while abs(f(x)) > SMALL:
            x -= f(x) / deriv(x)

        return x
