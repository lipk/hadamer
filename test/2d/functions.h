#pragma once

#include <config.h>
#include <stdio.h>

DOWNSAMPLE_FUNC(downsample, double) {
    *y = *x1 + *x2 + *x3 + *x4;
}

UPSAMPLE_FUNC(upsample, double) {
    *y1 = *y2 = *y3 = *y4 = (*x) / 4;
}

KERNEL_FUNC(blur, double) {
    *action = REFINE;
    return *CELL(data, mass, x1, x2);
}
