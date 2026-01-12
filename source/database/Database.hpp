//
// Created by nyjako on 02.07.25.
//

#ifndef DATABASE_HPP
#define DATABASE_HPP

#pragma once

#include <optional>
#include <string>
#include <SQLiteCpp/SQLiteCpp.h>

struct User {
    int id;
    std::string username;
    std::string password;
    bool force_pass_change;
    int permissions;
};

class Database {
public:
    /// Open or creates database
    /// @param path Path to the database file
    /// @return Database connector
    static SQLite::Database open(const std::string &path);

    /// Search for user by username
    /// @param database Database connector
    /// @param username Username to search for
    /// @return Pointer to user strucutre or nullptr
    static std::optional<User> find_user(const SQLite::Database &database, const std::string &username);

    static void update_db_info(const SQLite::Database& database, uint64_t project_version, int db_version);
    static void apply_migrations(SQLite::Database& database);
    static void init_db_info(SQLite::Database& database);
    static void check_users(SQLite::Database& database);
};

#endif //DATABASE_HPP
