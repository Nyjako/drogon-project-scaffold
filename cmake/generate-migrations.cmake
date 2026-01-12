file(GLOB MIGRATION_SCRIPTS "${CMAKE_SOURCE_DIR}/migrations/*.sql")

set(MIGRATIONS_HPP "${CMAKE_BINARY_DIR}/generated/migrations.hpp")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/generated")

# header file
file(WRITE ${MIGRATIONS_HPP} "// Auto-generated migrations header\n")
file(APPEND ${MIGRATIONS_HPP} "#pragma once\n\n")
file(APPEND ${MIGRATIONS_HPP} "#include <SQLiteCpp/SQLiteCpp.h>\n")
file(APPEND ${MIGRATIONS_HPP} "#include <vector>\n#include <functional>\n\n")
file(APPEND ${MIGRATIONS_HPP} "struct MigrationEntry {\n")
file(APPEND ${MIGRATIONS_HPP} "    int version;\n")
file(APPEND ${MIGRATIONS_HPP} "    std::function<void(SQLite::Database&)> migration;\n};\n\n")
file(APPEND ${MIGRATIONS_HPP} "static std::vector<MigrationEntry> migrations = {\n")

# Loop over each migration
foreach(script ${MIGRATION_SCRIPTS})
    # get numeric prefix 001_...
    get_filename_component(scriptName ${script} NAME_WE)
    string(SUBSTRING ${scriptName} 0 3 verString)
    string(REGEX REPLACE "^0+" "" ver ${verString})
    
    file(READ ${script} scriptContent)
    string(REPLACE "\n" "\\n" scriptContentEscaped "${scriptContent}")
    string(REPLACE "\"" "\\\"" scriptContentEscaped "${scriptContentEscaped}")

    file(APPEND ${MIGRATIONS_HPP} "    {${ver}, [](SQLite::Database& db) { db.exec(\"${scriptContentEscaped}\"); }},\n")
endforeach()

file(APPEND ${MIGRATIONS_HPP} "};\n")