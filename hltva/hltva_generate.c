//
//  hltva_generate.c
//  hltv-rank-analysis
//
//  Created by Noah Wooten on 11/21/23.
//

#include "hltva.h"
#include "../netapi/netapi.h"
#include <stdlib.h>
#include <string.h>

void HltvGenerateByUrl(char* Url) {
    if (!RankStats)
        return;
    
    char* PageBuffer = malloc(300000);
    WORD32 PageBufferSize;
    
    NetDownload(Url, PageBuffer, 300000, &PageBufferSize);
    if (PageBufferSize > 300000) {
        free(PageBuffer);
        PageBuffer = malloc(PageBufferSize);
        NetDownload(Url, PageBuffer, PageBufferSize,
                    &PageBufferSize);
    }
    
    if (!RankStats->RankPages) {
        RankStats->RankPages = malloc(sizeof(HLTV_RANK_PAGE));
    } else {
        RankStats->RankPages = realloc(RankStats->RankPages,
            sizeof(HLTV_RANK_PAGE) *
            (RankStats->RankPageCount + 1));
    }
    
    PHLTV_RANK_PAGE NewPage = &RankStats->RankPages
        [RankStats->RankPageCount++];
    
    NewPage->HtmlData = malloc(PageBufferSize + 3072000);
    strcpy(NewPage->HtmlData, PageBuffer);
    free(PageBuffer);
    
    char* ThisHeader = NewPage->HtmlData;
    int Escape = 0;
    
    do {
        ThisHeader = strstr(ThisHeader,
            "\"ranking-header\"") + strlen("\"ranking-header\"");
        if (!ThisHeader) {
            Escape++;
            if (Escape > 15)
                break;
            continue;
        }
        
        char RankingNumber[3] = "00";
        ThisHeader += strlen("><span class=\"position\">#");
        
        if (ThisHeader < 0x100) {
            Escape++;
            if (Escape > 15)
                break;
            continue;
        }
        
        memcpy(RankingNumber, ThisHeader, 2);
        RankingNumber[2] = 0x00;
        if (RankingNumber[1] == '<') {
            RankingNumber[1] = RankingNumber[0];
            RankingNumber[0] = '0';
        }
        
        int ThisRanking = atoi(RankingNumber);
        
        ThisHeader = strstr(ThisHeader, "<span class=\"name\">");
        ThisHeader += strlen("<span class=\"name\">");
        
        int i = 0;
        char TeamName[128];
        while (ThisHeader[i] != '<') {
            TeamName[i] = ThisHeader[i];
            if (i > 100)
                break;
            i++;
        }
        TeamName[i] = 0x00;
        
        if (TeamName[0] == 0x00)
            printf("[ERROR]!\n");
        
        i = 0;
        char Points[5] = {0, 0, 0, 0, 0};
        ThisHeader = strstr(ThisHeader, "<span class=\"points\">(") +
            strlen("<span class=\"points\">(");
        while (ThisHeader[i] != ' ') {
            Points[i] = ThisHeader[i];
            i++;
        }
        int PointsInt = atoi(Points);
        
        //printf("#%i: %s (%i points)\n", ThisRanking, TeamName, PointsInt);
        
        PSTATS_TEAM ResolvedTeam = NULL;
        for (int i = 0; i < RankStats->TeamCount; i++) {
            PSTATS_TEAM ThisTeam = &RankStats->Teams[i];
            
            if (ThisTeam->TeamName[0] == TeamName[0] &&
                ThisTeam->TeamName[1] == TeamName[1] &&
                strlen(ThisTeam->TeamName) == strlen(TeamName)
            ) {
                ResolvedTeam = ThisTeam;
                break;
            }
        }
        
        if (!ResolvedTeam) {
            if (!RankStats->Teams)
                RankStats->Teams = malloc(sizeof(STATS_TEAM));
            else
                RankStats->Teams = realloc(RankStats->Teams,
                    (sizeof(STATS_TEAM) * (RankStats->TeamCount + 1)));
            
            printf("Creating team: %s\n", TeamName);
            
            ResolvedTeam = &RankStats->Teams[RankStats->TeamCount++];
            memset(ResolvedTeam, 0, sizeof(STATS_TEAM));
            ResolvedTeam->PeakRank = 31;
            strcpy(ResolvedTeam->TeamName, TeamName);
        }
        
        ResolvedTeam->TotalPoints += PointsInt;
        if (ResolvedTeam->PeakRank > ThisRanking)
            ResolvedTeam->PeakRank = ThisRanking;
        
        if (ResolvedTeam->PeakRank == ThisRanking)
            ResolvedTeam->TimeAtPeak++;
            
        int PlayerCountInTeam = 0;
        PSTATS_PLAYER PlayersInTeam[5];
        for (int i = 0; i < 5; i++)
            PlayersInTeam[i] = NULL;
        while (strstr(ThisHeader, "<div class=\"rankingNicknames\"><span>")) {
            char* NextPlayer = strstr(ThisHeader, "<div class=\"rankingNicknames\"><span>");
            char* NextTeam = strstr(ThisHeader,
                                    "\"ranking-header\"");
            if (NextTeam)
                NextTeam += strlen("\"ranking-header\"");
            if (NextTeam && NextPlayer > NextTeam)
                break;
            if (!NextTeam || NextTeam[0] == '\20' || !NextTeam[0])
                break;
            
            ThisHeader = strstr(ThisHeader, "<div class=\"rankingNicknames\"><span>");
            ThisHeader += strlen("<div class=\"rankingNicknames\"><span>");
            
            char* Player = malloc(128);
            strncpy(Player, ThisHeader, 128);
            if (strstr(Player, "</span>"))
                strstr(Player, "</span>")[0] = 0x00;
            
            //printf("\t%s\n", Player);
            
            PSTATS_PLAYER ResolvedPlayer = NULL;
            for (int i = 0; i < RankStats->PlayerCount; i++) {
                PSTATS_PLAYER ThisPlayer = &RankStats->Players[i];
                
                if (!strcmp(ThisPlayer->PlayerName, Player)) {
                    ResolvedPlayer = ThisPlayer;
                    break;
                }
            }
            
            if (!ResolvedPlayer) {
                if (!RankStats->Players) {
                    RankStats->Players = malloc(sizeof(STATS_PLAYER));
                } else {
                    RankStats->Players = realloc(RankStats->Players,
                        sizeof(STATS_PLAYER) *
                        (RankStats->PlayerCount + 1));
                }
                
                ResolvedPlayer = &RankStats->Players
                    [RankStats->PlayerCount++];
                memset(ResolvedPlayer, 0, sizeof(STATS_PLAYER));
                ResolvedPlayer->PeakRank = 31;
                ResolvedPlayer->LongestTeam = NULL;
                
                strncpy(ResolvedPlayer->PlayerName, Player, 128);
            }
            
            PlayersInTeam[PlayerCountInTeam] = ResolvedPlayer;
            PlayerCountInTeam++;
            
            BYTE TeamNeedsAdding = 1;
            for (int i = 0; i < ResolvedPlayer->TeamCount; i++) {
                if (ResolvedPlayer->Teams[i]->TeamName[0] == TeamName[0] &&
                    ResolvedPlayer->Teams[i]->TeamName[1] == TeamName[1] &&
                     strlen(ResolvedPlayer->Teams[i]->TeamName) == strlen(TeamName)
                ) {
                    TeamNeedsAdding = 0;
                    break;
                }
            }
            
            if (TeamNeedsAdding) {
                if (!ResolvedPlayer->Teams) {
                    ResolvedPlayer->Teams = malloc(sizeof(PSTATS_TEAM));
                } else {
                    ResolvedPlayer->Teams = realloc(ResolvedPlayer->Teams,
                        sizeof(PSTATS_TEAM) * (ResolvedPlayer->TeamCount + 1));
                }
                
                ResolvedPlayer->Teams[ResolvedPlayer->TeamCount++] =
                    ResolvedTeam;
                ResolvedPlayer->TimeInCurrentTeam = 1;
                if (!ResolvedPlayer->LongestTeam)
                    ResolvedPlayer->LongestTeam = ResolvedTeam;
            }
            
            BYTE PlayersNeedsAdding = 1;
            for (int i = 0; i < ResolvedTeam->PlayerCount; i++) {
                if (ResolvedTeam->Players[i] == ResolvedPlayer) {
                    PlayersNeedsAdding = 0;
                    break;
                }
            }
            
            if (PlayersNeedsAdding) {
                if (!ResolvedTeam->Players) {
                    ResolvedTeam->Players = malloc(sizeof(PSTATS_PLAYER));
                } else {
                    ResolvedTeam->Players = realloc(ResolvedTeam->Players,
                        sizeof(PSTATS_PLAYER) * (ResolvedTeam->PlayerCount + 1));
                }
                
                ResolvedTeam->Players[ResolvedTeam->PlayerCount] =
                    ResolvedPlayer;
                ResolvedTeam->PlayerCount++;
            }
            
            if (ResolvedTeam->PeakRank < ResolvedPlayer->PeakRank)
                ResolvedPlayer->PeakRank = ResolvedTeam->PeakRank;
            
            ResolvedPlayer->WeeksPresent++;
            if (ResolvedPlayer->CurrentTeam != ResolvedTeam)
                ResolvedPlayer->TimeInCurrentTeam = 0;
            ResolvedPlayer->TimeInCurrentTeam++;
            ResolvedPlayer->CurrentTeam = ResolvedTeam;
            
            if (ResolvedPlayer->TimeInCurrentTeam >=
                ResolvedPlayer->TimeInLongestTeam &&
                ResolvedPlayer->LongestTeam != ResolvedPlayer->CurrentTeam
            ) {
                ResolvedPlayer->LongestTeam = ResolvedTeam;
                ResolvedPlayer->TimeInLongestTeam =
                    ResolvedPlayer->TimeInCurrentTeam;
            }
            
            if (ResolvedPlayer->LongestTeam == ResolvedPlayer->CurrentTeam)
                ResolvedPlayer->TimeInLongestTeam = ResolvedPlayer->TimeInCurrentTeam;	
            
            free(Player);
        }
        
        for (int i = 0; i < PlayerCountInTeam; i++) {
            PSTATS_PLAYER ThisPlayer = PlayersInTeam[i];
            if (!ThisPlayer)
                continue;
            
            ThisPlayer->TotalPoints += PointsInt / PlayerCountInTeam;
        }
        
        continue;
    } while ((ThisHeader > 0x100) && strstr(ThisHeader, "\"ranking-header\""));
    
    free(NewPage->HtmlData);
    return;
}
