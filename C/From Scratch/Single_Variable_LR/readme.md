# Single-Variable Linear Regression in C

A minimal single-variable linear regression implementation in C using batch gradient descent on Mean Squared Error (MSE).

## Requirements

- **C23 standard** (`-std=c23` or `-std=gnu23`)
- A C23-compliant compiler (such as GCC / MinGW-w64)

## Project Structure

- `LR.h`: Model struct (`slr_t`), configuration macros (`MAX_ITER`, `DEFAULT_TOL`), and public function declarations (`fit`, `predict`).
- `LR.c`: Gradient descent training loop (`fit`), MSE partial derivatives, and inference routine (`predict`).
- `main.c`: Train/test split example, prediction loop, and cleanup.

## API Overview

```c
typedef struct {
    double a; // slope
    double w; // intercept
} slr_t;

slr_t fit(const double *x, const double *y, size_t len,
          double a, double w, double alpha, double tol);

double *predict(slr_t func, const double *x, size_t len);