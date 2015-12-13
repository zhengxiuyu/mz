#ifndef MZ_H
#define MZ_H

#include <stdbool.h>

enum {
    MZ_SUCCESS              = 0,
    MZ_OUT_OF_MEMORY,
    MZ_INVALID_ARGUMENTS
};

typedef struct {
    float min[2];
    float max[2];
    float extent[2];
} mz_domain;

typedef struct {
    float (*positions)[2];
    float (*velocities)[2];
    float *lambdas;                                     /* step sizes        */
    float (*dpositions)[2];                             /* positions updates */
    float *densities;
    float rest_density;
    int num_particles;
} mz_fluid;

/*
 * Acceleration structure for quickly finding particle neighbors.
 * TODO: Sorting fluid by their cell id instead of storing [ids] might
 *       increase overall memory coherency.
 * TODO: Using Z-ordering might increase memory coherency.
 */
typedef struct {
    mz_domain domain;
    float dx;
    int num_cells_total;
    int num_cells[2];
    int *num_particles;                 /* # fluid for each cell.             */
    int *start_ids;                     /* reference to the first particle id
                                           for each cell in [ids].            */
    int *ids;                           /* particle ids for referencing
                                           fluid within a cell                */
} mz_grid;

extern int mz_make_domain(
    mz_domain *domain,
    float xmin,
    float ymin,
    float xmax,
    float ymax
);

#define mz_grid_index_from_coord(grid, coord)                                  \
    (coord)[0] * (grid)->num_cells[0] + (coord)[1]

extern int mz_init_fluid(
    mz_fluid *fluid,
    float rest_density,
    unsigned int num_particles
);
extern void mz_deinit_fluid(mz_fluid *fluid);
extern int mz_copy_fluid(mz_fluid *dst, const mz_fluid *src);

extern int mz_init_grid(
    mz_grid *grid,
    const mz_fluid *fluid,
    const mz_domain *domain,
    float dx
);
extern int mz_update_grid(mz_grid *grid, const mz_fluid *fluid);
extern void mz_deinit_grid(mz_grid *grid);

/*
 * Computes the grid coordinate of the cell containing [position] and stores
 * it in [coord]. Returns false if the position is outside the grid, otherwise
 * true.
 */
extern bool mz_grid_coord_from_position(
    const mz_grid *grid,
    int coord[2],
    const float position[2]
);

/*
 * Computes the cell index for the cell containing [position] and stores it in
 * [index]. Returns false if the position is outside the grid, otherwise true.
 */
extern bool mz_grid_index_from_position(
    const mz_grid *grid,
    int *index,
    float position[2]
);

extern int mz_calc_lambdas(
    mz_fluid *fluid,
    const mz_grid *grid,
    float support
);
extern void mz_calc_lambdas_naive(mz_fluid *fluid, float support);

#endif /* end of include guard: PARTICLES_H */
