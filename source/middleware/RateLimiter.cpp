#include "middleware/RateLimiter.hpp"
#include <unordered_map>
#include <chrono>
#include <mutex>
#include <algorithm>

using namespace scaffold;

struct TokenBucket {
    double tokens;
    std::chrono::steady_clock::time_point last;
};

SimpleRateLimiter &SimpleRateLimiter::instance() {
    static SimpleRateLimiter rl;
    return rl;
}

bool SimpleRateLimiter::allow(const std::string &ip) {
    static std::unordered_map<std::string, TokenBucket> buckets;
    static std::mutex mtx;

    const double maxTokens = 5.0;      // burst size
    const double refillPerSec = 1.0;   // tokens per second

    std::lock_guard<std::mutex> g(mtx);
    auto &b = buckets[ip];
    if (b.last.time_since_epoch().count() == 0) {
        b.tokens = maxTokens;
        b.last = std::chrono::steady_clock::now();
    }
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> diff = now - b.last;
    b.tokens = std::min(maxTokens, b.tokens + diff.count() * refillPerSec);
    b.last = now;
    if (b.tokens >= 1.0) {
        b.tokens -= 1.0;
        return true;
    }
    return false;
}
