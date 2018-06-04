#include "rand_alg.h"
#include "def.h"
//------------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
//------------------------------------------------------------------------------
void RandAlg_InitDev(RandAlg *ra)
{
	ra->curr = 0;
	ra->count = 0;
}
//------------------------------------------------------------------------------
void RandAlg_SetMinMax(RandAlg *ra, int min, int max)
{
	ra->min = min;
	ra->max = max;

	ra->node[0].rand_min = min;
	ra->node[0].delta = max - min;
}
//------------------------------------------------------------------------------
int RandAlg_GetRandInt(int low, int high)
{
	return rand() % ((high + 1) - low) + low;
}
//------------------------------------------------------------------------------
int RangAlg_Find_Id(RandAlg *ra)
{
	int count = ra->node[0].count;
	int range_id = -1;
	for(int id = 0; id < ra->node_sz; id++) {
		count += ra->node[id].count;
		if(ra->count < count) {
			range_id = id;
			break;
		}
	}
	return range_id;
}
//------------------------------------------------------------------------------
int RandAlg_Step(RandAlg *ra,int *Id)
{
	int id = RangAlg_Find_Id(ra);

	if(Id != NULL) {
		*Id = id;
	}

	int rand_min = ra->node[id].rand_min;
	int rand_max = rand_min + ra->node[id].delta;
	int rand = RandAlg_GetRandInt(rand_min,rand_max);

	ra->curr += rand;

	while( ra->curr > ra->max) {
		ra->curr = (ra->curr - ra->max) + ra->min;
	}

	ra->count++;
	return ra->curr;
}
//------------------------------------------------------------------------------
#if 0
//------------------------------------------------------------------------------
static RandAlgNode RandAlgNode_Init[] = {
	{	.count		= 1,
		.rand_min	= 5,
		.delta		= 95,
	},{
		.count		= 30,
		.rand_min	= 10,
		.delta		= 8,
	},{
		.count		= 30,
		.rand_min	= 7,
		.delta		= 6,
	},{
		.count		= 30,
		.rand_min	= 6,
		.delta		= 2,
	},{
		.count		= 15,
		.rand_min	= 5,
		.delta		= 2,
	},{
		.count		= 7,
		.rand_min	= 4,
		.delta		= 2,
	},{
		.count		= 7,
		.rand_min	= 3,
		.delta		= 2,
	},{
		.count		= 7,
		.rand_min	= 1,
		.delta		= 0,
	}
};
//------------------------------------------------------------------------------
static RandAlg RandAlg_init = {
	.node = RandAlgNode_Init,
	.node_sz = ARRAY_SIZE(RandAlgNode_Init),
	.min = 5,
	.max = 100,
};
//------------------------------------------------------------------------------
void RandAlg_TestInit()
{
	RandAlg_Init(&RandAlg_Init);
}
//------------------------------------------------------------------------------
void RandAlg_TestLoop()
{
	int id = 0;
	while(id >= 0) {
		int rand = RandAlg_Step(&RandAlg_Init,&id);
		printf("Rand:%i id:%i\n",rand,id);
	}
}
//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------
