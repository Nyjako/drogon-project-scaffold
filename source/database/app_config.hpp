//
// Created by nyjako on 03.07.25.
//

#ifndef APP_CONFIG_HPP
#define APP_CONFIG_HPP

#pragma once

#include <string>
#include <SQLiteCpp/SQLiteCpp.h>

#define CROW_MIDDLEWARE crow::CookieParser, CsrfMiddleware

struct PasswordPolicy {
    int min_len = 8;
    bool require_lower = true;
    bool require_digit_upper = true;
    bool require_digit = true;
};

struct AppConfig {
    int default_expiration_hours = 24;
    int max_expiration_days = 30;
    bool enable_one_time_links = true;
    int cleanup_interval_minutes = 60;
    int admin_session_timeout_secs = 1800;
    PasswordPolicy password_policy = {};
    int log_retention_days = 7;
    bool maintenance_mode = false;
    int session_expiration_days = 30;
};

// Load once at startup
void load_config_from_db_once(const SQLite::Database& db);

// Get read-only config
const AppConfig& get_config();

// Runtime setter
void set_config_value(const SQLite::Database& db, const std::string& name, const std::string& value);

#endif //APP_CONFIG_HPP
