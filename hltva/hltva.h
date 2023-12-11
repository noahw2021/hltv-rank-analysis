//
//  hltva.h
//  hltv-rank-analysis
//
//  Created by Noah Wooten on 11/21/23.
//

#ifndef hltva_h
#define hltva_h

#include "../basetypes.h"

typedef struct _STATS_PLAYER *PSTATS_PLAYER;

typedef struct _STATS_TEAM {
    char TeamName[768];
    
    WORD32 TotalPoints;
    WORD32 PeakRank;
    WORD32 TimeAtPeak;
    
    PSTATS_PLAYER* Players;
    WORD32 PlayerCount;
}STATS_TEAM, *PSTATS_TEAM;

typedef struct _STATS_PLAYER {
    char PlayerName[128];
    
    WORD32 TotalPoints;
    WORD32 PeakRank;
    WORD32 WeeksPresent;
    // AveragePoints[0] = 
    WORD32 AveragePoints[2];
    WORD32 TimeInLongestTeam;
    PSTATS_TEAM LongestTeam;
    PSTATS_TEAM CurrentTeam;
    WORD32 TimeInCurrentTeam;
    
    PSTATS_TEAM* Teams;
    WORD32 TeamCount;
}STATS_PLAYER, *PSTATS_PLAYER;

typedef struct _HLTV_RANK_PAGE {
    char* HtmlData;
    
    WORD32 TeamCount;
    PSTATS_TEAM* Teams;
    
    WORD32 PlayerCount;
    PSTATS_PLAYER* Players;
}HLTV_RANK_PAGE,
*PHLTV_RANK_PAGE;

typedef struct _STATS_CTX {
    PHLTV_RANK_PAGE RankPages;
    WORD32 RankPageCount;
    
    PSTATS_TEAM Teams;
    PSTATS_PLAYER Players;
    WORD32 TeamCount;
    WORD32 PlayerCount;
}STATS_CTX, *PSTATS_CTX;
extern PSTATS_CTX RankStats;

void HltvInit(void);
void HltvShutdown(void);

void HltvGenerateByUrl(char* Url);

#endif /* hltva_h */
