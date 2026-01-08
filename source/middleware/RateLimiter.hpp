#pragma once
#include <string>

namespace scaffold {

    // Very small in-memory rate limiter (per-IP token bucket).
    class SimpleRateLimiter {
    public:
        static SimpleRateLimiter &instance();
        bool allow(const std::string &ip);
    };

} // namespace scaffold