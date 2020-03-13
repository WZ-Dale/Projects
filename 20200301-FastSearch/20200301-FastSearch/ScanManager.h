#pragma once

#include "Common.h"
#include "DataManager.h"

class ScanManager 
{
public:
	ScanManager() {
		_datamgr.Init();
	}
	void Scan(const string& path);

private:
	DataManager _datamgr;
};
