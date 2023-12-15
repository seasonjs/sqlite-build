#ifndef SQLITE_ABI_H
#define SQLITE_ABI_H

#include "sqlite3.h"

#ifdef SQLITE_ABI_SHARED
#if defined(_WIN32) && !defined(__MINGW32__)
#ifdef SQLITE_ABI_BUILD
#define SQLITE_ABI_API __declspec(dllexport)
#else
#define SQLITE_ABI_API __declspec(dllimport)
#endif
#else
#define SQLITE_ABI_API __attribute__((visibility("default")))
#endif
#else
#define SQLITE_ABI_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

SQLITE_ABI_API sqlite3 *sqlite3_abi_init(const char *path);

SQLITE_ABI_API const char *sqlite3_abi_exec(sqlite3 *db, const char *sql);

SQLITE_ABI_API sqlite3_stmt * sqlite3_abi_prepare(sqlite3 *db, const char *zSql, int nByte);

#ifdef __cplusplus
}
#endif

#endif //SQLITE_ABI_H
