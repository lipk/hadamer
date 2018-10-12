#pragma once

#include <stdint.h>
#include <config.h>
#include <buffer.h>

typedef struct RefinePlan
{
    uint32_t level;
	uint64_t pos[DIM];
	uint64_t size[DIM];
	Array *nodes;
	bool propagate;
} RefinePlan;

RefinePlan *RefinePlan_mergeIfPossible(RefinePlan *plan1, uint32_t index1,
                                       RefinePlan *plan2, uint32_t index2,
                                       uint32_t currentLevel);

void RefinePlan_destroy(RefinePlan *plan);
