CPMAddPackage(
        NAME drogon
        VERSION 1.9.11
        GITHUB_REPOSITORY drogonframework/drogon
        OPTIONS
            "BUILD_EXAMPLES Off"
)


#find_package (SQLite3 REQUIRED)
#
#CPMAddPackage(
#        NAME SQLiteCpp
#        GITHUB_REPOSITORY SRombauts/SQLiteCpp
#        VERSION 3.3.3
#        GIT_TAG 3.3.3
#)