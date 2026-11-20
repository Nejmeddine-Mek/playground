#ifndef MLR_H
#define MLR_H


#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>



#define MAX_ITER 1'000'000
#define DEFAULT_TOL 1e-6


typedef struct {
    double *w;
    double b;
}mlr_t;

int fit( const double *const *x, const double*y,const size_t x_len,const size_t y_len, double *w, double *b, double lr, double tol, uint8_t n_thread);
double *predict(const double *const *x,  double *w, double b, const size_t x_len,const size_t y_len, uint8_t n_threads);

    


#endif