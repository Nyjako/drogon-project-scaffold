#pragma once


#include <string>
#include <unordered_map>


// Very small template helper: text replacement of {{key}} with value.
// Not safe for untrusted content — only for simple server-side composition.
std::string renderTemplateFromFile(const std::string &path, const std::unordered_map<std::string, std::string> &ctx);