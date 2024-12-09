#pragma once
#include<string>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<dxgidebug.h>
#include<dxcapi.h>
#include<wrl.h>
#include<vector>
#include"DirectXCommon.h"

#include"externals/DirectXTex/DirectXTex.h"
class TextureManager
{
public:
	static TextureManager* GetInstance();
	uint32_t GetTextureIndexByFilePath(const std::string& filePath);
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandleGPU(uint32_t textureIndex);
	//終了
	void Finalize();

	void Initialize(DirectXCommon* dxcCommon);
	//テクスチャ読み込み	
	void LoadTexture(const std::string& filePath);
private:
	//テクスチャ1枚分のデータ
	struct  TextureData
	{
		std::string filePath;

		DirectX::TexMetadata metaData;
		Microsoft::WRL::ComPtr<ID3D12Resource>resource;
		
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};
	DirectXCommon* dxcCommon_ = nullptr;
	static TextureManager* instance;
	TextureManager()= default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = delete;
	TextureManager& operator=(TextureManager&) = delete;

	static uint32_t kSRVIndexTop;

	// テクスチャデータ
		std::vector<TextureData>textureDatas;
		

};

