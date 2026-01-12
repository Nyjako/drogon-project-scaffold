cmake_minimum_required(VERSION 4.2)
project(drogon_project_scaffold
        VERSION 0.0.1
        DESCRIPTION "Simple web app"
        LANGUAGES CXX
)

include(cmake/generate-migrations.cmake)
include(cmake/get_cpm.cmake)
include(cmake/libs.cmake)

set(CMAKE_CXX_STANDARD 20)

# ---- Default user that is created at first launch ----
set(DEFAULT_USERNAME "Admin")
set(DEFAULT_PASSWORD "Admin")

# ---- Password generator settings ----
set(PASSWORD_GENERATOR_DICTIONARY "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789")
set(PASSWORD_GENERATOR_LENGTH 8)

configure_file(
        "${PROJECT_SOURCE_DIR}/config/config.h.in"
        "${PROJECT_BINARY_DIR}/generated/config.h"
        @ONLY
)

# ---- Default user that is created at first launch ----
set(DEFAULT_USERNAME "Admin")
set(DEFAULT_PASSWORD "Admin")

# ---- Password generator settings ----
set(PASSWORD_GENERATOR_DICTIONARY "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789")
set(PASSWORD_GENERATOR_LENGTH 8)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source)

add_executable(${PROJECT_NAME}
        source/main.cpp

        source/routers/MainPage.cpp
        source/routers/MainPage.hpp

        source/api/HelloApi.cpp
        source/api/HelloApi.hpp

        source/components/HelloComponent.cpp
        source/components/HelloComponent.hpp

        source/middleware/RateLimiter.cpp
        source/middleware/RateLimiter.hpp

        source/utils/template.cpp
        source/utils/template.hpp

        source/database/Database.cpp
        source/database/Database.hpp
        source/database/app_config.cpp
        source/database/app_config.hpp
)

#add_custom_command(TARGET ${PROJECT_NAME}
#        POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/templates ${PROJECT_BINARY_DIR}
#        COMMENT "Copying templates"
#)

file( COPY ${PROJECT_SOURCE_DIR}/templates   DESTINATION ${PROJECT_BINARY_DIR} )
file( COPY ${PROJECT_SOURCE_DIR}/migrations  DESTINATION ${PROJECT_BINARY_DIR} )
file( COPY ${PROJECT_SOURCE_DIR}/config.json DESTINATION ${PROJECT_BINARY_DIR} )

target_link_libraries(${PROJECT_NAME} PRIVATE
        drogon
        SQLiteCpp
)

include_directories(FileShare PUBLIC
        ${PROJECT_BINARY_DIR}/generated
)