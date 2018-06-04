#ifndef RAND_ALG_H
#define RAND_ALG_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif
//------------------------------------------------------------------------------
typedef  struct {
	int count;
	int delta;
	int rand_min;
} RandAlgNode;
//------------------------------------------------------------------------------
typedef struct {
	RandAlgNode *node;
	int node_sz;
	int min;
	int max;
	int curr;
	int count;
} RandAlg;
//------------------------------------------------------------------------------
void RandAlg_InitDev(RandAlg *ra);
int RandAlg_Step(RandAlg *ra, int *Id);
void RandAlg_SetMinMax(RandAlg *ra,int min,int max);
//------------------------------------------------------------------------------
void RandAlg_TestInit();
void RandAlg_TestLoop();
//------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif
