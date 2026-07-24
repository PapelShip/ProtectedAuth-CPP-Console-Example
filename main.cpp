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

std::atomic<bool> g_sessionActive{ false };
std::mutex g_consoleMutex;

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
    QP_RunSecurityChecks(ctx);

    qpapel::SetConfig(ctx, _xorrr_("apikey"), "", 0, _xorrr_("2.0.0"));
    if (!qpapel::Connect(ctx)) {
        char* err = qpapel::GetLastStatus(ctx);
        std::cout << _xorrr_("[FAIL] Connection failed: ") << (err ? err : "Unknown") << std::endl;
        if (err) qpapel::FreeString(err);
        qpapel::DestroyContext(ctx);
        return 1;
    }
    std::string userKey;


    bool authed = false;
    while (!authed) {
        if (userKey.empty()) {
            std::cout << _xorrr_("Enter License Key: ");
            std::cin >> std::ws;
            std::getline(std::cin, userKey);
            std::cout << std::endl;
        }

        char* sessionToken = qpapel::Authenticate(ctx, userKey.c_str());
        
        if (sessionToken != nullptr) {
            authed = true;
            std::cout << "[SUCCESS] License Validated!" << std::endl;
            g_sessionActive = true;
            
            bool running = true;
            while (running) {
                std::cout << "\n--- Main Menu ---" << std::endl;
                std::cout << "1. View License Info" << std::endl;
                std::cout << "2. Get Server String" << std::endl;
                std::cout << "3. Download Server File" << std::endl;
                std::cout << "4. Run Server File (StartFile)" << std::endl;
                std::cout << "5. Load Product (PE Loader)" << std::endl;
                std::cout << "6. Exit" << std::endl;
            
                std::cout << "Select option: ";

                int choice;
                if (!(std::cin >> choice)) {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    continue;
                }
                
                if (choice == 1) {
                    char* info = qpapel::GetLicenseInfo(ctx, userKey.c_str());
                    if (info) {
                        std::cout << "\n--- License Information ---" << std::endl;
                        std::cout << info << std::endl;
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

                    char* val = qpapel::FetchString(ctx, accessId.c_str(), userKey.c_str());
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

                    if (qpapel::FetchFile(ctx, accessId.c_str(), userKey.c_str(), &fileData, &fileLen) && fileData != nullptr) {
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

                    if (qpapel::RunFile(ctx, accessId.c_str(), userKey.c_str(), args.c_str())) {
                        std::cout << "[SUCCESS] File executed." << std::endl;
                    }
                    else {
                        std::cout << "[FAILED] Execution failed." << std::endl;
                    }
                }
                else if (choice == 5) {
                    std::string accessId, targetProc;
                    std::cout << "Enter Access ID for DLL";
                    std::cin >> accessId;
                 
                    
                    std::cout << "Enter Target Process Name (e.g. notepad.exe or self): ";
                    std::cin >> targetProc;
                    if (targetProc.empty()) targetProc = "notepad.exe";
                    
                    std::cout << "[ServerMapper] Attempting to map DLL into " << targetProc << "..." << std::endl;
                    int result = qpapel::ServerMapper(ctx, accessId.c_str(), targetProc.c_str(), userKey.c_str());
                    if (result == 1) {
                        std::cout << "[SUCCESS] ServerMapper successfully injected DLL into " << targetProc << "!" << std::endl;
                    } else {
                        char* err = qpapel::GetLastStatus(ctx);
                        std::cout << "[FAILED] ServerMapper error: " << (err ? err : "Unknown Error") << std::endl;
                        if (err) qpapel::FreeString(err);
                    }
                }
                else if (choice == 6) {
                    running = false;
                }
            }
        }
        else {
            char* err = qpapel::GetLastStatus(ctx);
            std::cout << "\n[FAILED] Error: " << (err ? err : "Authentication failed") << std::endl;
            if (err) qpapel::FreeString(err);

            userKey.clear(); 
            std::cout << "\n[Press ENTER to try another key...]";
            std::cin.ignore(10000, '\n');
            std::cin.get();
         
        }
    }

    std::cout << _xorrr_("\nCleaning up and Exiting...") << std::endl;

    qpapel::DestroyContext(ctx);
    return 0;
}
