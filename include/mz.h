#ifndef MZ_H
#define MZ_H

#include <stdbool.h>

enum {
    MZ_SUCCESS              = 0,
    MZ_OUT_OF_MEMORY,
    MZ_INVALID_ARGUMENTS
};

struct mz_parameters {
    double support;
    double relaxation;
    double repulsion_k;
    double repulsion_q;
    double dpos_atten;
};

struct mz_fluid {
    double (*positions)[2];
    double (*positions_tmp)[2];
    double (*velocities)[2];
    double *lambdas;                                     /* step sizes         */
    double (*dpositions)[2];                             /* positions updates  */
    double *densities;
    double rest_density;
    int num_particles;
};

extern struct mz_parameters mz_parameters();
extern int mz_init_fluid(
    struct mz_fluid *fluid,
    double rest_density,
    unsigned int num_particles
);
extern void mz_deinit_fluid(struct mz_fluid *fluid);
extern void mz_calc_lambdas_naive(
    struct mz_fluid *fluid,
    const struct mz_parameters *parameters
);
extern void mz_calc_dpositions_naive(
    struct mz_fluid *fluid,
    const struct mz_parameters *parameters
);
extern void mz_update_positions(
    struct mz_fluid *fluid,
    const struct mz_parameters *parameters
);
extern void mz_apply_gravity(
    struct mz_fluid *fluid,
    double gravity,
    double dt
);

#endif /* end of include guard: PARTICLES_H */
