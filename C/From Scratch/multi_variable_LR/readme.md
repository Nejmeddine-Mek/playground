# Multi-Variable Linear Regression in C (Single-Threaded Baseline)

A multi-variable linear regression implementation in C using batch gradient descent on Mean Squared Error (MSE). This initial commit establishes a clean, single-threaded reference implementation before adding POSIX threads (`<pthread.h>`) and atomics (`<stdatomic.h>`).

## Requirements

- **C23 standard** (`-std=c23` or `-std=gnu23`)
- A C23-compliant compiler (such as GCC / MinGW-w64)

## What Was Built (Current State)

- **Prediction (`y_hat` and `predict`):** Computes linear predictions $\hat{y}_i = \mathbf{x}_i \cdot \mathbf{w} + b = \sum_{j=0}^{m-1} x_{i,j} w_j + b$ for sample row `i` across `x_len` features, and allocates a heap array of `y_len` predictions in `predict` (to be freed by the caller via `free()`).
- **MSE Partial Derivatives (`d_MSE_dwi` and `d_MSE_db`):** Evaluates the partial derivatives of $\text{MSE} = \frac{1}{n}\sum_{i=0}^{n-1} (\hat{y}_i - y_i)^2$ with respect to each feature weight $w_j$ and the bias $b$.
- **In-Place Gradient Descent (`fit`):** Iteratively updates `w` (of length `x_len`) and `*b` up to `MAX_ITER`, stopping early once parameter step magnitudes fall below `tol` (falling back to `DEFAULT_TOL` when `tol <= 0.0`) or if values become non-finite.
- **Forward-Compatible Signature:** Keeps `n_thread` (`uint8_t`) in `fit` and `predict` so the public interface stays stable when concurrency is added.

## What Comes Next (Roadmap)

1. **Input & Bounds Hardening:** Add full null-pointer checks, zero-length guards (`x_len == 0`, `y_len == 0`), feature-index bounds checks (`wi >= x_len`), and finite-value checks so bad inputs or exploding learning rates fail safely.
2. **Single-Pass Gradient Accumulation:** Compute prediction error once per sample `i` and accumulate gradients across all features `j` and bias `b` in a single pass instead of recomputing `y_hat` repeatedly per feature.
3. **POSIX Threads (`<pthread.h>`) & Atomics (`<stdatomic.h>`):** Partition sample rows across worker threads using per-thread gradient accumulators to avoid lock contention and false sharing, reducing results once per iteration and coordinating work/flags with atomic operations.
4. **Memory Layout & Evaluation Metrics:** Compare row-pointer matrices (`const double *const *x`) against contiguous flat buffers (`const double *x`), and add evaluation helpers such as MSE, RMSE, MAE, and $R^2$ alongside regularization options (such as L2 / Ridge).

## Build and Run

```bash
gcc -std=c23 -Wall -Wextra -Werror -O2 main.c mlr.c -lm -o mlr
./mlr