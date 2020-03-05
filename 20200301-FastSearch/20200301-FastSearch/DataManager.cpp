// 数据模块
#include "DataManager.h"

void SqliteManager::Open(const string& path) {
	int ret = sqlite3_open(path.c_str(), &_db);
	if (ret == SQLITE_OK) {		// 如果打开成功
		TRACE_LOG("sqlite_open success\n");
	}
	else {						// 如果打开失败
		ERROE_LOG("sqlite_open\n");
	}
}
void SqliteManager::Close() {

}
void SqliteManager::ExecuteSql(const string& sql) {

}
void SqliteManager::GetTable(const string& sql, int& row, int& col, char**& ppRet) {

}

