#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <cstdint>
#include <cstddef>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* QPCTX;

typedef struct {
    char accessId[128];
    char displayName[256];
    int isForced;
    char payloadType[32];
} QP_ProductInfo;

bool  QP_StubInit();
QPCTX QP_CreateContext();
void  QP_DestroyContext(QPCTX c);
void  QP_SetConfig(QPCTX c, const char* apiKey, const char* ip, int port, const char* version);
void  QP_DisableWatchdog(QPCTX c);
int   QP_Connect(QPCTX c);
char* QP_Authenticate(QPCTX c, const char* licenseKey);
char* QP_GetLicenseInfo(QPCTX c, const char* licenseKey);
char* QP_FetchString(QPCTX c, const char* stringId, const char* licenseKey);
int   QP_FetchFile(QPCTX c, const char* fileId, const char* licenseKey, unsigned char** outData, int* outLen);
int   QP_RunFile(QPCTX c, const char* fileId, const char* licenseKey, const char* arguments);
int   QP_ServerMapper(QPCTX c, const char* accessId, const char* targetProcess, const char* licenseKey);
int   QP_CheckIntegrity(QPCTX c);
int   QP_CheckDebugger(QPCTX c);
char* QP_RunSecurityChecks(QPCTX c);
void  QP_ReportEvent(QPCTX c, const char* eventType, const char* action, const char* eventData);
void  QP_OptimizeClock(QPCTX c);
char* QP_GetLastStatus(QPCTX c);
char* QP_GetToken(QPCTX c);
char* QP_DecryptLogBuffer();
char* QP_EnumChannels(QPCTX c, const char* licenseKey);
char* QP_RequestBlock(QPCTX c, const char* channelId, const char* blockId, const char* licenseKey);
int   QP_ReadSegment(QPCTX c, const char* channelId, int segmentIndex, const char* licenseKey, unsigned char** outData, int* outLen);
int   QP_PE_LoadProduct(QPCTX c, const char* accessId, const char* licenseKey, char** outError, unsigned int* outPid);
int   QP_GetAvailableProducts(QPCTX c, const char* licenseKey, QP_ProductInfo* outProducts, int maxProducts, int* outCount);
void  QP_GetTraceUuid(QPCTX c, char* outUuid, int maxLen);
int   QP_GetTraceHierarchy(QPCTX c, int* outHierarchy, int maxCount);
void  QP_FreeString(char* s);
void  QP_FreeBytes(unsigned char* p);

int   QP_SessionGuard(QPCTX c);
unsigned int QP_GetSignedResourceVersion(void);
char* QP_GetMachineHash(QPCTX c);
char* QP_AuthenticateSigned(QPCTX c, const char* licenseKey, const unsigned char* nonce32,
    unsigned char** leaseProof, int* proofLen);
int QP_FetchStringSigned(QPCTX c, const char* id, const char* licenseKey,
    const unsigned char* nonce32, unsigned char** payload, int* payloadLen,
    unsigned char** proof, int* proofLen);
int QP_FetchFileSigned(QPCTX c, const char* id, const char* licenseKey,
    const unsigned char* nonce32, unsigned char** payload, int* payloadLen,
    unsigned char** proof, int* proofLen);

void  QP_SetConfigEncrypted(QPCTX c,
                            const unsigned char* encKey, int encLen, int rotor,
                            const char* ip, int port, const char* version);

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
    inline void  SetConfigEncrypted(QPCTX c, const unsigned char* enc, int n, int rotor, const char* ip, int p, const char* v) { QP_SetConfigEncrypted(c, enc, n, rotor, ip, p, v); }
    inline void  DisableWatchdog(QPCTX c)                                                            { QP_DisableWatchdog(c); }
    inline int   Connect(QPCTX c)                                                                    { return QP_Connect(c); }
    inline char* Authenticate(QPCTX c, const char* lk)                                               { return QP_Authenticate(c, lk); }
    inline char* GetLicenseInfo(QPCTX c, const char* lk)                                             { return QP_GetLicenseInfo(c, lk); }
    inline char* FetchString(QPCTX c, const char* id, const char* lk)                                { return QP_FetchString(c, id, lk); }
    inline int   FetchFile(QPCTX c, const char* id, const char* lk, unsigned char** outD, int* outL) { return QP_FetchFile(c, id, lk, outD, outL); }
    inline int   RunFile(QPCTX c, const char* id, const char* lk, const char* args)                  { return QP_RunFile(c, id, lk, args); }
    inline int   ServerMapper(QPCTX c, const char* accessId, const char* targetProcess, const char* licenseKey) { return QP_ServerMapper(c, accessId, targetProcess, licenseKey); }
    inline int   CheckIntegrity(QPCTX c)                                                             { return QP_CheckIntegrity(c); }
    inline int   CheckDebugger(QPCTX c)                                                              { return QP_CheckDebugger(c); }
    inline char* RunSecurityChecks(QPCTX c)                                                          { return QP_RunSecurityChecks(c); }
    inline void  ReportEvent(QPCTX c, const char* ev, const char* act, const char* data)             { QP_ReportEvent(c, ev, act, data); }
    inline void  OptimizeClock(QPCTX c)                                                              { QP_OptimizeClock(c); }
    inline char* GetLastStatus(QPCTX c)                                                              { return QP_GetLastStatus(c); }
    inline char* GetToken(QPCTX c)                                                                   { return QP_GetToken(c); }
    inline char* DecryptLogBuffer()                                                                  { return QP_DecryptLogBuffer(); }
    inline char* EnumChannels(QPCTX c, const char* lk)                                               { return QP_EnumChannels(c, lk); }
    inline char* RequestBlock(QPCTX c, const char* ch, const char* bl, const char* lk)               { return QP_RequestBlock(c, ch, bl, lk); }
    inline int   ReadSegment(QPCTX c, const char* ch, int idx, const char* lk, unsigned char** outD, int* outL) { return QP_ReadSegment(c, ch, idx, lk, outD, outL); }
    inline int   PE_LoadProduct(QPCTX c, const char* accessId, const char* licenseKey, char** outError, unsigned int* outPid) { return QP_PE_LoadProduct(c, accessId, licenseKey, outError, outPid); }
    inline int   GetAvailableProducts(QPCTX c, const char* licenseKey, QP_ProductInfo* outProducts, int maxProducts, int* outCount) { return QP_GetAvailableProducts(c, licenseKey, outProducts, maxProducts, outCount); }
    inline void  GetTraceUuid(QPCTX c, char* outUuid, int maxLen)                                    { QP_GetTraceUuid(c, outUuid, maxLen); }
    inline int   GetTraceHierarchy(QPCTX c, int* outHierarchy, int maxCount)                         { return QP_GetTraceHierarchy(c, outHierarchy, maxCount); }
    inline void  FreeString(char* s)                                                                 { QP_FreeString(s); }
    inline void  FreeBytes(unsigned char* p)                                                         { QP_FreeBytes(p); }
    inline int   SessionGuard(QPCTX c)                                                               { return QP_SessionGuard(c); }
}

namespace _qp_xs_detail {

    template <size_t N, int K1, int K2>
    struct Enc {
        char d[N];
        constexpr Enc(const char (&s)[N]) : d{} {
            for (size_t i = 0; i < N; ++i) {
                d[i] = static_cast<char>(s[i] ^ static_cast<char>(K1 + (i * 7) + K2));
            }
        }
    };

    template <size_t N, int K1, int K2>
    struct Dec {
        char buf[N];
        __forceinline Dec(const Enc<N, K1, K2>& e) : buf{} {
            for (size_t i = 0; i < N; ++i) {
                buf[i] = static_cast<char>(e.d[i] ^ static_cast<char>(K1 + (i * 7) + K2));
            }
        }
        __forceinline const char* c_str() const { return buf; }
        __forceinline const char* get()   const { return buf; }
        __forceinline operator const char*() const { return buf; }
        __forceinline operator std::string() const {
            return std::string(buf, (N > 0 && buf[N - 1] == '\0') ? N - 1 : N);
        }
        __forceinline std::string str() const {
            return std::string(buf, (N > 0 && buf[N - 1] == '\0') ? N - 1 : N);
        }
        template <typename Stream>
        friend Stream& operator<<(Stream& os, const Dec& d) { return os << d.buf; }
    };

}

template <size_t N, int K1, int K2 = 0x5A>
struct XorStr {
    _qp_xs_detail::Enc<N, K1, K2> _enc;
    constexpr XorStr(const char (&s)[N]) : _enc(s) {}
    __forceinline _qp_xs_detail::Dec<N, K1, K2> get() const {
        return _qp_xs_detail::Dec<N, K1, K2>(_enc);
    }
    using DecryptedString = _qp_xs_detail::Dec<N, K1, K2>;
};

#ifndef _xorrr_

#define _xorrr_(s)                                                              \
    ([]() {                                                                     \
        constexpr int _k1_ = __COUNTER__ + 1;                                   \
        constexpr ::_qp_xs_detail::Enc<sizeof(s), _k1_, 0x5A> _e(s);            \
        return ::_qp_xs_detail::Dec<sizeof(s), _k1_, 0x5A>(_e);                 \
    }())
#endif

#endif
