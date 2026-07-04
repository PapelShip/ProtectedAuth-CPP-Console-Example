// ════════════════════════════════════════════════════════════════
//  qPapelLib.h  —  qPapelAuth DLL Loader Library Interface
//  Distribute this header + qPapelLib.lib to developers.
//  Protects internal loader stub logic from developer inspection.
// ════════════════════════════════════════════════════════════════
#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

// ── Types ───────────────────────────────────────────────────────
typedef void* QPCTX;

typedef struct {
    char accessId[128];
    char displayName[256];
    int isForced;
    char payloadType[32];
} QP_ProductInfo;

// ── Raw C Export Symbols (Global Scope) ─────────────────────────
bool  QP_StubInit();
QPCTX QP_CreateContext();
void  QP_DestroyContext(QPCTX c);
void  QP_SetConfig(QPCTX c, const char* apiKey, const char* ip, int port, const char* version);
int   QP_Connect(QPCTX c);
char* QP_Authenticate(QPCTX c, const char* licenseKey);
char* QP_GetLicenseInfo(QPCTX c, const char* licenseKey);
char* QP_FetchString(QPCTX c, const char* stringId, const char* licenseKey);
int   QP_FetchFile(QPCTX c, const char* fileId, const char* licenseKey, unsigned char** outData, int* outLen);
int   QP_RunFile(QPCTX c, const char* fileId, const char* licenseKey, const char* arguments);
int   QP_CheckIntegrity(QPCTX c);
int   QP_CheckDebugger(QPCTX c);
void  QP_ReportEvent(QPCTX c, const char* eventType, const char* action, const char* eventData);
void  QP_OptimizeClock(QPCTX c);
char* QP_GetLastStatus(QPCTX c);
char* QP_GetToken(QPCTX c);
char* QP_EnumChannels(QPCTX c, const char* licenseKey);
char* QP_RequestBlock(QPCTX c, const char* channelId, const char* blockId, const char* licenseKey);
int   QP_ReadSegment(QPCTX c, const char* channelId, int segmentIndex, const char* licenseKey, unsigned char** outData, int* outLen);
int   QP_PE_LoadProduct(QPCTX c, const char* accessId, const char* licenseKey, char** outError, unsigned int* outPid);
int   QP_GetAvailableProducts(QPCTX c, const char* licenseKey, QP_ProductInfo* outProducts, int maxProducts, int* outCount);
void  QP_FreeString(char* s);
void  QP_FreeBytes(unsigned char* p);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace qpapel
{
    inline bool  Init()                                                                              { return QP_StubInit(); }
    inline QPCTX CreateContext()                                                                     { return QP_CreateContext(); }
    inline void  DestroyContext(QPCTX c)                                                             { QP_DestroyContext(c); }
    inline void  SetConfig(QPCTX c, const char* k, const char* ip, int p, const char* v)             { QP_SetConfig(c, k, ip, p, v); }
    inline int   Connect(QPCTX c)                                                                    { return QP_Connect(c); }
    inline char* Authenticate(QPCTX c, const char* lk)                                               { return QP_Authenticate(c, lk); }
    inline char* GetLicenseInfo(QPCTX c, const char* lk)                                             { return QP_GetLicenseInfo(c, lk); }
    inline char* FetchString(QPCTX c, const char* id, const char* lk)                                { return QP_FetchString(c, id, lk); }
    inline int   FetchFile(QPCTX c, const char* id, const char* lk, unsigned char** outD, int* outL) { return QP_FetchFile(c, id, lk, outD, outL); }
    inline int   RunFile(QPCTX c, const char* id, const char* lk, const char* args)                  { return QP_RunFile(c, id, lk, args); }
    inline int   CheckIntegrity(QPCTX c)                                                             { return QP_CheckIntegrity(c); }
    inline int   CheckDebugger(QPCTX c)                                                              { return QP_CheckDebugger(c); }
    inline void  ReportEvent(QPCTX c, const char* ev, const char* act, const char* data)             { QP_ReportEvent(c, ev, act, data); }
    inline void  OptimizeClock(QPCTX c)                                                              { QP_OptimizeClock(c); }
    inline char* GetLastStatus(QPCTX c)                                                              { return QP_GetLastStatus(c); }
    inline char* GetToken(QPCTX c)                                                                   { return QP_GetToken(c); }
    inline char* EnumChannels(QPCTX c, const char* lk)                                               { return QP_EnumChannels(c, lk); }
    inline char* RequestBlock(QPCTX c, const char* ch, const char* bl, const char* lk)               { return QP_RequestBlock(c, ch, bl, lk); }
    inline int   ReadSegment(QPCTX c, const char* ch, int idx, const char* lk, unsigned char** outD, int* outL) { return QP_ReadSegment(c, ch, idx, lk, outD, outL); }
    inline int   PE_LoadProduct(QPCTX c, const char* accessId, const char* licenseKey, char** outError, unsigned int* outPid) { return QP_PE_LoadProduct(c, accessId, licenseKey, outError, outPid); }
    inline int   GetAvailableProducts(QPCTX c, const char* licenseKey, QP_ProductInfo* outProducts, int maxProducts, int* outCount) { return QP_GetAvailableProducts(c, licenseKey, outProducts, maxProducts, outCount); }
    inline void  FreeString(char* s)                                                                 { QP_FreeString(s); }
    inline void  FreeBytes(unsigned char* p)                                                         { QP_FreeBytes(p); }
}
#endif

