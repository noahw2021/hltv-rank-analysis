//
//  netapi_dl.c
//  hltv-rank-analysis
//
//  Created by Noah Wooten on 11/21/23.
//

#include <stdio.h>
#include "netapi.h"

// Mozilla Firefox User Agent, macOS 14.2 Beta (23C5047e) arm64
#define NETAPI_AGENT "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:109.0) Gecko/20100101 Firefox/119.0"

void NetDownload(
    const char* Url, char* Buffer,
    WORD32 BufferSize, WORD32* RequiredBufferSize
) {
    FILE* TempFile = tmpfile();
    if (!TempFile) {
        NetCtx->DidSucceed = 0;
        return;
    }
    
    curl_easy_setopt(NetCtx->ThisCurlCtx,
        CURLOPT_URL, Url);
    curl_easy_setopt(NetCtx->ThisCurlCtx,
        CURLOPT_WRITEFUNCTION, fwrite);
    curl_easy_setopt(NetCtx->ThisCurlCtx,
        CURLOPT_WRITEDATA, TempFile);
    curl_easy_setopt(NetCtx->ThisCurlCtx,
        CURLOPT_USERAGENT, NETAPI_AGENT);
    
    CURLcode UrlCode =
        curl_easy_perform(NetCtx->ThisCurlCtx);
    
    if (UrlCode != CURLE_OK)
        NetCtx->DidSucceed = 0;
    
    fseek(TempFile, 0, SEEK_END);
    WORD32 FileLength = ftell(TempFile);
    *RequiredBufferSize = FileLength;
    fseek(TempFile, 0, SEEK_SET);
    
    if (FileLength > BufferSize) {
        fclose(TempFile);
        return;
    }
    
    fread(Buffer, FileLength, 1, TempFile);
    fclose(TempFile);
    NetCtx->DidSucceed = 1;
    
    return;
}
