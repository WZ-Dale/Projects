// 公共模块
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <set>

#include <io.h>
#include "sqlite-amalgamation-3280000/sqlite3.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::set;

// 目录列举
static void DirectoryList(const string& path, vector<string>& dirs, vector<string>& files) {
	_finddata_t file;	//	文件信息结构体
	const string path_ = path + "\\*.*";	// 该目录下的所有文件
	intptr_t handle = _findfirst(path_.c_str(), &file);
	if (handle == -1) {
		cout << "_findfirst:" << path << "error" << endl;
		return;
	}
	do {
		if (file.attrib & _A_SUBDIR) {		// 如果当前文件是一个目录
			if ((strcmp(file.name, ".") != 0) && (strcmp(file.name, "..") != 0)) {
				string subpath = path + "\\" + file.name;
				dirs.push_back(file.name);
			}
		}
		else {		// 当前文件是一个纯文件
			files.push_back(file.name);
		}
	} while (_findnext(handle, &file) == 0);	//	右兄弟
}

////////////////////////日志模块（win和Linux都可以跑的）/////////////////////////
////////////////////////  用于记录错误，打印错误信息的  /////////////////////////
static std::string GetFileName(const std::string& path)
{
	char ch = '/';
#ifdef _WIN32
	ch = '\\';
#endif
	size_t pos = path.rfind(ch);
	if (pos == std::string::npos)
		return path;
	else
		return path.substr(pos + 1);
}
//用于调试追溯的trace log
inline static void __TraceDebug(const char* filename, int line, const char* function, const
	char* format, ...)
{
#ifdef __TRACE__
	//输出调用函数的信息
	fprintf(stdout, "[TRACE][%s:%d:%s]:", GetFileName(filename).c_str(), line, function);
	//输出用户打的trace信息
	va_list args;
	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);
	fprintf(stdout, "\n");
#endif
}

inline static void __ErrorDebug(const char* filename, int line, const char* function, const
	char* format, ...)
{
#ifdef __DEBUG__
	//输出调用函数的信息
	fprintf(stdout, "[ERROR][%s:%d:%s]:", GetFileName(filename).c_str(), line, function);
	//输出用户打的trace信息
	va_list args;
	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);
	fprintf(stdout, " errmsg:%s, errno:%d\n", strerror(errno), errno);
#endif
}

// 记录日常追述信息，三个点表示可变参数
#define TRACE_LOG(...) \
 __TraceDebug(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
// 记录日常错误信息
#define ERROE_LOG(...) \
 __ErrorDebug(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
