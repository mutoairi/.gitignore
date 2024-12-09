#pragma once
#include<Windows.h>
#include<cstdint>
#include<string>
#include<format>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<dxgidebug.h>
#include<dxcapi.h>
#include<math.h>
#include<fstream>
#include<sstream>
#include<wrl.h>
#include"Logger.h"
#include"StringUtility.h"
#include"DirectXCommon.h"

class SpriteCommon
{
public:
	void Initialize(DirectXCommon*dxcCommon);
	//共通描画
	void CommonDrawing();
	//getter
	DirectXCommon* GetDxcCommon()const { return dxcCommon_; }


private:
	void RootSignature();
	void GraphicsPipeline();

	HRESULT hr;
	DirectXCommon* dxcCommon_;

	//rootSignature
	Microsoft::WRL::ComPtr < ID3D12RootSignature> rootSignature;
	//graphicsPipeline
	Microsoft::WRL::ComPtr < ID3D12PipelineState> graphicsPipelineState;
};

