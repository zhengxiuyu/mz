#include <mz.h>
#include "common.h"

#define EPS 0.01

#define is_valid_coord(grid, coord)                                           \
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
    float h5 = h * h * h * h * h;       /* numerical error if h is small? */
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
                int index = mz_grid_index_from_coord(grid, cc);

                if (!is_valid_coord(grid, cc))
                    continue;
                calc_quantities_cell(fluid, i, grid, index, &nom,
                    &denoma, denomb, fluid->positions[i], support);
            }
        }
        fluid->densities[i] = nom;
        nom = nom / restdens - 1.0;
        fluid->lambdas[i] = -nom / (denoma + mz_dot(denomb, denomb) + EPS) * rho2;
    }
    return MZ_SUCCESS;
}

void mz_calc_lambdas_naive(mz_fluid *fluid, float support) {
    int i, j;
    float invrest = 1.0 / fluid->rest_density;

    for (i = 0; i < fluid->num_particles; i++) {
        float pi[2], dens = 0.0;
        float a = 0.0, b[2];

        memcpy(pi, fluid->positions[i], sizeof(float[2]));
        memset(b, 0, sizeof(float[2]));
        for (j = 0; j < fluid->num_particles; j++) {
            float pij[2], len;

            mz_sub(pij, pi, fluid->positions[j]);
            len = mz_dot(pij, pij);
            if (len < support * support) {
                len = sqrtf(len);
                dens += eval_poly6(len, support);

                if (i != j) {
                    float spiky = eval_spiky_grad(len, support);
                    float grad[2];

                    grad[0] = invrest * spiky * pij[0];
                    grad[1] = invrest * spiky * pij[1];
                    a += mz_dot(grad, grad);
                    b[0] += grad[0];
                    b[1] += grad[1];
                }
            }
        }

        fluid->densities[i] = dens;
        fluid->lambdas[i] = -(dens * invrest - 1.0) /  (a + mz_dot(b, b) + EPS);
    }
}

static void update_dposition_cell(
    mz_fluid *fluid,
    int particle_index,
    const mz_grid *grid,
    int cell_index,
    float dposition[2],
    const float position[2],
    float support
) {
    int i = grid->start_ids[cell_index];
    int imax = i + grid->num_particles[cell_index];

    for (; i < imax; i++) {
        int k = grid->ids[i];
        float diff[2], dot, r, spiky, tmp;

        mz_sub(diff, position, fluid->positions[k]);
        dot = mz_dot(diff, diff);
        if (dot > support * support || k == particle_index)
            continue;
        r = sqrtf(dot);
        spiky = eval_spiky_grad(r, support);
        tmp = (fluid->lambdas[particle_index] + fluid->lambdas[k]) * spiky;
        dposition[0] += tmp * diff[0];
        dposition[1] += tmp * diff[1];
    }
}

int mz_calc_dpositions(
    mz_fluid *fluid,
    const mz_grid *grid,
    float support
) {
    int cp[2], cc[2];              /* grid coordinates for particle and cell */
    int pidx;                      /* particle index */

    if (support != grid->dx) {
        mz_debug("Expected support to equal grid spacing");
        return MZ_INVALID_ARGUMENTS;
    }

    for (pidx = 0; pidx < fluid->num_particles; pidx++) {
        float pos[2];
        float dpos[2];

        memcpy(pos, fluid->positions[pidx], sizeof(float[2]));
        memset(dpos, 0, sizeof(float[2]));
        mz_grid_coord_from_position(grid, cp, pos);
        for (cc[0] = cp[0] - 1; cc[0] <= cp[0] + 1; cc[0]++) {
            for (cc[1] = cp[1] - 1; cc[1] <= cp[1] + 1; cc[1]++) {
                int cidx = mz_grid_index_from_coord(grid, cc);

                if (!is_valid_coord(grid, cc))
                    continue;
                update_dposition_cell(fluid, pidx, grid, cidx, dpos,
                    pos, support);
            }
        }
        dpos[0] /= fluid->rest_density;
        dpos[1] /= fluid->rest_density;
        memcpy(fluid->dpositions[pidx], dpos, sizeof(float[2]));
    }
    return MZ_SUCCESS;
}

void mz_calc_dpositions_naive(mz_fluid *fluid, float support) {
    int i, j;
    float invrest = 1.0 / fluid->rest_density;

    for (i = 0; i < fluid->num_particles; i++) {
        float dpos[2], pi[2], lami;

        lami = fluid->lambdas[i];
        memset(dpos, 0, sizeof(float[2]));
        memcpy(pi, fluid->positions[i], sizeof(float[2]));
        for (j = 0; j < fluid->num_particles; j++) {
            float pij[2], len;

            mz_sub(pij, pi, fluid->positions[j]);
            len = mz_dot(pij, pij);
            if ((len < support * support) && (i != j)) {
                float spiky, a;
                const float k = 0.001;
                float q = 0.0 * support;
                float s = eval_poly6(len, support) / eval_poly6(q, support);

                len = sqrtf(len);
                spiky = eval_spiky_grad(len, support);
                a = invrest * (lami + fluid->lambdas[j] - k * s * s * s * s) * spiky;
                dpos[0] += a * pij[0];
                dpos[1] += a * pij[1];
            }
        }
        memcpy(fluid->dpositions[i], dpos, sizeof(float[2]));
    }
}

void mz_update_positions(mz_fluid *fluid)
{
    int i;

    for (i = 0; i < fluid->num_particles; i++) {
        fluid->positions[i][0] += fluid->dpositions[i][0];
        fluid->positions[i][1] += fluid->dpositions[i][1];
    }
}

