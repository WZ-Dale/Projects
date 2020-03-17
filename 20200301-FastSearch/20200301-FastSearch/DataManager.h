#pragma once

#include "Common.h"

//////////////////////////////////////////////////////////
// SqliteManager 是对 Sqlite的接口 进行一层简单的封装
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
	void Open(const string& path);		// 打开数据库
	void Close();						// 关闭数据库
	void ExecuteSql(const string& sql);		// 执行数据库语句
	void GetTable(const string& sql, int& row, int& col, char**& ppRet);	// 获取数据库中列表信息

	SqliteManager(const SqliteManager&) = delete;		// 屏蔽拷贝构造和赋值拷贝
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

//////////////////////////////////////////////////////////
// 数据管理模块
#define DB_NAME "doc.db"
#define TB_NAME "tb_doc"
//为了方便加锁,设计成单例模式
//一般使用饿汉模式,不用考虑线程安全问题哦
class DataManager
{
public:
	static DataManager* GetInstance() {
		static DataManager datamgr;
		datamgr.Init();
		return &datamgr;
	}

	void Init();	// 数据库初始化（打开数据库，建表）
	void GetDoc(const string& path, set<string>& dbset);	// 查找path下的所有子文档，放到set中用于比对
	void InsertDoc(const string& path, const string& name);		// 插入到数据库
	void DeleteDoc(const string& path, const string& name);		// 删除数据库中数据
	void Search(const string& key, vector<std::pair<string, string>>& docinfos);	// 快速搜索
	void SplitHighLight(const string& key, const string& name, string& prefix, string& highlight, string& suffix);	// 把需要高亮的分割出来
private:
	DataManager() {}
	SqliteManager _dbmgr;
};
