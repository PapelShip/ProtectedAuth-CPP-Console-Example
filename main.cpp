#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <mutex>
#include "Include/qPapelLib.h"

#pragma comment(lib, "Lib/qPapelLib.lib")
#pragma comment(lib, "Lib/libsodium.lib")

#ifndef _xorrr_
#define _xorrr_(x) x
#endif

std::atomic<bool> g_sessionActive{ false };
std::mutex g_consoleMutex;

void PrintCleanInfo(std::string info) {
    size_t pos = 0;
    while ((pos = info.find("\\n", pos)) != std::string::npos) {
        info.replace(pos, 2, "\n");
        pos += 1;
    }
    std::cout << info << std::endl;
}

__declspec(noinline) static void obfcrash() {
    DWORD delay = 1000 + (GetTickCount() % 3000);
    Sleep(delay);

    volatile char* p = (volatile char*)malloc(32);
    if (p) {
        for (int i = 0; i < 64; i++) p[i] = (char)(i ^ 0xAA);
        free((void*)p);
    }

    volatile int* np = nullptr;
    *np = 0xDEAD;
}

void SessionCheckLoop(QPCTX ctx) {
    std::this_thread::sleep_for(std::chrono::seconds(5));

    while (g_sessionActive) {
        if (qpapel::CheckIntegrity(ctx) != 0) { 
            g_sessionActive = false;
            obfcrash();
        }
        if (qpapel::Connect(ctx) == 0) {
            std::lock_guard<std::mutex> lock(g_consoleMutex);
            char* err = qpapel::GetLastStatus(ctx);
            std::cout << "\n\n[FATAL] Server Connection Lost or Integrity Check Failed! ("
                << (err ? err : "Unknown Error") << ")" << std::endl;

            if (err) qpapel::FreeString(err);

            std::cout << "Exiting for security..." << std::endl;
            g_sessionActive = false;
            exit(1);
        }

        std::this_thread::sleep_for(std::chrono::seconds(20));
    }
}

void tmploop(QPCTX ctx) {
    while (true) {



        qpapel::OptimizeClock(ctx);

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

int main() {
    std::cout << _xorrr_("--- PapelShip C++ Demo ---") << std::endl;
    std::cout << _xorrr_("[Loader] Initializing...") << std::endl;
    
    if (!qpapel::Init()) {
        std::cout << _xorrr_("[FAILED] Core stub initialization failed.") << std::endl;
        return 1;
    }

    QPCTX ctx = qpapel::CreateContext();
    if (!ctx) {
        std::cout << _xorrr_("[FAILED] Could not allocate QPCTX.") << std::endl;
        return 1;
    }

    qpapel::SetConfig(ctx, _xorrr_("apikey"), "", 0, _xorrr_("1.0.0"));

    //qpapel::CheckIntegrity(ctx);

    if (qpapel::Connect(ctx)) {
        std::cout << _xorrr_("[SUCCESS] Secure connection established.") << std::endl;
        std::cout << _xorrr_("\n[Locked] Enter License Key to continue: ");

        std::string licenseKey;
        std::cin >> licenseKey;

    
        std::cout << _xorrr_("[Auth] Validating key...") << std::endl;

        char* sessionToken = qpapel::Authenticate(ctx, licenseKey.c_str());

        if (sessionToken != nullptr) {
            std::cout << "[SUCCESS] License Validated!" << std::endl;
            qpapel::FreeString(sessionToken); 
            g_sessionActive = true;
            std::thread t(tmploop, ctx);
            t.detach();

            std::thread watchdog(SessionCheckLoop, ctx);
            watchdog.detach();

            bool running = true;
            while (running) {
                std::cout << "\n--- Main Menu ---" << std::endl;
                std::cout << "1. View License Info" << std::endl;
                std::cout << "2. Get Server String" << std::endl;
                std::cout << "3. Download Server File" << std::endl;
                std::cout << "4. Run Server File (StartFile)" << std::endl;
                std::cout << "5. Exit" << std::endl;
                std::cout << "Select option: ";

                int choice;
                std::cin >> choice;

                if (choice == 1) {
                    char* info = qpapel::GetLicenseInfo(ctx, licenseKey.c_str());
                    if (info) {
                        std::cout << "\n--- License Information ---" << std::endl;
                        PrintCleanInfo(info);
                        std::cout << "---------------------------" << std::endl;
                        qpapel::FreeString(info); 
                    }
                    else {
                        char* err = qpapel::GetLastStatus(ctx);
                        std::cout << "[FAILED] " << (err ? err : "Unknown") << std::endl;
                        if (err) qpapel::FreeString(err);
                    }
                }
                else if (choice == 2) {
                    std::string accessId;
                    std::cout << "Enter String Access ID: ";
                    std::cin >> accessId;

                    char* val = qpapel::FetchString(ctx, accessId.c_str(), licenseKey.c_str());
                    if (val) {
                        std::cout << "[Server String]: " << val << std::endl;
                        qpapel::FreeString(val);
                    }
                    else {
                        std::cout << "[Error] Failed to fetch string (Check ID or Permissions)." << std::endl;
                    }
                }
                else if (choice == 3) {
                    std::string accessId;
                    std::cout << "Enter File Access ID to Download: ";
                    std::cin >> accessId;
                    std::cout << "[Downloading]..." << std::endl;

                    unsigned char* fileData = nullptr;
                    int fileLen = 0;

                    if (qpapel::FetchFile(ctx, accessId.c_str(), licenseKey.c_str(), &fileData, &fileLen) && fileData != nullptr) {
                        std::ofstream out("downloaded_file.exe", std::ios::binary);
                        out.write(reinterpret_cast<char*>(fileData), fileLen);
                        out.close();

                        std::cout << "[SUCCESS] File downloaded to 'downloaded_file.exe'." << std::endl;
                        qpapel::FreeBytes(fileData); 
                    }
                    else {
                        std::cout << "[FAILED] Download failed." << std::endl;
                    }
                }
                else if (choice == 4) {
                    std::string accessId, args;
                    std::cout << "Enter File Access ID to Run: ";
                    std::cin >> accessId;
                    std::cout << "Enter Arguments (optional, type 'none' for empty): ";

                    std::cin >> std::ws; 
                    std::getline(std::cin, args);
                    if (args == "none") args = "";

                    std::cout << "[Launching]..." << std::endl;

                    if (qpapel::RunFile(ctx, accessId.c_str(), licenseKey.c_str(), args.c_str())) {
                        std::cout << "[SUCCESS] File executed." << std::endl;
                    }
                    else {
                        std::cout << "[FAILED] Execution failed." << std::endl;
                    }
                }
                else if (choice == 5) {
                    running = false;
                }
            }
        }
        else {
            char* err = qpapel::GetLastStatus(ctx);
            std::cout << "[FAILED] Error: " << (err ? err : "Authentication failed") << std::endl;
            if (err) qpapel::FreeString(err);
        }
    }
    else {
        char* err = qpapel::GetLastStatus(ctx);
        std::cout << _xorrr_("[FAILED] Access Denied. ") << (err ? err : "Connection failed") << std::endl;
        if (err) qpapel::FreeString(err);
    }

    std::cout << _xorrr_("\nCleaning up and Exiting...") << std::endl;

   
    qpapel::DestroyContext(ctx);

    return 0;
}