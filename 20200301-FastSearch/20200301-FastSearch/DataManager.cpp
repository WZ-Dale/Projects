// 数据模块
#include "DataManager.h"

// DDL 数据定义语言，用来维护存储数据的结构代表指令：create，drop，alter
// DML 数据操纵语言，用来对数据进行操作代表指令：insert，delete，update
//     DML中又单独分了一个DQL，数据查询语言，代表指令：select
// DCL 数据控制语言，主要负责权限管理和事务代表指令：grant，revoke，commit

void SqliteManager::Open(const string& path) {
	int ret = sqlite3_open(path.c_str(), &_db);
	if (ret == SQLITE_OK) {		// 如果打开 成功
		TRACE_LOG("sqlite3_open success\n");
	}
	else {						// 如果打开 失败
		ERROE_LOG("sqlite3_open\n");
	}
}
void SqliteManager::Close() {
	int ret = sqlite3_close(_db);
	if (ret == SQLITE_OK) {		// 如果关闭 成功
		TRACE_LOG("sqlite3_close success\n");
	}
	else {						// 如果关闭 失败
		ERROE_LOG("sqlite3_close\n");
	}
}
void SqliteManager::ExecuteSql(const string& sql) {
	char* errmsg;		// 存储错误信息
	int ret = sqlite3_exec(_db, sql.c_str(), nullptr, nullptr, &errmsg);
	if (ret == SQLITE_OK) {		// 如果执行数据库语句 成功
		TRACE_LOG("sqlite3_exec(%s) success\n", sql.c_str());
	}
	else {						// 如果执行数据库语句 失败
		ERROE_LOG("sqlite3_exec(%s) errmsg:%s\n", sql.c_str(), errmsg);
		sqlite3_free(errmsg);
	}
}
void SqliteManager::GetTable(const string& sql, int& row, int& col, char**& ppRet) {
	char* errmsg;		// 存储错误信息
	int ret = sqlite3_get_table(_db, sql.c_str(), &ppRet, &row, &col, &errmsg);
	if (ret == SQLITE_OK) {		// 如果执行数据库语句 成功
		TRACE_LOG("sqlite3_get_table(%s) success\n", sql.c_str());
	}
	else {						// 如果执行数据库语句 失败
		ERROE_LOG("sqlite3_get_table(%s) errmsg:%s\n", sql.c_str(), errmsg);
		sqlite3_free(errmsg);
	}
}
