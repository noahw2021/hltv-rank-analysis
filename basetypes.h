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


void WeekToDateString(int WeekNum, char* DateStringOut, int BfrSize);

#endif /* basetypes_h */
