#include "Common.h"
#include "DataManager.h"
#include "ScanManager.h"

void TestDirectoryList() {
	vector<string> dirs, files;
	DirectoryList("E:\\Projects\\20200301-FastSearch", dirs, files);
	for (const auto& e : dirs) {
		cout << e << endl;
	}
	for (const auto& e : files) {
		cout << e << endl;
	}
}

void TestSqlite() {
	// 打开数据库
	SqliteManager sq;
	sq.Open("test.db");

	// 创建表
	//string exists_sql = "select count(*)  from sqlite_master where type='table' and name = 'tb_doc'"
	string createtb_sql = "create table tb_doc(id integer primary key autoincrement, doc_path text, doc_name int)";
	sq.ExecuteSql(createtb_sql);
	
	// 插入数据
	string insert_sql = "insert into tb_doc values(NULL, '路径', 'name')";
	sq.ExecuteSql(insert_sql);
	insert_sql = "insert into tb_doc values(NULL, '22', 'qq')";
	sq.ExecuteSql(insert_sql);
	insert_sql = "insert into tb_doc values(NULL, '33', 'ww')";
	sq.ExecuteSql(insert_sql);
	insert_sql = "insert into tb_doc values(NULL, '44', 'ee')";
	sq.ExecuteSql(insert_sql);

	// 查找数据
	string query_sql = "select * from tb_doc where doc_path = '33'";
	int row, col;
	char** ppRet;
	/*
	sq.GetTable(query_sql, row, col, ppRet);
	for (int i = 1; i <= row; ++i) {			// 第一行是字段名称，不取
		for (int j = 0; j < col; ++j) {
			cout << ppRet[i * col + j] << "\t";		// 由于该数据库表在内存中按一维数组存放的
		}
		cout << endl;
	}
	sqlite3_free_table(ppRet);
	*/
	// 使用智能指针进行动态内存释放
	AutoGetTable agt(sq, query_sql, row, col, ppRet);
	for (int i = 1; i <= row; ++i) {			// row,col 表示的是数据的行列，不算第一行的字段
		for (int j = 0; j < col; ++j) {
			cout << ppRet[i * col + j] << "\t";		// 由于该数据库表在内存中按一维数组存放的
		}
		cout << endl;
	}
}

void TestScanManager() {
	ScanManager scanmgr;
	scanmgr.Scan("E:\\Projects\\20200301-FastSearch\\20200301-FastSearch\\Test");
}

int main() {
	//TestDirectoryList();
	//TestSqlite();
	TestScanManager();
	return 0;
}
