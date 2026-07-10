#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <mutex>
#include <limits>
#include "Include/qPapelLib.h"

#pragma comment(lib, "Lib/qPapelLib.lib")
#pragma comment(lib, "Lib/libsodium.lib")

#ifndef _xorrr_
#define _xorrr_(x) x
#endif

std::atomic<bool> g_sessionActive{ false };
std::mutex g_consoleMutex;

static void PrintCleanInfo(std::string info) {
    size_t pos = 0;
    while ((pos = info.find("\\n", pos)) != std::string::npos) {
        info.replace(pos, 2, "\n");
        pos += 1;
    }
    std::cout << info << std::endl;
}

static void ReportFailureAndExit(QPCTX ctx, const std::string& message) {
    std::cout << "\n" << message << std::endl;
    std::cout << "[SYSTEM] Press Enter to close application..." << std::endl;

    std::cin.clear();
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    std::cin.get();

    if (ctx) qpapel::DestroyContext(ctx);
    std::exit(1);
}

__declspec(noinline) static void obfcrash() {
    DWORD delay = 1000 + (GetTickCount() % 3000);
    Sleep(delay);

    volatile char* p = (volatile char*)malloc(32);
    if (p) {
        for (int i = 0; i < 64; i++) {
            p[i] = (char)(i ^ 0xAA);
        }
        free((void*)p);
    }

    volatile int* np = nullptr;
    *np = 0xDEAD;
}

static void SessionCheckLoop(QPCTX ctx) {
    std::this_thread::sleep_for(std::chrono::seconds(5));

    while (g_sessionActive) {
        if (qpapel::CheckIntegrity(ctx) != 0) { 
            g_sessionActive = false;
            obfcrash();
        }

        if (qpapel::Connect(ctx) == 0) {
            std::lock_guard<std::mutex> lock(g_consoleMutex);
            char* err = qpapel::GetLastStatus(ctx);
            std::cout << "\n\n[FATAL] Session integrity check failed or remote link closed ("
                << (err ? err : "ERR_CONN_LOST") << ")" << std::endl;

            if (err) qpapel::FreeString(err);

            g_sessionActive = false;
            exit(1);
        }

        std::this_thread::sleep_for(std::chrono::seconds(20));
    }
}

static void tmploop(QPCTX ctx) {
    while (true) {
        if (qpapel::CheckIntegrity(ctx) != 0) {
            qpapel::ReportEvent(ctx, "tamper", "ban", "{\"scrnshot\":true}");
            std::exit(-1);
        }

        qpapel::OptimizeClock(ctx);
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

int main() {
    std::cout << _xorrr_("==================================================") << std::endl;
    std::cout << _xorrr_("[SYSTEM] Initializing client runtime...") << std::endl;

    if (!qpapel::Init()) {
        ReportFailureAndExit(nullptr, "[CRITICAL] Failed to initialize security stub.");
    }

    QPCTX ctx = qpapel::CreateContext();
    if (!ctx) {
        ReportFailureAndExit(nullptr, "[CRITICAL] Memory allocation failure (context null).");
    }

    qpapel::SetConfig(ctx, _xorrr_("apikey"), "", 0, _xorrr_("version"));

    qpapel::CheckIntegrity(ctx);

    if (qpapel::Connect(ctx)) {
        std::cout << _xorrr_("[NETWORK] Secure channel established with licensing endpoint.") << std::endl;
        std::cout << _xorrr_("\n[SECURITY] Enter License Key: ");

        std::string licenseKey;
        std::cin >> licenseKey;

        std::thread t(tmploop, ctx);
        t.detach();

        std::cout << _xorrr_("[CRYPT] Performing challenge-response handshake...") << std::endl;
        char* sessionToken = qpapel::Authenticate(ctx, licenseKey.c_str());

        if (sessionToken != nullptr) {
            std::cout << "[SUCCESS] Handshake verified. Session authenticated." << std::endl;
            qpapel::FreeString(sessionToken); 
            g_sessionActive = true;
        
            std::thread watchdog(SessionCheckLoop, ctx);
            watchdog.detach();

            bool running = true;
            while (running) {
                std::cout << "\n-----------------------" << std::endl;
                std::cout << "1. Query License Status" << std::endl;
                std::cout << "2. Retrieve Remote Variable" << std::endl;
                std::cout << "3. Pull Remote Asset" << std::endl;
                std::cout << "4. Execute Protected Image" << std::endl;
                std::cout << "5. Terminate" << std::endl;
                std::cout << "Select Operation: ";

                int choice;
                std::cin >> choice;

                if (choice == 1) {
                    char* info = qpapel::GetLicenseInfo(ctx, licenseKey.c_str());
                    if (info) {
                        std::cout << "\n--- Node Descriptor ---" << std::endl;
                        PrintCleanInfo(info);
                        std::cout << "-----------------------" << std::endl;
                        qpapel::FreeString(info); 
                    } else {
                        char* err = qpapel::GetLastStatus(ctx);
                        std::cout << "[ERROR] Query failed: " << (err ? err : "ERR_UNKNOWN") << std::endl;
                        if (err) qpapel::FreeString(err);
                    }
                } else if (choice == 2) {
                    std::string accessId;
                    std::cout << "Enter Asset Reference ID: ";
                    std::cin >> accessId;

                    char* val = qpapel::FetchString(ctx, accessId.c_str(), licenseKey.c_str());
                    if (val) {
                        std::cout << "[DATA] Retrieved value: " << val << std::endl;
                        qpapel::FreeString(val);
                    } else {
                        std::cout << "[ERROR] Access denied or variable not found." << std::endl;
                    }
                } else if (choice == 3) {
                    std::string accessId;
                    std::cout << "Enter File Reference ID: ";
                    std::cin >> accessId;
                    std::cout << "[TRANSFER] Downloading remote file stream..." << std::endl;

                    unsigned char* fileData = nullptr;
                    int fileLen = 0;

                    if (qpapel::FetchFile(ctx, accessId.c_str(), licenseKey.c_str(), &fileData, &fileLen) && fileData != nullptr) {
                        std::ofstream out("downloaded_file.exe", std::ios::binary);
                        out.write(reinterpret_cast<char*>(fileData), fileLen);
                        out.close();

                        std::cout << "[SUCCESS] File written to disk ('downloaded_file.exe')." << std::endl;
                        qpapel::FreeBytes(fileData); 
                    } else {
                        std::cout << "[ERROR] Data transmission failed." << std::endl;
                    }
                } else if (choice == 4) {
                    std::string accessId, args;
                    std::cout << "Enter Executable Reference ID: ";
                    std::cin >> accessId;
                    std::cout << "Enter Runtime Arguments (type 'none' for none): ";

                    std::cin >> std::ws; 
                    std::getline(std::cin, args);
                    if (args == "none") args = "";

                    std::cout << "[SECURITY] Starting remote process execution..." << std::endl;

                    if (qpapel::RunFile(ctx, accessId.c_str(), licenseKey.c_str(), args.c_str())) {
                        std::cout << "[SUCCESS] Subprocess executed." << std::endl;
                    } else {
                        std::cout << "[ERROR] Loader execution failed." << std::endl;
                    }
                } else if (choice == 5) {
                    running = false;
                }
            }
        } else {
            char* err = qpapel::GetLastStatus(ctx);
            std::string errMsg = "[CRITICAL] Authentication failed: " + (err ? std::string(err) : "ERR_INVALID_SESSION");
            if (err) qpapel::FreeString(err);
            ReportFailureAndExit(ctx, errMsg);
        }
    } else {
        char* err = qpapel::GetLastStatus(ctx);
        std::string errMsg = "[CRITICAL] Handshake timed out: " + (err ? std::string(err) : "ERR_NETWORK_DISCONNECTED");
        if (err) qpapel::FreeString(err);
        ReportFailureAndExit(ctx, errMsg);
    }

    std::cout << _xorrr_("\n[SYSTEM] Deallocating secure context and exiting...") << std::endl;
    qpapel::DestroyContext(ctx);

    return 0;
}
