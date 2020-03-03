// 公共模块
#pragma once
#include <iostream>
#include <string>
#include <vector>

#include <io.h>

using std::cout;
using std::endl;
using std::string;
using std::vector;

// 目录列举
void DirectoryList(const string& path, vector<string>& docs) {
	_finddata_t file;	//	文件信息结构体
	const string path_ = path + "\\*.*";	// 该目录下的所有文件
	intptr_t handle = _findfirst(path_.c_str(), &file);
	if (handle == -1) {
		cout << "_findfirst:" << path << "error" << endl;
		return;
	}
	do {
		docs.push_back(file.name);
	} while (_findnext(handle, &file) == 0);	//	右兄弟
}
