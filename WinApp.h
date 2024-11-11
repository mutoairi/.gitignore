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

	
	//クライアント領域のサイズ
	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;
	HWND hwndGetter() const{ return hwnd; }
	HINSTANCE hInstanceGetter() const{ return wc.hInstance; }
private:
	HRESULT hr;
	HWND hwnd = nullptr;
	WNDCLASS wc{};
};

