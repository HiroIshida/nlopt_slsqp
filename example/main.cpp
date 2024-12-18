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

double inequality_constraint(unsigned n, const double *x, double *grad, void *data) {
  // Inequality constraint: -x[0] + x[1] + 1 <= 0
    if (grad) {
        grad[0] = -1.0;
        grad[1] = 1.0;
    }
    return -x[0] + x[1] + 1.0;
}

int main() {
    unsigned n = 2;              // Number of variables
    unsigned m = 1;              // One inequality constraint
    unsigned p = 0;              // Zero equality constraint
    double lb[2] = {-10.0, -10.0}; // Lower bounds
    double ub[2] = {10.0, 10.0};   // Upper bounds
    double x[2] = {2.0, 2.0};      // Initial guess
    double minf;                    // Minimum value of the objective function

    nlopt_constraint fc[1];
    fc[0].f = inequality_constraint;
    fc[0].f_data = NULL;
    fc[0].pre = NULL;
    fc[0].m = 1;
    double tol = 1e-4;
    fc[0].tol = &tol;

    // Stopping criteria (see: nlopt/src/api/option.c for default values)
    nlopt_stopping stop;
    stop.minf_max = -10000000;
    stop.ftol_rel = 1e-6;
    stop.ftol_abs = 1e-6;
    stop.xtol_rel = 0;
    int num_evals = 0;
    stop.x_weights = NULL;
    stop.xtol_abs = NULL;
    stop.nevals_p = &num_evals;
    stop.maxeval = 1000;
    stop.maxtime = 10000;
    stop.start = 0;
    stop.force_stop = 0;

    nlopt_result result = nlopt_slsqp(n, objective, NULL, 
                                    m, fc,
                                    p, NULL,
                                    lb, ub, x, &minf, &stop);

    if (result > 0) {
        printf("Optimization succeeded!\n");
        printf("Optimal x: [%f, %f]\n", x[0], x[1]);
        printf("Minimum value: %f\n", minf);
        printf("Inequality constraint value: %f\n", 
               inequality_constraint(n, x, NULL, NULL));
    } else {
        printf("Optimization failed with code: %d\n", result);
    }

    auto is_near = [](double a, double b) {
        return fabs(a - b) < 1e-6;
    };
    if (is_near(x[0], 0.5) && is_near(x[1], -0.5) && is_near(minf, 0.5)) {
        return 0;
    } else {
        return 1;
    }
}
