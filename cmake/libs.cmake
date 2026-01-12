CPMAddPackage("gh:madler/zlib@1.3.1.2")
CPMAddPackage("gh:viaduck/openssl-cmake@3.6")
CPMAddPackage("gh:open-source-parsers/jsoncpp@1.9.6#1.9.6")

if (jsoncpp_ADDED)
    if (TARGET JsonCpp::JsonCpp AND NOT TARGET Jsoncpp_lib)
        add_library(Jsoncpp_lib ALIAS JsonCpp::JsonCpp)
        message(STATUS "Created alias: Jsoncpp_lib -> JsonCpp::JsonCpp")
    elseif (TARGET jsoncpp_lib AND NOT TARGET Jsoncpp_lib)
        add_library(Jsoncpp_lib ALIAS jsoncpp_lib)
        message(STATUS "Created alias: Jsoncpp_lib -> jsoncpp_lib")
    else()
        if (DEFINED jsoncpp_SOURCE_DIR)
            set(_candidate1 "${jsoncpp_SOURCE_DIR}/build/lib/libjsoncpp.a")
            set(_candidate2 "${jsoncpp_SOURCE_DIR}/lib/libjsoncpp.a")
            set(_candidate3 "${jsoncpp_SOURCE_DIR}/libjsoncpp.a")
            foreach(_cand IN LISTS _candidate1 _candidate2 _candidate3)
                if (EXISTS "${_cand}" AND NOT TARGET Jsoncpp_lib)
                    add_library(Jsoncpp_lib STATIC IMPORTED)
                    set_target_properties(Jsoncpp_lib PROPERTIES
                            IMPORTED_LOCATION "${_cand}"
                            INTERFACE_INCLUDE_DIRECTORIES "${jsoncpp_SOURCE_DIR}/include"
                    )
                    message(STATUS "Created IMPORTED Jsoncpp_lib -> ${_cand}")
                    break()
                endif()
            endforeach()
        endif()
    endif()
endif()

CPMAddPackage(
        NAME drogon
        VERSION 1.9.11
        GITHUB_REPOSITORY drogonframework/drogon
        OPTIONS
            "BUILD_EXAMPLES Off"
)

find_package (SQLite3 REQUIRED)

CPMAddPackage(
        NAME SQLiteCpp
        GITHUB_REPOSITORY SRombauts/SQLiteCpp
        VERSION 3.3.3
        GIT_TAG 3.3.3
)