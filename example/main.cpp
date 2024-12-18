#include "nlopt.h"
#include "slsqp.h"
#include <stdio.h>

double objective(unsigned n, const double *x, double *grad, void *data) {
    // Objective function: f(x) = x[0]^2 + x[1]^2
    if (grad) {
        grad[0] = 2 * x[0];
        grad[1] = 2 * x[1];
    }
    return x[0] * x[0] + x[1] * x[1];
}

int main() {
    unsigned n = 2; // Number of variables
    unsigned m = 0; // No inequality constraints
    unsigned p = 0; // No equality constraints
    double lb[2] = {-10.0, -10.0}; // Lower bounds
    double ub[2] = {10.0, 10.0};   // Upper bounds
    double x[2] = {2.0, 2.0};      // Initial guess
    double minf;                   // Minimum value of the objective function

    nlopt_stopping stop; // see api/nlopt-internal.h for detail
    stop.minf_max = -10000000;
    stop.ftol_rel = 1e-6;
    stop.ftol_abs = 1e-6;
    stop.xtol_rel = 1e-6;
    int num_evals = 0;
    stop.nevals_p = &num_evals;
    stop.maxeval = 1000;
    stop.maxtime = 10000;
    stop.start = 0;
    stop.force_stop = 0;

    nlopt_result result = nlopt_slsqp(n, objective, NULL, m, NULL, p, NULL, lb, ub, x, &minf, &stop);

    if (result > 0) {
        printf("Optimization succeeded!\n");
        printf("Optimal x: [%f, %f]\n", x[0], x[1]);
        printf("Minimum value: %f\n", minf);
    } else {
        printf("Optimization failed with code: %d\n", result);
    }
    return 0;
}
