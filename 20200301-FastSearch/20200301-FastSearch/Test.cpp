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
	//ScanManager scanmgr;
	//scanmgr.Scan("E:\\Projects\\20200301-FastSearch\\20200301-FastSearch\\Test");
}

void TestSearch() {
	//ScanManager scanmgr;
	//scanmgr.Scan("E:\\Projects\\20200301-FastSearch\\20200301-FastSearch\\Test");
	ScanManager::CreateInstance();

	//DataManager datamgr;
	//datamgr.Init();
	string key;
	vector<std::pair<string, string>> docinfos;
	cout << "==============================================================================================" << endl;
	cout << "=================================== 快 --- 速 --- 搜 --- 索 ==================================" << endl;
	cout << "==============================================================================================" << endl;
	cout << "请输入要搜索的关键字：";
	while (cin >> key) {
		docinfos.clear();
		DataManager::GetInstance()->Search(key, docinfos);
		printf("%-30s %-50s\n", "名称", "路径");
		cout << "----------------------------------------------------------------------------------------------" << endl;
		for (const auto& e : docinfos) {
			//cout << e.first << '\t' << '\t' << '\t' << e.second << endl;
			//printf("%-30s %-50s\n", e.first.c_str(), e.second.c_str());
			const string& name = e.first;
			const string& path = e.second;
			string prefix, highlight, suffix;
			DataManager::GetInstance()->SplitHighLight(key, name.c_str(), prefix, highlight, suffix);
			cout << prefix;
			ColourPrintf(highlight.c_str());
			cout << suffix;
			for (size_t i = name.size(); i < 30; ++i) {		//补齐空格
				cout << " ";
			}
			printf("%-50s\n", path.c_str());
		}
		cout << "==============================================================================================" << endl;
		cout << "请输入要搜索的关键字：";
	}
}
void TestPinyin() {
	ColourPrintf("我是谁 who am i pinyin\n");
	cout << ChineseConvertPinYinAllSpell("我是谁 who am i pinyin") << endl;
	cout << ChineseConvertPinYinInitials("我是谁 who am i pinyin") << endl;
}
void TestHighlight() {
	//1.key是汉字->高亮对应汉字
	{
		string key = "优秀";
		string str = "你是那么优秀的人";
		size_t pos = str.find(key);
		string prefix, suffix;
		if (pos != string::npos) {
			prefix = str.substr(0, pos);
			suffix = str.substr(pos + key.size(), string::npos);
			cout << prefix;
			ColourPrintf(key.c_str());
			cout << suffix << endl;
		}
		else {
			cout << "无匹配内容" << endl;
		}
	}
	//2.key是拼音->高亮对应汉字
	{
		string key = "youxiu";
		string str = "你是那么优秀的人";
		string key_pinyin = ChineseConvertPinYinAllSpell(key);
		string str_pinyin = ChineseConvertPinYinAllSpell(str);
		size_t pos = str_pinyin.find(key_pinyin);
		string prefix, highlight, suffix;
		if (pos != string::npos) {
			size_t str_i = 0, str_j = 0, py_i = 0;
			while (py_i < pos) {
				char chinese[3] = { 0 };
				chinese[0] = str[str_i];
				chinese[1] = str[str_i + 1];
				string ch_pinyin = ChineseConvertPinYinAllSpell(chinese);
				py_i += ch_pinyin.size();
				str_i += 2;
			}
			prefix = str.substr(0, str_i);		// 第一段
			str_j = str_i;
			while (py_i < pos + key_pinyin.size()) {
				char chinese[3] = { 0 };
				chinese[0] = str[str_j];
				chinese[1] = str[str_j + 1];
				string ch_pinyin = ChineseConvertPinYinAllSpell(chinese);
				py_i += ch_pinyin.size();
				str_j += 2;
			}
			highlight = str.substr(str_i, str_j - str_i);		// 匹配段
			suffix = str.substr(str_j, string::npos);		// 第三段
			cout << prefix;
			ColourPrintf(highlight.c_str());
			cout << suffix << endl;
		}
		else {
			cout << "无匹配内容" << endl;
		}
 	}
	//2.key是拼音->高亮对应汉字（解决汉字英文混合问题）
	{
		string key = "youxiu";
		string str = "a你是b那c么优秀d的人e";
		string key_pinyin = ChineseConvertPinYinAllSpell(key);
		string str_pinyin = ChineseConvertPinYinAllSpell(str);
		size_t pos = str_pinyin.find(key_pinyin);
		string prefix, highlight, suffix;
		if (pos != string::npos) {
			size_t str_i = 0, str_j = 0, py_i = 0;
			while (py_i < pos) {
				if (str[str_i] >= 0 && str[str_i] <= 127) {	//如果是ASCII字符
					++str_i;
					++py_i;
				}
				else{
					char chinese[3] = { 0 };
					chinese[0] = str[str_i];
					chinese[1] = str[str_i + 1];
					string ch_pinyin = ChineseConvertPinYinAllSpell(chinese);
					py_i += ch_pinyin.size();
					str_i += 2;
				}
			}
			prefix = str.substr(0, str_i);		// 第一段
			str_j = str_i;
			while (py_i < pos + key_pinyin.size()) {
				if (str[str_j] >= 0 && str[str_j] <= 127) {	//如果是ASCII字符
					++str_j;
					++py_i;
				}
				else {
					char chinese[3] = { 0 };
					chinese[0] = str[str_j];
					chinese[1] = str[str_j + 1];
					string ch_pinyin = ChineseConvertPinYinAllSpell(chinese);
					py_i += ch_pinyin.size();
					str_j += 2;
				}
			}
			highlight = str.substr(str_i, str_j - str_i);		// 匹配段
			suffix = str.substr(str_j, string::npos);		// 第三段
			cout << prefix;
			ColourPrintf(highlight.c_str());
			cout << suffix << endl;
		}
		else {
			cout << "无匹配内容" << endl;
		}
	}

	//3.key是拼音首字母->高亮对应汉字
	{
		string key = "yx";
		string str = "你是那么优秀的人";
		string key_py_initials = ChineseConvertPinYinInitials(key);
		string str_py_initials = ChineseConvertPinYinInitials(str);
		size_t pos = str_py_initials.find(key_py_initials);
		string prefix, highlight, suffix;
		if (pos != string::npos) {
		//	size_t str_i = 0, str_j = 0, py_i = 0;
		//	while (py_i < pos) {
		//		char chinese[3] = { 0 };
		//		chinese[0] = str[str_i];
		//		chinese[1] = str[str_i + 1];
		//		string ch_py_initials = ChineseConvertPinYinInitials(chinese);
		//		py_i += ch_py_initials.size();
		//		str_i += 2;
		//	}
		//	prefix = str.substr(0, str_i);		// 第一段
		//	str_j = str_i;
		//	while (py_i < pos + key_py_initials.size()) {
		//		char chinese[3] = { 0 };
		//		chinese[0] = str[str_j];
		//		chinese[1] = str[str_j + 1];
		//		string ch_py_initials = ChineseConvertPinYinInitials(chinese);
		//		py_i += ch_py_initials.size();
		//		str_j += 2;
		//	}
		//	highlight = str.substr(str_i, str_j - str_i);		// 匹配段
		//	suffix = str.substr(str_j, string::npos);		// 第三段

			prefix = str.substr(0, pos * 2);		// 第一段
			highlight = str.substr(pos * 2, key_py_initials.size() * 2);		// 匹配段
			suffix = str.substr(pos * 2 + key_py_initials.size() * 2, string::npos);		// 第三段
			cout << prefix;
			ColourPrintf(highlight.c_str());
			cout << suffix << endl;
		}
		else {
			cout << "无匹配内容" << endl;
		}
	}
	//3.key是拼音首字母->高亮对应汉字（解决汉字英文混合问题）
	{
		string key = "yx";
		string str = "a你是b那c么优秀d的人e";
		string key_py_initials = ChineseConvertPinYinInitials(key);
		string str_py_initials = ChineseConvertPinYinInitials(str);
		size_t pos = str_py_initials.find(key_py_initials);
		string prefix, highlight, suffix;
		if (pos != string::npos) {
			size_t str_i = 0, str_j = 0, py_i = 0;
			while (py_i < pos) {
				if (str[str_i] >= 0 && str[str_i] <= 127) {	//如果是ASCII字符
					++str_i;
					++py_i;
				}
				else {
					char chinese[3] = { 0 };
					chinese[0] = str[str_i];
					chinese[1] = str[str_i + 1];
					string ch_py_initials = ChineseConvertPinYinInitials(chinese);
					py_i += ch_py_initials.size();
					str_i += 2;
				}
			}
			prefix = str.substr(0, str_i);		// 第一段
			str_j = str_i;
			while (py_i < pos + key_py_initials.size()) {
				if (str[str_i] >= 0 && str[str_i] <= 127) {	//如果是ASCII字符
					++str_i;
					++py_i;
				}
				else {
					char chinese[3] = { 0 };
					chinese[0] = str[str_j];
					chinese[1] = str[str_j + 1];
					string ch_py_initials = ChineseConvertPinYinInitials(chinese);
					py_i += ch_py_initials.size();
					str_j += 2;
				}
			}
			highlight = str.substr(str_i, str_j - str_i);		// 匹配段
			suffix = str.substr(str_j, string::npos);		// 第三段
			cout << prefix;
			ColourPrintf(highlight.c_str());
			cout << suffix << endl;
		}
		else {
			cout << "无匹配内容" << endl;
		}
	}
}
void TestIsChinese() {
	string temp = "125可口a可乐efg";
	int i = 0;
	while (i != temp.size())
	{
		if (temp[i] >= 0 && temp[i] <= 127)
		{
			cout << temp.substr(i, 1) << endl;
			++i;
		}
		else
		{
			cout << "          " << temp.substr(i, 2) << endl;
			i += 2;
		}
	}
}

int main() {
	//TestDirectoryList();
	//TestSqlite();
	//TestScanManager();
	TestSearch();
	//TestPinyin();
	//TestHighlight();
	//TestIsChinese();
	return 0;
}
