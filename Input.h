#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "Windows.h"
#include <wrl.h>

class Input
{
public:
   //namespace省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;



public :
	void Initialize(HINSTANCE,HWND);
	void Update();
	bool PushKey(BYTE );
	bool TriggerKey(BYTE);
private:
	
	
	ComPtr<IDirectInputDevice8> keyboard;
	BYTE key[256] = {};
	//前回の全キーの状態
	BYTE keyPre[256] = {};
};

