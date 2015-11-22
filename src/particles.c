#include <stdlib.h>
#include <math.h>
#include <mz.h>
#include "particles.h"
#include "common.h"

static float eval_w_poly6(float r, float h);
static void update_constraints(mz_particles *particles, float rest_density);

mz_particles *mz_particles_new(unsigned int num_particles)
{
    mz_particles *particles = calloc(1, sizeof(*particles));

    particles->positions = calloc(num_particles,
        sizeof(*particles->positions));
    checkmem(particles->positions);
    particles->velocities = calloc(num_particles,
        sizeof(*particles->velocities));
    checkmem(particles->velocities);
    particles->constraints = calloc(num_particles,
        sizeof(*particles->constraints));
    checkmem(particles->constraints);
    particles->constraint_gradients = calloc(num_particles,
        sizeof(*particles->constraint_gradients));
    checkmem(particles->constraint_gradients);
    return particles;
error:
    mz_particles_delete(particles);
    return NULL;
}

void mz_particles_delete(mz_particles *particles)
{
    if (!particles) return;
    free(particles->positions);
    free(particles->normals);
    free(particles->constraints);
    free(particles->constraint_gradients);
    free(particles);
}

float eval_w_poly6(float r, float h)
{
    const float c = 4.0f / M_PI;
    float a, h2, h4, h8;

    if (r > h) return 0.0f;
    h2 = h * h;
    h4 = h2 * h2;
    h8 = h4 * h4;
    a = h2 - r * r;
    return c * a * a * a / h8;
}

void update_constraints(mz_particles *particles, float rest_density)
{

}

