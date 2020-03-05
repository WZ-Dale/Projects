// 扫描模块
#include "ScanManager.h"

void ScanManager::Scan(const string& path) {
	// 比对  本地文件 和 数据库
	vector<string> localdirs;
	vector<string> localfiles;
	DirectoryList(path, localdirs, localfiles);

	// 在集合里，即排序
	set<string> localset;
	localset.insert(localdirs.begin(), localdirs.end());
	localset.insert(localfiles.begin(), localfiles.end());

	set<string> dbset;
	//DataManager();

	auto localit = localset.begin();
	auto dbit = dbset.begin();
	while (localit != localset.end() && dbit != dbset.end()) {
		if (*localit < *dbit) {		// 本地有，数据库没有，数据库新增

			++localit;
		}
		else if (*localit > *dbit) {		// 本地没有，数据库有，数据库删除

			++dbit;
		}
		else {		// 本地和数据库一致，数据不变
			++localit;
			++dbit;
		}
	}
	while (localit != localset.end()) {
		// 数据库新增
		++localit;
	}
	while (dbit != dbset.end()) {
		// 数据库删除
		++dbit;
	}
	// 递归比对子目录数据
	for (const auto& subdirs : localdirs) {
		//Scan(subdirs);
	}
}

