#pragma once
#include <iostream>
#include <chrono>

#ifndef ChkRet
#define ChkRet(hr_)                                     \
    do {                                                \
        HRESULT _hr = (hr_);                            \
        if (FAILED(_hr)) {                              \
            std::cerr << "[ERROR] " << __func__         \
                      << " failed: 0x" << std::hex      \
                      << _hr << " (" << __FILE__        \
                      << ":" << __LINE__ << ")"         \
                      << std::endl;                     \
            return _hr;                                 \
        }                                               \
    } while (0)
#endif

#ifndef ChkRetVal
#define ChkRetVal(hr_, ret_)                            \
    do {                                                \
        HRESULT _hr = (hr_);                            \
        if (FAILED(_hr)) {                              \
            std::cerr << "[ERROR] " << __func__         \
                      << " failed: 0x" << std::hex      \
                      << _hr << " (" << __FILE__        \
                      << ":" << __LINE__ << ")"         \
                      << std::endl;                     \
            return (ret_);                              \
        }                                               \
    } while (0)
#endif

inline double GetHighPrecisionTime() {
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = currentTime - startTime;
    return elapsed.count();
};