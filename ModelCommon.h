#pragma once
#include"DirectXCommon.h"
class ModelCommon
{
public:
	void Initialize(DirectXCommon* dxcCommon);
	DirectXCommon* GetDxCommon()const { return dxcCommon_; }
private:
	DirectXCommon* dxcCommon_;
};

