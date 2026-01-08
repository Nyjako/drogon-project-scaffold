#include "HelloApi.hpp"
#include <drogon/drogon.h>
#include "../middleware/RateLimiter.hpp"
#include <chrono>


void registerApiRoutes() {
    using namespace drogon;

    app().registerHandler("/api/hello", [](const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback){
        // Get client IP: prefer X-Forwarded-For if present (when behind proxies),
        // otherwise use peer address from the TCP connection.
        std::string ip;
        const auto xff = req->getHeader("X-Forwarded-For");
        if (!xff.empty()) {
            // X-Forwarded-For may contain a comma-separated list; take the first
            auto pos = xff.find(',');
            ip = (pos == std::string::npos) ? xff : xff.substr(0, pos);
        } else {
            // getPeerAddr() returns trantor::InetAddress
            ip = req->getPeerAddr().toIp();
        }

        // Use your renamed rate limiter
        if (!scaffold::SimpleRateLimiter::instance().allow(ip)) {
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k429TooManyRequests);
            resp->setBody(R"({"error":"rate limit exceeded"})");
            resp->setContentTypeCode(CT_APPLICATION_JSON);
            callback(resp);
            return;
        }

        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        Json::Value json;
        json["message"] = "Hello from API";
        json["time"] = std::string(std::ctime(&t));
        auto resp = HttpResponse::newHttpJsonResponse(json);
        callback(resp);
    });
}