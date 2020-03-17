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
	/*
	char search_sql[256];
	sprintf(search_sql, "select name, path from %s where name like '%%%s%%'", TB_NAME, key.c_str());
	int row, col;
	char** ppRet;
	AutoGetTable agt(_dbmgr, search_sql, row, col, ppRet);
	for (int i = 1; i <= row; ++i) {
		docinfos.push_back(std::make_pair(ppRet[i * col], ppRet[i * col + 1]));
	}
	*/
	char search_sql[256];
	string key_pinyin = ChineseConvertPinYinAllSpell(key);
	string key_initials = ChineseConvertPinYinInitials(key);
	sprintf(search_sql, "select name, path from %s where name_pinyin like '%%%s%%' or name_initials like '%%%s%%'", TB_NAME, key_pinyin.c_str(), key_initials.c_str());
	int row, col;
	char** ppRet;
	AutoGetTable agt(_dbmgr, search_sql, row, col, ppRet);
	for (int i = 1; i <= row; ++i) {
		docinfos.push_back(std::make_pair(ppRet[i * col], ppRet[i * col + 1]));
	}
}
void DataManager::SplitHighLight(const string& key, const string& name, string& prefix, string& highlight, string& suffix) {
	//1.key是汉字->高亮对应汉字
	{
		size_t pos = name.find(key);
		if (pos != string::npos) {
			prefix = name.substr(0, pos);
			highlight = key;
			suffix = name.substr(pos + key.size(), string::npos);

			return;
		}
	}
	//2.key是拼音->高亮对应汉字（解决汉字英文混合问题）
	//{
	//	string key_pinyin = ChineseConvertPinYinAllSpell(key);
	//	string name_pinyin = ChineseConvertPinYinAllSpell(name);
	//	size_t pos = name_pinyin.find(key_pinyin);
	//	if (pos != string::npos) {
	//		size_t name_i = 0, name_j = 0, py_i = 0;
	//		while (py_i < pos) {
	//			if (name[name_i] >= 0 && name[name_i] <= 127) {	//如果是ASCII字符
	//				++name_i;
	//				++py_i;
	//			}
	//			else {
	//				char chinese[3] = { 0 };
	//				chinese[0] = name[name_i];
	//				chinese[1] = name[name_i + 1];
	//				string ch_pinyin = ChineseConvertPinYinAllSpell(chinese);
	//				py_i += ch_pinyin.size();
	//				name_i += 2;
	//			}
	//		}
	//		prefix = name.substr(0, name_i);		// 第一段
	//		name_j = name_i;
	//		while (py_i < pos + key_pinyin.size()) {
	//			if (name[name_j] >= 0 && name[name_j] <= 127) {	//如果是ASCII字符
	//				++name_j;
	//				++py_i;
	//			}
	//			else {
	//				char chinese[3] = { 0 };
	//				chinese[0] = name[name_j];
	//				chinese[1] = name[name_j + 1];
	//				string ch_pinyin = ChineseConvertPinYinAllSpell(chinese);
	//				py_i += ch_pinyin.size();
	//				name_j += 2;
	//			}
	//		}
	//		highlight = name.substr(name_i, name_j - name_i);		// 匹配段
	//		suffix = name.substr(name_j, string::npos);		// 第三段
	
	//		return;
	//	}
	//}
	// 由于两个while的逻辑大部分相似，可以合并两个while
	{
		string key_ap = ChineseConvertPinYinAllSpell(key);	//key转拼音
		string name_ap = ChineseConvertPinYinAllSpell(name);	//原串转拼音
		size_t ap_start = name_ap.find(key_ap);	//找到拼音匹配位置
		size_t ap_end = ap_start + key_ap.size();
		size_t ap_index = 0;	//原串转成拼音中的指针（并不是指针，只是为了方便表示而已）
		size_t ht_index = 0;	//原串汉字中的指针
		size_t ht_start = 0, ht_len = 0;		//高亮长度
		if (ap_start != string::npos) {
			while (ap_index < ap_end) {
				if (ap_index == ap_start) {		// 当拼音匹配时，原串匹配，就找到了高亮的开始
					ht_start = ht_index;
				}
				if (name[ht_index] >= 0 && name[ht_index] <= 127) {	//如果是ASCII字符
					++ht_index;
					++ap_index;
				}
				else {
					char chinese[3] = { '\0' };
					chinese[0] = name[ht_index];
					chinese[1] = name[ht_index + 1];
					string py_str = ChineseConvertPinYinAllSpell(chinese);
					ap_index += py_str.size();	//跳过该汉字拼音的长度
					ht_index += 2;	//gbk汉字两个字符
				}
			}
			ht_len = ht_index - ht_start;

			prefix = name.substr(0, ht_start);
			highlight = name.substr(ht_start, ht_len);
			suffix = name.substr(ht_start + ht_len, string::npos);

			return;
		}
	}
	//3.key是拼音首字母->高亮对应汉字（解决汉字英文混合问题）
	//{
	//	string key_py_initials = ChineseConvertPinYinInitials(key);
	//	string name_py_initials = ChineseConvertPinYinInitials(name);
	//	size_t pos = name_py_initials.find(key_py_initials);
	//	if (pos != string::npos) {
	//		size_t name_i = 0, name_j = 0, py_i = 0;
	//		while (py_i < pos) {
	//			if (name[name_i] >= 0 && name[name_i] <= 127) {	//如果是ASCII字符
	//				++name_i;
	//				++py_i;
	//			}
	//			else {
	//				char chinese[3] = { 0 };
	//				chinese[0] = name[name_i];
	//				chinese[1] = name[name_i + 1];
	//				string ch_py_initials = ChineseConvertPinYinInitials(chinese);
	//				py_i += ch_py_initials.size();
	//				name_i += 2;
	//			}
	//		}
	//		prefix = name.substr(0, name_i);		// 第一段
	//		name_j = name_i;
	//		while (py_i < pos + key_py_initials.size()) {
	//			if (name[name_i] >= 0 && name[name_i] <= 127) {	//如果是ASCII字符
	//				++name_i;
	//				++py_i;
	//			}
	//			else {
	//				char chinese[3] = { 0 };
	//				chinese[0] = name[name_j];
	//				chinese[1] = name[name_j + 1];
	//				string ch_py_initials = ChineseConvertPinYinInitials(chinese);
	//				py_i += ch_py_initials.size();
	//				name_j += 2;
	//			}
	//		}
	//		highlight = name.substr(name_i, name_j - name_i);		// 匹配段
	//		suffix = name.substr(name_j, string::npos);		// 第三段

	//		return;
	//	}
	//}
	{
		string key_it = ChineseConvertPinYinInitials(key);	//key转首字母
		string name_it = ChineseConvertPinYinInitials(name);	//原串转首字母
		size_t it_start = name_it.find(key_it);
		size_t it_end = it_start + key_it.size();
		size_t it_index = 0;	//原串转成首字母中的指针（并不是指针，只是为了方便表示而已）
		size_t ht_index = 0;	//原串汉字中的指针
		size_t ht_start = 0, ht_len = 0;		//高亮长度
		if (it_start != string::npos) {
			while (it_index < it_end) {
				if (it_index == it_start) {
					ht_start = ht_index;
				}
				if (name[ht_index] >= 0 && name[ht_index] <= 127) {	//如果是ASCII字符
					++ht_index;
					++it_index;
				}
				else {
					++it_index;	//跳过该汉字首字母
					ht_index += 2;	//gbk汉字两个字符
				}
			}
			ht_len = ht_index - ht_start;

			prefix = name.substr(0, ht_start);
			highlight = name.substr(ht_start, ht_len);
			suffix = name.substr(ht_start + ht_len, string::npos);

			return;
		}
	}
	// 如果高亮没有匹配到，将原串给第一段，中间段和第三段为空就好
	TRACE_LOG("Split highlight no match. name: %s, key: %s\n", name.c_str(), key.c_str());
	prefix = name;
}
