#pragma once
#include <chrono>

unsigned benchmark(const std::function<void(void)> &callback) {
    auto start = std::chrono::steady_clock::now();
    callback();
    auto finish = std::chrono::steady_clock::now();
    return  std::chrono:: duration_cast<std::chrono::milliseconds>(finish - start).count();
}