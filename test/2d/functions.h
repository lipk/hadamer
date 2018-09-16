#pragma once

#include <config.h>

UPSAMPLE_FUNC(upsample, double) {
	*y = *x1 + *x2 + *x3 + *x4;
}

DOWNSAMPLE_FUNC(downsample, double) {
	*y1 = *y2 = *y3 = *y4 = (*x) / 4;
}
