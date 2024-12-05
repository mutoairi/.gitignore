#pragma once
#include"Windows.h"
#include<cstdint>
#include<string>

namespace StringUtility {
	//stringをwstringに変換する
	std::wstring ConvertString(const std::string& str);
	//wstring をstringに変換する
	std::string ConvertString(const std::wstring& str);
}
