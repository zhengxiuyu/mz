#include <mz.h>
#include "common.h"

#define EPS 0.0001

#define is_valid_coord(grid, coord)                                            \
    (mz_inrange((coord)[0], 0, (grid)->num_cells[0] - 1) &&                   \
        mz_inrange((coord)[1], 0, (grid)->num_cells[1] - 1))

static float eval_poly6(float r, float h) {
    const float c = 4.0 / M_PI;
    float h2 = h * h;
    float h8 = h2 * h2 * h2 * h2;
    float a = h2 - r * r;
    float b = c / h8;
    return b * a * a * a;
}

static float eval_spiky_grad(float r, float h) {
    const float c = -30.0 / M_PI;
    float h5 = h * h * h * h * h;
    float b = h - r;
    return c / (h5 * r) * b * b;
}

static void calc_quantities_cell(
    mz_fluid *fluid,
    int particle_index,
    const mz_grid *grid,
    int cell_index,
    float *dens,
    float *grad_a,
    float grad_b[2],
    const float position[2],
    float support
) {
    int i = grid->start_ids[cell_index];
    int imax = i + grid->num_particles[cell_index];

    for (; i < imax; i++) {
        int k = grid->ids[i];
        float dot, diff[2], r, w;

        mz_sub(diff, position, fluid->positions[k]);
        dot = mz_dot(diff, diff);
        if (dot > support * support)
            continue;

        r = sqrtf(dot);
        w = eval_poly6(r, support);
        *dens += w;

        if (k != particle_index) {
            float spiky = eval_spiky_grad(r, support);
            float grad[2];
            grad[0] = spiky * diff[0];
            grad[1] = spiky * diff[1];
            grad_b[0] += grad[0];
            grad_b[1] += grad[1];
            *grad_a = mz_dot(grad, grad);
        }
    }
}

int mz_calc_lambdas(
    mz_fluid *fluid,
    const mz_grid *grid,
    float support
) {
    int i = 0;
    int cp[2], cc[2];              /* grid coordinates for particle and cell */
    int index;                     /* particle index                         */
    float restdens = fluid->rest_density;
    float rho2 = restdens * restdens;

    if (support != grid->dx) {
        mz_debug("Expected support to equal grid spacing");
        return MZ_INVALID_ARGUMENTS;
    }

    for (i = 0; i < fluid->num_particles; i++) {
        float nom = 0.0;                        /* density for particle i    */
        float denoma = 0.0, denomb[2];

        memset(denomb, 0, sizeof(float[2]));
        mz_grid_coord_from_position(grid, cp, fluid->positions[i]);
        for (cc[0] = cp[0] - 1; cc[0] <= cp[0] + 1; cc[0]++) {
            for (cc[1] = cp[1] - 1; cc[1] <= cp[1] + 1; cc[1]++) {
                index = mz_grid_index_from_coord(grid, cc);
                if (!is_valid_coord(grid, cc))
                    continue;
                calc_quantities_cell(fluid, i, grid, index, &nom,
                    &denoma, denomb, fluid->positions[i], support);
            }
        }
        fluid->densities[i] = nom;
        nom = nom / restdens - 1.0;
        fluid->lambdas[i] = nom / (denoma + mz_dot(denomb, denomb)) * rho2;
    }
    return MZ_SUCCESS;
}

void mz_calc_lambdas_naive(mz_fluid *fluid, float support) {
    int i = 0, j = 0;

    for (i = 0; i < fluid->num_particles; i++) {
        float dens = 0;

        for (j = 0; j < fluid->num_particles; j++) {
            float diff[2], dot;

            mz_sub(diff, fluid->positions[i], fluid->positions[j]);
            dot = mz_dot(diff, diff);
            if (dot <= support * support) {
                float r = sqrtf(dot);
                float w = eval_poly6(r, support);

                dens += w;
            }
        }
        fluid->densities[i] = dens;
    }
}

