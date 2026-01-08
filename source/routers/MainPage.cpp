#include "MainPage.hpp"
#include <drogon/drogon.h>
#include "../utils/template.hpp"


void registerMainPageRoutes() {
    using namespace drogon;


    app().registerHandler("/", [](const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback){
    std::unordered_map<std::string, std::string> ctx;
    ctx["title"] = "Drogon Demo";
    // layout.html contains the page shell with a div#content and the button
    std::string html = renderTemplateFromFile("templates/layout.html", ctx);
    auto resp = HttpResponse::newHttpResponse();
    resp->setBody(html);
    resp->setContentTypeCode(CT_TEXT_HTML);
    callback(resp);
    });
}