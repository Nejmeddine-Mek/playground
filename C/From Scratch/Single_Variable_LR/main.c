#include "./LR.h"
#include <stdio.h>
#include <stddef.h>

int main(int argc, char **argv){

    const size_t N1 = 12;
    const double X1[12] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 12.0, 15.0};
    const double y1[12] = {15.5, 19.0, 22.5, 26.0, 29.5, 33.0, 36.5, 40.0, 43.5, 47.0, 54.0, 64.5};

    const size_t n_fit = N1 * 0.8;
    const size_t n_pred = N1 - n_fit;
    slr_t lr = fit(X1, y1, 12 * 0.8, 0, 0, 0.01, 0.001);

    double *y_hat = predict(lr, X1 + n_fit, n_pred);
    if (!y_hat) {
        return 1;
    }
    printf("parameters: a = %lf, w = %lf\n", lr.a, lr.w);
    for (size_t j = 0; j < n_pred; ++j) {
        size_t idx = n_fit + j;
        printf("y[%zu] = %lf, y_hat[%zu] = %lf\n", idx, y1[idx], j, y_hat[j]);
    }

    free(y_hat);
    return 0;
}