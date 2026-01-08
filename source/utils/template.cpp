#include "template.hpp"
#include <fstream>
#include <sstream>


std::string renderTemplateFromFile(const std::string &path, const std::unordered_map<std::string, std::string> &ctx) {
    std::ifstream in(path);
    if (!in) return "";
    std::stringstream ss;
    ss << in.rdbuf();
    std::string out = ss.str();


    for (const auto &p : ctx) {
        std::string needle = "{{" + p.first + "}}";
        size_t pos = 0;
        while ((pos = out.find(needle, pos)) != std::string::npos) {
            out.replace(pos, needle.length(), p.second);
            pos += p.second.length();
        }
    }
    return out;
}