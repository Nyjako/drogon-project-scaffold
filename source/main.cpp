#include <drogon/drogon.h>
#include "routers/MainPage.hpp"
#include "api/HelloApi.hpp"
#include "components/HelloComponent.hpp"
#include "middleware/RateLimiter.hpp"
#include "database/Database.hpp"
#include <filesystem>

void create_dirs()
{
    namespace fs = std::filesystem;

    // create required directories BEFORE drogon tries to open files
    fs::create_directories("data");
    fs::create_directories("logs");
    fs::create_directories("migrations");
}

int main()
{
    auto db = Database::open("./myapp.db");

    // Register routes/components/apis
    // drogon::app().setLogLevel(trantor::Logger::LogLevel::kDebug);
    create_dirs();

    // Serve static files (if you place files in ./static)
    drogon::app().loadConfigFile("config.json");

    registerMainPageRoutes();
    registerHelloComponentRoutes();
    registerApiRoutes();

    drogon::app().run();
    return 0;
}