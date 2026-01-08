#include <drogon/drogon.h>
#include "routers/MainPage.hpp"
#include "api/HelloApi.hpp"
#include "components/HelloComponent.hpp"
#include "middleware/RateLimiter.hpp"

int main()
{
    // Register routes/components/apis
    drogon::app().setLogLevel(trantor::Logger::LogLevel::kDebug);


    registerMainPageRoutes();
    registerHelloComponentRoutes();
    registerApiRoutes();


    // Serve static files (if you place files in ./static)
    drogon::app().loadConfigFile("config.json");
    // Or setup in code
    // drogon::app().addListener("0.0.0.0", 8080);
    drogon::app().run();
    return 0;
}