#include <mz.h>
#include "common.h"

int mz_make_domain(mz_domain *domain, float xmin, float ymin,
                   float xmax, float ymax)
{
    if (!domain || xmin > xmax || ymin > ymax) {
        mz_debug("Invalid domain arguments\n");
        return MZ_INVALID_ARGUMENTS;
    }
    mz_set(domain->min, xmin, ymin);
    mz_set(domain->max, xmax, ymax);
    mz_sub(domain->extent, domain->max, domain->min);
    return MZ_SUCCESS;
}


