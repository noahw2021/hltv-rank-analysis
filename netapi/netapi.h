//
//  netapi.h
//  hltv-rank-analysis
//
//  Created by Noah Wooten on 11/21/23.
//

#ifndef netapi_h
#define netapi_h
#include <curl/curl.h>
#include "../basetypes.h"

void NetInit(void);
void NetShutdown(void);

void NetDownload(const char* Url, char* Buffer,
    WORD32 BufferSize, WORD32* RequiredBufferSize);
BYTE NetDidSucceed(void);

typedef struct _NET_CTX {
    CURL* ThisCurlCtx;
    BYTE DidSucceed;
}NET_CTX, *PNET_CTX;
extern PNET_CTX NetCtx;

#endif /* netapi_h */
