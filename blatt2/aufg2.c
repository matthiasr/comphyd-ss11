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
#define SIZE 16000

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803
#endif

double complex f(double complex z) {
    return z*z*z - 1;
}

double complex f_deriv(double complex z) {
    return 3*z*z;
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
    png_write_info(pict,info);

    png_bytep data;

    int i, j, c;
    struct newton_result r;
    double complex zeros[3] = { 1, cexp(2.*M_PI*I/3.), cexp(4.*M_PI*I/3.) };
    const png_byte R[4] = {255, 0, 255, 0};
    const png_byte G[4] = {255, 255, 0, 0};
    const png_byte B[4] = {255, 0, 0, 255};


#pragma omp parallel for ordered private(i,j,r,c,data) shared(pict)
    for(i=0;i<SIZE;i++) {
        data = (png_bytep) malloc(3*SIZE*sizeof(png_byte)); assert(data!=NULL);
        for(j=0;j<SIZE;j++) {
            r = newton(f, f_deriv, \
                    ((10./(double)SIZE)*j - 5.0) + \
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

        data[j*3] = R[c];
        data[j*3+1] = G[c];
        data[j*3+2] = B[c];

        }
#pragma omp ordered
        png_write_row(pict, data);
        free(data); data = NULL;
    }

    png_write_end(pict,info);
    png_destroy_write_struct(&pict,&info);
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
