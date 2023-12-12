#include "sqlite-abi.h"
#include "json.hpp"
#include <string>

sqlite3* sqlite3_abi_init(const char* path) {
    sqlite3* db;
    const auto result = sqlite3_open(path, &db);
    if (result != SQLITE_OK) {
        return nullptr;
    }
    return db;
}

const char* sqlite3_abi_exec(sqlite3* db, const char* sql) {
    nlohmann::json resultArray = nlohmann::json::array();
    auto state = sqlite3_exec(db, sql, [](void* data, int argc, char** argv, char** colName) -> int {
        auto row = nlohmann::json::object();
        for (int i = 0; i < argc; i++) {
            row[colName[i]] = argv[i] ? argv[i] : "NULL";
        }
        auto result = static_cast<nlohmann::json *>(data);
        result->push_back(row);
        return 0;
    }, nullptr, nullptr);

    if (state != SQLITE_OK) {
        nlohmann::json errorJson = {
            {"code", SQLITE_ERROR},
            {"message", "Error executing query: " + std::string(sqlite3_errmsg(db))},
            {"list", nlohmann::json::array()}
        };
        const auto errorStr = errorJson.dump();
        const auto resultStr = new char[errorStr.size() + 1];
        std::strcpy(resultStr, errorStr.c_str());
        return resultStr;
    }

    nlohmann::json successJson = {
        {"code", SQLITE_OK},
        {"message", "Query executed successfully"},
        {"list", resultArray}
    };
    const std::string successStr = successJson.dump();
    const auto resultStr = new char[successStr.size() + 1];
    std::strcpy(resultStr, successStr.c_str());
    return resultStr;
}
