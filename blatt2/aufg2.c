#include <stdlib.h>
#include <stdio.h>
#include <complex.h>
#include <math.h>
#include <png.h>
#include <assert.h>

#define SMALL 1E-6
#define BIG 1E6
#define RESOLUTION 501

double complex f(double complex z) {
    return z*z*z - 1;
}

double complex f_deriv(double complex z) {
    return 3*z;
}

struct newton_result {
    double complex root;
    int iterations; /* -1 if the iteration did not converge */
};

struct newton_result newton(double complex function(double complex), \
                            double complex derivative(double complex), \
                            double complex z0) {
    struct newton_result r;

    r.root = z0;
    r.iterations = 0;

    while ( cabs(function(r.root)) > SMALL ) {
        r.iterations++;
        r.root -= function(r.root) / derivative(r.root);

        if(creal(r.root) > BIG || cimag(r.root) > BIG) {
            r.iterations = -1;
            break;
        }
    }

    return r;
}

void makeimage(char* fname) {
    /* TODO */
}

void usage(char* progname) {
    printf("Usage:\n"
            "\t%s <filename>\tgenerate image and save in PNG format\n"
            "\t%s x y\tprint root and number of iterations for starting point x+yI\n", \
            progname, progname);
    exit(1);
}

int main(int argc, char** argv) {
    if(argc == 3) {
            double complex z;
            z = strtod(argv[1], (char**)NULL) + I*strtod(argv[2], (char**)NULL);
            // if(isnan(z)) usage(argv[0]);
            struct newton_result r = newton(f,f_deriv,z);
            printf("root = %g + %gI\niterations = %d\n", creal(r.root), cimag(r.root), r.iterations);
            exit(0);
    } else if (argc == 2) {
            makeimage(argv[1]);
            exit(0);
    } else {
            usage(argv[0]);
    }

    exit(255); /* should never be reached */
}
