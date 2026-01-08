cmake_minimum_required(VERSION 4.2)
project(drogon_project_scaffold)

include(cmake/get_cpm.cmake)
include(cmake/libs.cmake)

set(CMAKE_CXX_STANDARD 20)

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
)

#add_custom_command(TARGET ${PROJECT_NAME}
#        POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/templates ${PROJECT_BINARY_DIR}
#        COMMENT "Copying templates"
#)

file( COPY ${PROJECT_SOURCE_DIR}/templates   DESTINATION ${PROJECT_BINARY_DIR} )
file( COPY ${PROJECT_SOURCE_DIR}/config.json DESTINATION ${PROJECT_BINARY_DIR} )

target_link_libraries(${PROJECT_NAME} PRIVATE drogon)
