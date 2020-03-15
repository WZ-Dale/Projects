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
	assert(_db);
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
	assert(_db);
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
////////////////////////////////////////////////////////
void DataManager::Init() {
	_dbmgr.Open(DB_NAME);
	char createtb_sql[256];
	sprintf(createtb_sql, "create table if not exists %s (id integer primary key, path text, name text, name_pinyin text, name_initials text)", TB_NAME);	// 完整地建表
	_dbmgr.ExecuteSql(createtb_sql);
}
void DataManager::GetDoc(const string& path, set<string>& dbset) {
	char query_sql[256];
	sprintf(query_sql, "select name from %s where path = '%s'", TB_NAME, path.c_str());
	int row, col;
	char** ppRet;
	AutoGetTable agt(_dbmgr, query_sql, row, col, ppRet);
	for (int i = 1; i <= row; ++i) {
		for (int j = 0; j < col; ++j) {
			dbset.insert(ppRet[i * col + j]);
		}
	}
}
void DataManager::InsertDoc(const string& path, const string& name) {
	char insert_sql[256];
	string pinyin = ChineseConvertPinYinAllSpell(name);
	string initials = ChineseConvertPinYinInitials(name);
	sprintf(insert_sql, "insert into %s (path, name, name_pinyin, name_initials) values('%s', '%s', '%s', '%s')", TB_NAME, path.c_str(), name.c_str(), pinyin.c_str(), initials.c_str());
	_dbmgr.ExecuteSql(insert_sql);
}
void DataManager::DeleteDoc(const string& path, const string& name) {
	char delete_sql[256];
	sprintf(delete_sql, "delete from %s where path = '%s' and name = '%s'", TB_NAME, path.c_str(), name.c_str());
	_dbmgr.ExecuteSql(delete_sql);
	// 注意：此处若文件系统中删除的目录含有下级文件，数据库中也需要将下级文件删除
	string path_ = path;
	path_ += '\\';
	path_ += name;
	sprintf(delete_sql, "delete from %s where path like '%s%%'", TB_NAME, path_.c_str());
	_dbmgr.ExecuteSql(delete_sql);
}
void DataManager::Search(const string& key, vector<std::pair<string, string>>& docinfos) {
	char search_sql[256];
	sprintf(search_sql, "select name, path from %s where name like '%%%s%%'", TB_NAME, key.c_str());
	int row, col;
	char** ppRet;
	AutoGetTable agt(_dbmgr, search_sql, row, col, ppRet);
	for (int i = 1; i <= row; ++i) {
		docinfos.push_back(std::make_pair(ppRet[i * col], ppRet[i * col + 1]));
	}
}
