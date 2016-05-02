//vv_clksrc.h
#ifndef __VV_CLKSRC_H
#define __VV_CLKSRC_H

#include "includes.h"

typedef enum {
	CLK_SRC_OSC_TO_PLL_AND_SYNC = 1,
	CLK_SRC_OSC_TO_PLL_ONLY,
	CLK_SRC_SYNC_TO_PLL
} CLK_SRC;


void vv_clksrc_sel(CLK_SRC value);

#endif
