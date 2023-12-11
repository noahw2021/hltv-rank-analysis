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

void WeekToDateString(int _WeekNum, char* DateStringOut, int BfrSize) {
    
    if (_WeekNum > 14) {
        int WeekNum = _WeekNum - 15;
        int BaseMonth, BaseDay, BaseYear;
        
        BaseMonth = 0; // January
        BaseDay = 10; // 11th
        BaseYear = 2016; // 2016
        
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
    } else {
        switch (_WeekNum) {
            case 0:
                strcpy(DateStringOut, "2015/october/1");
                break;
            case 1:
                strcpy(DateStringOut, "2015/october/5");
                break;
            case 2:
                strcpy(DateStringOut, "2015/october/12");
                break;
            case 3:
                strcpy(DateStringOut, "2015/october/19");
                break;
            case 4:
                strcpy(DateStringOut, "2015/october/26");
                break;
            case 5:
                strcpy(DateStringOut, "2015/november/3");
                break;
            case 6:
                strcpy(DateStringOut, "2015/november/9");
                break;
            case 7:
                strcpy(DateStringOut, "2015/november/16");
                break;
            case 8:
                strcpy(DateStringOut, "2015/november/24");
                break;
            case 9:
                strcpy(DateStringOut, "2015/december/1");
                break;
            case 10:
                strcpy(DateStringOut, "2015/december/8");
                break;
            case 11:
                strcpy(DateStringOut, "2015/december/14");
                break;
            case 12:
                strcpy(DateStringOut, "2015/december/21");
                break;
            case 13:
                strcpy(DateStringOut, "2015/december/28");
                break;
            case 14:
                strcpy(DateStringOut, "2016/january/5");
                break;
        }
    }
}
void WeekToDateStringEx(int WeekNum, char* DateStringOut, int BfrSize, int Style) {
    if (WeekNum > 14) {
        if (Style == WTDS_STYLE_HLTV) {
            WeekToDateString(WeekNum, DateStringOut, BfrSize);
            return;
        }
        
        int BaseMonth, BaseDay, BaseYear;
        
        BaseMonth = 0; // January
        BaseDay = 10; // 11th
        BaseYear = 2016; // 2016
        
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
        sprintf(Buffer, "%i/%i/%i",
                BaseMonth, BaseDay, BaseYear);
        
        if ((strlen(Buffer) + 1) <= BfrSize)
            strcpy(DateStringOut, Buffer);
        
        free(Buffer);
        return;
    }
}
