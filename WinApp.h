#pragma once
#include<cstdint>
#include"Windows.h"
class WinApp
{
public:
	//初期化
	void Initialize();

	//更新
	void Update();

	//静的メンバ関数
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

};

