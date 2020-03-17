#pragma once

#include "Common.h"
#include "DataManager.h"

//整个程序只需要一个扫描模块,因此将ScanManager设计成单例
class ScanManager 	//扫描出信息,放到数据库中
{
public:
	void Scan(const string& path);
	void StartScan() {
		while (1) {
			Scan("E:\\Projects\\20200301-FastSearch\\20200301-FastSearch\\Test");
			std::this_thread::sleep_for(std::chrono::seconds(5));	// 每隔5s扫描一次
		}
	}
	static ScanManager* CreateInstance() {
		static ScanManager scanmgr;
		static std::thread thd(&StartScan, &scanmgr);	// 用scanmgr对象调StartScan函数
		thd.detach();		// 线程分离一下
		return &scanmgr;
	}
private:
	ScanManager() {
		//_datamgr.Init();
	}
	ScanManager(const ScanManager&);
	//DataManager _datamgr;
};
