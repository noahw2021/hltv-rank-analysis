//
//  hltva_init.c
//  hltv-rank-analysis
//
//  Created by Noah Wooten on 11/21/23.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hltva.h"

PSTATS_CTX RankStats;

void HltvInit(void) {
    RankStats = malloc(sizeof(STATS_CTX));
    memset(RankStats, 0, sizeof(STATS_CTX));

    return;
    
}

void HltvShutdown(void) {
    if (!RankStats)
        return;
    
    
}
