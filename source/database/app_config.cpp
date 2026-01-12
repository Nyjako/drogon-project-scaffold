//
// Created by nyjako on 03.07.25.
//

#include "app_config.hpp"
#include <SQLiteCpp/SQLiteCpp.h>
#include <stdexcept>

static AppConfig global_config;
static bool config_loaded = false;

void load_config_from_db_once(const SQLite::Database &db)
{
    if (config_loaded) return;

    SQLite::Statement query(db, "SELECT name, data FROM app_setting");

    while (query.executeStep()) {
        set_config_value(db, query.getColumn(0).getString(), query.getColumn(1).getString());
    }

    config_loaded = true;
}

void set_config_value(const SQLite::Database& db, const std::string& name, const std::string& value) {
    // Update in memory
    if (name == "default_expiration_hours") {
        global_config.default_expiration_hours = std::stoi(value);
    } else if (name == "max_expiration_days") {
        global_config.max_expiration_days = std::stoi(value);
    } else if (name == "enable_one_time_links") {
        global_config.enable_one_time_links = value == "true";
    } else if (name == "cleanup_interval_minutes") {
        global_config.cleanup_interval_minutes = std::stoi(value);
    } else if (name == "admin_session_timeout_secs") {
        global_config.admin_session_timeout_secs = std::stoi(value);
    } else if (name == "log_retention_days") {
        global_config.log_retention_days = std::stoi(value);
    } else if (name == "maintenance_mode") {
        global_config.maintenance_mode = value == "true";
    } else if (name == "session_expiration_days") {
        global_config.session_expiration_days = std::stoi(value);
    } else if (name == "password_policy") {
        throw std::runtime_error("Unimplemented");
        // const auto json = crow::json::load(value);
        // if (!json) throw std::runtime_error("Invalid JSON for password_policy");
        //
        // if (json.has("min_len")) global_config.password_policy.min_len = static_cast<int>(json["min_len"].i());
        // if (json.has("require_lower")) global_config.password_policy.require_lower = json["require_lower"].b();
        // if (json.has("require_digit_upper")) global_config.password_policy.require_digit_upper = json["require_digit_upper"].b();
        // if (json.has("require_digit")) global_config.password_policy.require_digit = json["require_digit"].b();
    } else {
        throw std::runtime_error("Unknown config field: " + name);
    }

    // Update in database
    SQLite::Statement update(db, R"(
        INSERT INTO app_setting (name, data) VALUES (:name, :data)
        ON CONFLICT(name) DO UPDATE SET data = excluded.data
    )");
    update.bind(":name", name);
    update.bind(":data", value);
    update.exec();
}

const AppConfig& get_config() {
    if (!config_loaded) {
        throw std::runtime_error("Config not loaded! Call load_config_from_db_once() first.");
    }
    return global_config;
}