//
// Created by nyjako on 02.07.25.
//

#include "Database.hpp"
#include <format>
#include <config.h>
#include <migrations.hpp>
#include <utility>
#include "app_config.hpp"
#include "config.h"

void Database::init_db_info(SQLite::Database& database)
{
    database.exec(
        "CREATE TABLE IF NOT EXISTS db_info "
        "(project_version INTEGER, db_version INTEGER);"
    );
    // If no entry exists, insert default version 0
    int count = database.execAndGet("SELECT COUNT(*) FROM db_info").getInt();
    if (count == 0) {
        database.exec("INSERT INTO db_info (project_version, db_version) VALUES (0, 0);");
    }
}

void Database::check_users(SQLite::Database &database)
{
    int count = database.execAndGet("SELECT COUNT(*) FROM user").getInt();
    if (count == 0) {
        // CROW_LOG_CRITICAL << "No users found in database.";
        SQLite::Statement query(database, "INSERT INTO user (username, password, force_pass_change, permissions) VALUES (:username, :password, :force_pass_change, :permissions);");
        query.bind(":username",         DEFAULT_USERNAME);
        // query.bind(":password",          Cryptography::hash(DEFAULT_PASSWORD));
        query.bind(":password",          DEFAULT_PASSWORD);
        query.bind(":force_pass_change", DEFAULT_PASSWORD == "Admin"); // If password is changed we don't need to force change
        query.bind(":permissions",       1);
        query.exec();

        if (DEFAULT_PASSWORD == "Admin") {
            // CROW_LOG_CRITICAL << std::format("Created default user. Change password as soon as possible. Login: {}, Password: {}", DEFAULT_USERNAME, DEFAULT_PASSWORD);
        } else {
            // CROW_LOG_CRITICAL << std::format("Created default user. Login: {}", DEFAULT_USERNAME);
        }
    }
}

void Database::update_db_info(const SQLite::Database& database, const uint64_t project_version, const int db_version)
{
    SQLite::Statement query(database, "UPDATE db_info SET project_version = :project_version, db_version = :db_version");
    query.bind(":project_version", std::to_string(project_version));
    query.bind(":db_version",      std::to_string(db_version));
    query.exec();
}

void Database::apply_migrations(SQLite::Database& database)
{
    init_db_info(database);

    int current_db_version = database.execAndGet("SELECT db_version FROM db_info").getInt();
    uint64_t stored_project_version = database.execAndGet("SELECT project_version FROM db_info").getInt64();
    // CROW_LOG_INFO << std::format("Stored project version: {}", stored_project_version);
    // CROW_LOG_INFO << std::format("Current DB version: {}", current_db_version);
    // CROW_LOG_INFO << std::format("Current PROJECT_VERSION: {}", COMBINED_PROJECT_VERSION);

    if (stored_project_version != COMBINED_PROJECT_VERSION) {
        // CROW_LOG_WARNING << "Version mismatch: applying migrations...";
        for (const auto& entry : migrations) {
            if (entry.version > current_db_version) {
                try {
                    SQLite::Transaction transaction(database);
                    entry.migration(database);
                    // Update the db_info table for each migration applied
                    current_db_version = entry.version;
                    update_db_info(database, COMBINED_PROJECT_VERSION, current_db_version);
                    transaction.commit();
                    // CROW_LOG_INFO << std::format("Applied migration version {}", entry.version);
                } catch (const std::exception& ex) {
                    // CROW_LOG_CRITICAL << std::format("Error applying migration {}:{}", entry.version, ex.what());
                    throw;
                }
            }
        }
    } else {
        // CROW_LOG_INFO << "Database is up-to-date.";
    }
}

SQLite::Database Database::open(const std::string &path)
{
    SQLite::Database database(path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    apply_migrations(database);
    check_users(database);
    return database;
}

std::optional<User> Database::find_user(const SQLite::Database &database, const std::string &username)
{
    try {
        SQLite::Statement query(database, "SELECT * FROM user WHERE username = :username");
        query.bind(":username", username);

        while (query.executeStep()) {
            return User{
                .id = query.getColumn("id").getInt(),
                .username = query.getColumn("username").getString(),
                .password = query.getColumn("password").getString(),
                .force_pass_change = static_cast<bool>(query.getColumn("force_pass_change").getInt()),
                .permissions = query.getColumn("permissions").getInt()
            };
        }
        return {};
    } catch (const SQLite::Exception& e) {
        // CROW_LOG_ERROR << "SQLite exception: " << e.what();
        throw std::runtime_error(std::format("SQLite exception: {}", e.what()));
    }
}

// std::string Database::generate_password()
// {
//     std::string result;
//
//     for (int i = 0; i < PASSWORD_GENERATOR_LENGTH; ++i) {
//         result.append(PASSWORD_GENERATOR_DICTIONARY, rand() % PASSWORD_GENERATOR_DICTIONARY.length(), 1);
//     }
//
//     return result;
// }
