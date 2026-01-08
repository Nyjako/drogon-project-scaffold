#include "HelloComponent.hpp"
#include <drogon/drogon.h>
#include <chrono>


void registerHelloComponentRoutes() {
    using namespace drogon;


    app().registerHandler("/components/hello_fragment", [](const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback){
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::string body = "<div id=\"message\">Hello from server — ";
    body += std::string(std::ctime(&t));
    body += "</div>";
    auto resp = HttpResponse::newHttpResponse();
    resp->setBody(body);
    resp->setContentTypeCode(CT_TEXT_HTML);
    callback(resp);
    });
}