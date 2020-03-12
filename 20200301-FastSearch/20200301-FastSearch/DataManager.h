#pragma once

#include "Common.h"

// SqliteManager是对Sqlite的接口进行一层简单的封装。
class SqliteManager
{
public:
	SqliteManager()
		:_db(nullptr)
	{}
	~SqliteManager()
	{
		Close();
	}
	void Open(const string& path);
	void Close();
	void ExecuteSql(const string& sql);
	void GetTable(const string& sql, int& row, int& col, char**& ppRet);

	SqliteManager(const SqliteManager&) = delete;
	SqliteManager& operator=(const SqliteManager&) = delete;
private:
	sqlite3* _db;	// 数据库对象
};
// RAII
class AutoGetTable
{
public:
	AutoGetTable(SqliteManager& sm, const string& sql, int& row, int& col, char**& ppRet) {
		sm.GetTable(sql, row, col, ppRet);
		_ppRet = ppRet;
	}
	~AutoGetTable() {
		sqlite3_free_table(_ppRet);
	}
	AutoGetTable(const AutoGetTable&) = delete;
	AutoGetTable& operator=(const AutoGetTable&) = delete;
private:
	char** _ppRet;
};
