#include "LR.h"



double d_MSE_da(const double *x,const double *y, size_t len ,double a, double w);
double d_MSE_dw(const double *x,const double *y, size_t len ,double a, double w);
[[maybe_unused]]
double MSE(const double *x,const double *y, size_t len ,double a, double w);

slr_t fit(const double *x,const double *y,size_t len, double a, double w, double alpha, double tol){
    /*just in case someone is stupid enough*/
    slr_t res = {.a = a, .w = w};
    
    if (!x || !y || len == 0) {
        return res;
    }
    if(tol <= 0)
        tol = DEFAULT_TOL;



    for(size_t i = 0; i < MAX_ITER; ++i){
        double grad_a = d_MSE_da(x, y, len, res.a, res.w);
        double grad_w = d_MSE_dw(x, y, len, res.a, res.w);

        double step_a = alpha * grad_a;
        double step_w = alpha * grad_w;

        res.a -= step_a;
        res.w -= step_w;

        if(fabs(step_a) < tol && fabs(step_w) < tol) break;
    }

    return res;
}

double *predict(slr_t func,const double *x, size_t len){
    double *y_hat = (double *)malloc(len * sizeof(double));

    if(!y_hat) return NULL;
    
    for(size_t i = 0; i < len; ++i)
        y_hat[i] = func.a * x[i] + func.w;
    
    return y_hat;
}

double MSE(const double *x,const double *y, size_t len ,double a, double w){
    double result = 0;

    for(size_t i = 0; i < len; ++i){
        double err = (a * x[i] + w) - y[i];
        result += err * err;
    }

        return result / len;
}

double d_MSE_da(const double *x,const double *y, size_t len ,double a, double w){
    double result = 0;
    for(size_t i = 0; i < len; ++i)
        result += x[i] * ((a * x[i] + w) - y[i]);

        return (result * 2) / len;
}
double d_MSE_dw(const double *x,const double *y, size_t len ,double a, double w){
    double result = 0;
    for(size_t i = 0; i < len; ++i)
        result += ((a * x[i] + w) - y[i]);

        return (result * 2) / len;
}
 