#include "sqlite-abi.h"
#include "json.hpp"
#include <string>

sqlite3 *sqlite3_abi_init(const char *path) {
    sqlite3 *db;
    const auto result = sqlite3_open(path, &db);
    if (result != SQLITE_OK && db == nullptr) {
        return nullptr;
    }
    return db;
}

const char *sqlite3_abi_exec(sqlite3 *db, const char *sql) {
    nlohmann::json resultArray = nlohmann::json::array();
    auto state = sqlite3_exec(db, sql, [](void *data, int argc, char **argv, char **colName) -> int {
        auto row = nlohmann::json::object();
        for (int i = 0; i < argc; i++) {
            row[colName[i]] = argv[i] ? argv[i] : "NULL";
        }
        auto result = static_cast<nlohmann::json *>(data);
        result->push_back(row);
        return 0;
    }, nullptr, nullptr);

    if (state != SQLITE_OK) {
        return "";
    }
    const std::string successStr = resultArray.dump();
    const auto resultStr = new char[successStr.size() + 1];
    std::memcpy(resultStr, successStr.c_str(), successStr.size());
    return resultStr;
}

int sqlite3_abi_close(sqlite3 *db) {
    return sqlite3_close(db);
}

const char *sqlite3_abi_errmsg(sqlite3 *db) {
    return sqlite3_errmsg(db);
}

int sqlite3_abi_errcode(sqlite3 *db) {
    return sqlite3_errcode(db);
}
