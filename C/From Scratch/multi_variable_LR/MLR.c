#include "./LR.h"

#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>

double y_hat(const double *const *x, const double *w, double b, size_t i, size_t n_features);
double d_MSE_dwi(const double *const *x, const double *y, size_t n_features, size_t n_samples, const double *w, size_t wi, double b);
double d_MSE_db(const double *const *x, const double *y, size_t n_features, size_t n_samples, const double *w, double b);
/*each thread will compute a row at a time*/
atomic_size_t g_x_len;

int fit( const double *const *x, const double*y,const size_t x_len,const size_t y_len, double *w, double *b, double lr, double tol, uint8_t n_thread){
    /*things will be modified in place*/
    if(!x || !y || !w || !b || x_len <= 0 || y_len <= 0)
        return 1;

    if(tol == 0) tol = DEFAULT_TOL;

    double *step_w = (double *)malloc(x_len * sizeof(double));
    double step_b = 0.0;
    if(!step_w)
        return -1;
    for(size_t i = 0; i < MAX_ITER; ++i){
        size_t f_under_tol = 0;

        for(size_t j = 0; j < x_len; ++j){
            step_w[j] = lr * d_MSE_dwi(x,y,x_len, y_len, w, j, *b);
            w[j] -= step_w[j];
            if(fabs(step_w[i]) < tol)
                f_under_tol += 1;
        }
        step_b = lr * d_MSE_db(x, y, x_len, y_len, w, *b);
        *b -= step_b;

        if (!isfinite(*b)) {
            free(step_w);
            return -2;
        }

        int w_finite = 1;
        for (size_t j = 0; j < x_len; ++j) {
            if (!isfinite(w[j])) {
                w_finite = 0;
                break;
            }
        }
        if (!w_finite) {
            free(step_w);
            return -2;
        }

        if (f_under_tol == x_len && fabs(step_b) < tol) {
            break;
        }
    }

    free(step_w);
    return 0;

}

double *predict(const double *const *x,  double *w, double b, const size_t x_len,const size_t y_len, uint8_t n_threads){
    
    double *y_hats = (double *)malloc(y_len * sizeof(double));
    
    if(!y_hats) return NULL;
    
    for(size_t i = 0; i < y_len; ++i){
        y_hats[i] = y_hat(x, w, b, i, x_len);
    }
    return y_hats;
}

double y_hat(const double *const *x, const double *w, double b, size_t i, size_t n_features) {
    if (!x || !w || !x[i]) {
        return 0.0;
    }

    double res = 0.0;
    for (size_t j = 0; j < n_features; ++j) {
        res += w[j] * x[i][j];
    }
    return res + b;
}

double d_MSE_dwi(const double *const *x, const double *y, size_t n_features, size_t n_samples, const double *w, size_t wi, double b) {
    if (!x || !y || !w || n_samples == 0 || wi >= n_features) {
        return 0.0;
    }

    double res = 0.0;
    for (size_t i = 0; i < n_samples; ++i) {
        if (!x[i]) {
            return 0.0;
        }
        res += x[i][wi] * (y_hat(x, w, b, i, n_features) - y[i]);
    }

    return (2.0 * res) / (double)n_samples;
}

double d_MSE_db(const double *const *x, const double *y, size_t n_features, size_t n_samples, const double *w, double b) {
    if (!x || !y || !w || n_samples == 0) {
        return 0.0;
    }

    double res = 0.0;
    for (size_t i = 0; i < n_samples; ++i) {
        if (!x[i]) {
            return 0.0;
        }
        res += y_hat(x, w, b, i, n_features) - y[i];
    }

    return (2.0 * res) / (double)n_samples;
}
