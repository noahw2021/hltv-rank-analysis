//
//  utils.c
//  hltv-rank-analysis
//
//  Created by Noah Wooten on 11/21/23.
//

#include "basetypes.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int DaysInMonth[12] = {
    31, 28, 31, 30, 31, 30,
    31, 31, 30, 31, 30, 31
};

const char* Months[] = {
    "january", "february", "march",
    "april", "may", "june", "july",
    "august", "september", "october",
    "november", "december"
};

BYTE IsYearLeapYear(int Year) {
    /*
     Year must be divisible by four.
     Year mustn't be divisible by 100, unless
        also divisible by 400.
     */
    
    if ((Year % 4) != 0)
        return 0;
    
    if ((Year % 100) == 0) {
        if ((Year % 400) == 0)
            return 1;
        else
            return 0;
    }
    
    return 1;
}

void WeekToDateString(int WeekNum, char* DateStringOut, int BfrSize) {
    int BaseMonth, BaseDay, BaseYear;
    
    BaseMonth = 11; // December
    BaseDay = 13; // 14th
    BaseYear = 2015; // 2015
    
    for (int i = 0; i < WeekNum; i++) {
        int ThisYearDays[12];
        memcpy(&ThisYearDays, &DaysInMonth, sizeof(int) * 12);
        if (IsYearLeapYear(BaseYear))
            ThisYearDays[1]++;
        
        BaseDay += 7;
        if (BaseDay >= ThisYearDays[BaseMonth])
            BaseDay %= ThisYearDays[BaseMonth++];
        
        if (BaseMonth > 11) {
            BaseYear++;
            BaseMonth = 0;
        }
    }
    
    char* Buffer = malloc(512);
    sprintf(Buffer, "%04i/%s/%i", BaseYear,
        Months[BaseMonth], BaseDay + 1);
    
    if ((strlen(Buffer) + 1) <= BfrSize)
        strcpy(DateStringOut, Buffer);
    
    free(Buffer);
    return;
}
