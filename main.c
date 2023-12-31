//
//  main.c
//  hltv-rank-analysis
//
//  Created by Noah Wooten on 11/21/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#define MS_SLEEP(X) Sleep(X)
#else
#include <unistd.h>
#define MS_SLEEP(X) usleep(X * 1000)
#endif

#include "netapi/netapi.h"
#include "hltva/hltva.h"
#include "csv/csv.h"

#define WEEKS_TO_GENERATE 450

int main(int argc, char** argv) {
    NetInit();
    HltvInit();
    CsvInit();
    
    for (int i = 0; i < WEEKS_TO_GENERATE; i++) {
        char* DateString = malloc(128);
        char* PrimBuffer = malloc(256);
        
        WeekToDateString(i, DateString, 128);
        sprintf(PrimBuffer, "https://www.hltv.org/ranking/teams/%s",
            DateString);
        free(DateString);
        
        HltvGenerateByUrl(PrimBuffer);
        free(PrimBuffer);
        
        usleep(500000);
        
    }
    
    // Generate Player Points
    WORD32 Players = CsvCreateTable("players.csv");
    WORD32 Header = CsvCreateEntry(Players, 0);
    CsvEntryAddMember(Players, Header, "Player Name");
    CsvEntryAddMember(Players, Header, "Total Points");
    CsvEntryAddMember(Players, Header, "Peak Rank");
    CsvEntryAddMember(Players, Header, "Weeks Present");
    CsvEntryAddMember(Players, Header, "Average Points (All Time)");
    CsvEntryAddMember(Players, Header, "Average Points (Present)");
    CsvEntryAddMember(Players, Header, "Time In Longest Team");
    CsvEntryAddMember(Players, Header, "Longest Team");
    CsvEntryAddMember(Players, Header, "Last Team");
    CsvEntryAddMember(Players, Header, "Time In Current Team");
    CsvEntryAddMember(Players, Header, "Team Count");
    
    for (int i = 0; i < RankStats->PlayerCount; i++) {
        WORD32 PlayerEntry = CsvCreateEntry(Players, i + 1);
        PSTATS_PLAYER ThisPlayer = &RankStats->Players[i];
        char* StringBuf = malloc(128);
        int RemovedTeams = 0;
        
        CsvEntryAddMember(Players, PlayerEntry, ThisPlayer->PlayerName);
                
        sprintf(StringBuf, "%lu", ThisPlayer->TotalPoints);
        CsvEntryAddMember(Players, PlayerEntry, StringBuf);
        *StringBuf = 0x00;
        
        sprintf(StringBuf, "%lu", ThisPlayer->PeakRank);
        CsvEntryAddMember(Players, PlayerEntry, StringBuf);
        *StringBuf = 0x00;
        
        sprintf(StringBuf, "%lu", ThisPlayer->WeeksPresent);
        CsvEntryAddMember(Players, PlayerEntry, StringBuf);
        *StringBuf = 0x00;
        
        ThisPlayer->AveragePoints[0] = ThisPlayer->TotalPoints /
            WEEKS_TO_GENERATE;
        ThisPlayer->AveragePoints[1] = ThisPlayer->TotalPoints /
            ThisPlayer->WeeksPresent;
        
        sprintf(StringBuf, "%lu", ThisPlayer->AveragePoints[0]);
        CsvEntryAddMember(Players, PlayerEntry, StringBuf);
        *StringBuf = 0x00;
        
        sprintf(StringBuf, "%lu", ThisPlayer->AveragePoints[1]);
        CsvEntryAddMember(Players, PlayerEntry, StringBuf);
        *StringBuf = 0x00;
        
        sprintf(StringBuf, "%lu", ThisPlayer->TimeInLongestTeam);
        CsvEntryAddMember(Players, PlayerEntry, StringBuf);
        *StringBuf = 0x00;
        
        sprintf(StringBuf, "%s", ThisPlayer->LongestTeam->TeamName);
        CsvEntryAddMember(Players, PlayerEntry, StringBuf);
        *StringBuf = 0x00;
        
        sprintf(StringBuf, "%s", ThisPlayer->CurrentTeam->TeamName);
        CsvEntryAddMember(Players, PlayerEntry, StringBuf);
        *StringBuf = 0x00;
        
        sprintf(StringBuf, "%lu", ThisPlayer->TimeInCurrentTeam);
        CsvEntryAddMember(Players, PlayerEntry, StringBuf);
        *StringBuf = 0x00;
        
        sprintf(StringBuf, "%lu", ThisPlayer->TeamCount - RemovedTeams);
        CsvEntryAddMember(Players, PlayerEntry, StringBuf);
        *StringBuf = 0x00;
        
        free(StringBuf);
    }
    
    CsvGenerate(Players);
    
    WORD32 Teams = CsvCreateTable("teams.csv");
    WORD32 TeamHeader = CsvCreateEntry(Teams, 1);
    CsvEntryAddMember(Teams, TeamHeader, "Team Name");
    CsvEntryAddMember(Teams, TeamHeader, "Peak Rank");
    CsvEntryAddMember(Teams, TeamHeader, "Total Points");
    CsvEntryAddMember(Teams, TeamHeader, "Time at Peak");
    CsvEntryAddMember(Teams, TeamHeader, "Player Count");
    for (int i = 0; i < RankStats->TeamCount; i++) {
        PSTATS_TEAM ThisTeam = &RankStats->Teams[i];
        WORD32 ThisLine = CsvCreateEntry(Teams, i + 1);
        char* Buffer = malloc(128);
        
        sprintf(Buffer, "%s", ThisTeam->TeamName);
        CsvEntryAddMember(Teams, ThisLine, Buffer);
        
        sprintf(Buffer, "%lu", ThisTeam->PeakRank);
        CsvEntryAddMember(Teams, ThisLine, Buffer);
        
        sprintf(Buffer, "%lu", ThisTeam->TotalPoints);
        CsvEntryAddMember(Teams, ThisLine, Buffer);
        
        sprintf(Buffer, "%lu", ThisTeam->TimeAtPeak);
        CsvEntryAddMember(Teams, ThisLine, Buffer);
        
        sprintf(Buffer, "%lu", ThisTeam->PlayerCount);
        CsvEntryAddMember(Teams, ThisLine, Buffer);
        
        free(Buffer);
    }
    
    CsvGenerate(Teams);
    
    NetShutdown();
    HltvShutdown();
    CsvShutdown();
    return 0;
}
