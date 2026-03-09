#pragma once


#include <iostream>
#include <mutex>
#include <chrono>
#include <thread>


class TraceLogger {
public:
    TraceLogger(const char* func)
        : func_(func)
    {
        log("ENTER");
    }

    ~TraceLogger() {
        log("EXIT");
    }

private:
    const char* func_;

    static inline std::mutex mtx_;

    void log(const char* state) {
        std::lock_guard<std::mutex> lock(mtx_);
        std::cout << "[" << std::this_thread::get_id() << "] "
            << state << " "
            << func_;

        std::cout << "\n";
    }
};

#define TRACE_SCOPE() TraceLogger _trace_logger__(__func__)