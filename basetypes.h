//
//  basetypes.h
//  hltv-rank-analysis
//
//  Created by Noah Wooten on 11/21/23.
//

#ifndef basetypes_h
#define basetypes_h

typedef unsigned long long WORD64, *PWORD64;
typedef unsigned long WORD32, *PWORD32;
typedef unsigned short WORD16, *PWORD16;
typedef unsigned char BYTE, *PBYTE;

#define WTDS_STYLE_HLTV 0x01
#define WTDS_STYLE_XCEL 0x02

void WeekToDateString(int WeekNum, char* DateStringOut, int BfrSize);
void WeekToDateStringEx(int WeekNum, char* DateStringOut, int BfrSize, int Style);

#endif /* basetypes_h */
