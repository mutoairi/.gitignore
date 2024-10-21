#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "Windows.h"
#include <wrl.h>

class Input
{
public :
	void Initialize(HINSTANCE,HWND);
	void Update();
private:
	
	Microsoft::WRL::ComPtr<IDirectInput8>directInput = nullptr;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard;
};

