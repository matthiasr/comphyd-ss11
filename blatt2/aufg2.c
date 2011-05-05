#include <stdlib.h>
#include <stdio.h>
#include <complex.h>
#include <math.h>
#include <png.h>
#include <assert.h>

#define NOBREAK

#define MAXITER 1000000000000

#define SMALL 1E-12
#define BIG (1./SMALL)
#define SIZE 501

double complex f(double complex z) {
    return z*z*z - 1;
}

double complex f_deriv(double complex z) {
    return 3*z;
}

struct newton_result {
    double complex root;
    long iterations; /* -1 if the iteration did not converge */
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

#ifndef NOBREAK
#ifdef MAXITER
        if(r.iterations > MAXITER) {
#else
        if(creal(r.root) > BIG || cimag(r.root) > BIG) {
#endif
            r.iterations = -1;
            break;
        }
#endif
    }

    return r;
}

void makeimage(char* fname) {
    FILE* fd = fopen(fname, "wb"); assert(fd!=NULL);

    png_structp pict = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL); assert(pict!=NULL);
    png_infop info = png_create_info_struct(pict); assert(info!=NULL);

    png_init_io(pict,fd);
    png_set_IHDR(pict, info, SIZE, SIZE, \
         8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, \
         PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_bytep data = (png_bytep) malloc(3*SIZE*SIZE*sizeof(png_byte)); assert(data!=NULL);

    unsigned int i, j, c;
    struct newton_result r;
    double complex zeros[3];
    zeros[0] = 1;
    zeros[1] = cexp(2.*M_PI*I/3.);
    zeros[2] = cexp(4.*M_PI*I/3.);
    const png_byte R[4] = {255, 0, 255, 0};
    const png_byte G[4] = {255, 255, 0, 0};
    const png_byte B[4] = {255, 0, 0, 255};


#pragma omp parallel for private(i,j,r,c) shared(data,zeros,R,G,B)
    for(i=0;i<SIZE;i++) {
        for(j=0;j<SIZE;j++) {
            r = newton(f, f_deriv, \
                    ((10./(double)SIZE)*i - 5.0) + \
                    I*((10./(double)SIZE)*i - 5.0));
            if(r.iterations == -1) {
                c = 0;
            }
            if(cabs(r.root - zeros[0]) < 10*SMALL) {
                c = 1;
            } else if(cabs(r.root - zeros[1]) < 10*SMALL) {
                c = 2;
            } else if(cabs(r.root - zeros[2]) < 10*SMALL) {
                c = 3;
            } else {
                c = 0;
            }

        data[(i*SIZE+j)*3] = R[c];
        data[(i*SIZE+j)*3+1] = G[c];
        data[(i*SIZE+j)*3+2] = B[c];

        }
    }

    png_write_info(pict,info);

    for(i=0;i<SIZE;i++) {
        png_write_row(pict, data+3*SIZE*i);
    }

    png_write_end(pict,info);
    png_destroy_write_struct(&pict,&info);

    free(data); data = NULL;
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
            printf("z = %g + %gI\n", creal(z), cimag(z));
            // if(isnan(z)) usage(argv[0]);
            struct newton_result r = newton(f,f_deriv,z);
            printf("root = %g + %gI\niterations = %ld\n", creal(r.root), cimag(r.root), r.iterations);
            exit(0);
    } else if (argc == 2) {
            makeimage(argv[1]);
            exit(0);
    } else {
            usage(argv[0]);
    }

    exit(255); /* should never be reached */
}
