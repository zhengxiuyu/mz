#include <mz.h>
#include "common.h"

#define EPS 0.0

static double eval_poly6(double r, double h) {
    double h2 = h * h;
    double h8 = h2 * h2 * h2 * h2;
    double a = h2 - r * r;
    return 4.0 / (M_PI * h8) * a * a * a;
}

static double eval_spiky_grad(double r, double h) {
    double h5 = h * h * h * h * h;          /* numerical error if h is small? */
    double b = h - r;
    return -30.0 / (M_PI * h5) * b * b;
}

void mz_calc_lambdas_naive(
    struct mz_fluid *fluid,
    const struct mz_parameters *parameters
) {
    int i, j;
    double invrest = 1.0 / fluid->rest_density;
    double support = parameters->support;
    double eps = parameters->relaxation;

    for (i = 0; i < fluid->num_particles; i++) {
        double pi[2], dens = 0.0;
        double a = 0.0, b[2];

        memcpy(pi, fluid->positions[i], sizeof(pi));
        memset(b, 0, sizeof(b));
        for (j = 0; j < fluid->num_particles; j++) {
            double pij[2], len;

            mz_sub(pij, pi, fluid->positions[j]);
            len = mz_length(pij); 
            if (len < support) {
                dens += eval_poly6(len, support);

                if (len > 0.000001) {
                    double spiky = eval_spiky_grad(len, support);
                    double grad[2];

                    grad[0] = invrest * spiky * pij[0] / len;
                    grad[1] = invrest * spiky * pij[1] / len;
                    a += mz_dot(grad, grad);
                    b[0] += grad[0];
                    b[1] += grad[1];
                }
            }
        }
        fluid->densities[i] = dens;
        fluid->lambdas[i] = -(dens * invrest - 1.0) / (a + mz_dot(b, b) + eps);
    }
}

void mz_calc_dpositions_naive(
    struct mz_fluid *fluid,
    const struct mz_parameters *parameters
) {
    int i, j;
    double invrest = 1.0 / fluid->rest_density;
    double support = parameters->support;

    for (i = 0; i < fluid->num_particles; i++) {
        double dpos[2], pi[2];
        double lami = fluid->lambdas[i];

        memset(dpos, 0, sizeof(dpos));
        memcpy(pi, fluid->positions[i], sizeof(pi));
        for (j = 0; j < fluid->num_particles; j++) {
            double pij[2], len;

            mz_sub(pij, pi, fluid->positions[j]);
            len = mz_length(pij);
            if ((len < support) && (len > 0.0000001)) {
                double spiky, a;
                const double k = parameters->repulsion_k;
                double q = parameters->repulsion_q;
                double s;
                double lamj = fluid->lambdas[j];

                s = eval_poly6(len, support) / eval_poly6(q, support);
                spiky = eval_spiky_grad(len, support);
                a = (lami + lamj - k * s * s * s * s) * spiky * invrest / len;
                dpos[0] += a * pij[0];
                dpos[1] += a * pij[1];
            }
        }
        memcpy(fluid->dpositions[i], dpos, sizeof(dpos));
    }
}

void mz_update_positions(struct mz_fluid *fluid) {
    int i;

    for (i = 0; i < fluid->num_particles; i++) {
        fluid->positions[i][0] += fluid->dpositions[i][0];
        fluid->positions[i][1] += fluid->dpositions[i][1];
    }
}

