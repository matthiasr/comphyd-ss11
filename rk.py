#!/usr/bin/env python

SMALL = 1E-6

# general Runge-Kutta-Solver
class RK_Solver:
    # parameters
    a = [[]]
    b = []
    c = []

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

    def __init__(self, a, b, c):
        # check consistency and initialise parameters
        order = len(b)
        if abs(sum(b) - 1) < SMALL:
            self.b = b
        else:
            raise ValueError("sum of b's not 1")
        if a == None: a = []
        if c == None: c = []
        if len(c) == order-1:
            self.c = [0] + c
        elif len(c) == order and c[0] == 0:
            self.c = c
        else:
            raise ValueError("invalid number of c's")
        if len(a) == order-1:
            try:
                if reduce(lambda x,y:x and y,
                        ( abs(sum(a[i])-c[i+1]) < SMALL
                            for i in xrange(order-1)), True):
                    self.a = a
                else:
                    raise ValueError("invalid a's")
            except IndexError:
                raise ValueError("invalid a's array size")
        else:
            raise ValueError("invalid a's array size")

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
            y = self.results[n]
            k = []
            for i in xrange(len(self.b)):
                k.append( f(x+self.c[i]*h,y + sum(self.a[i-1][j]*h*k[j] for j in xrange(i-1))) )
            self.results.append(y + h*sum(self.b[i]*k[i] for i in xrange(len(self.b))))
            if exact: self.deltas.append(self.results[-1]-exact(x+h))
        return self.results[-1]
