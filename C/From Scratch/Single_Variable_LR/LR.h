#ifndef LR_H
#define LR_H

#include <math.h>
#include <stdlib.h>
#include <stddef.h>

#define MAX_ITER 1'000'000
#define DEFAULT_TOL 1e-6

typedef struct {
    double a;
    double w;
}slr_t;


slr_t fit(const double *x,const double *y,size_t len, double a, double w, double alpha, double tol);
double *predict(slr_t func,const double *x, size_t len);

#endif