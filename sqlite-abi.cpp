#include "sqlite-abi.h"
#include "json.hpp"

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
        return "";
    }

    // 将查询结果的 JSON 字符串复制到分配的内存中
    const auto jsonString = resultArray.dump();
    const auto resultCStr = static_cast<char *>(malloc(jsonString.size() + 1));
    strcpy(resultCStr, jsonString.c_str());
    return resultCStr;
}
