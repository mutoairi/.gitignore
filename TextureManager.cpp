#include "TextureManager.h"

TextureManager* TextureManager::instance = nullptr;
uint32_t TextureManager::kSRVIndexTop=1;

TextureManager* TextureManager::GetInstance()
{
    if (instance == nullptr) {
        instance = new TextureManager;
    }
    return instance;
}

uint32_t TextureManager::GetTextureIndexByFilePath(const std::string& filePath)
{
	//読み込みテクスチャを検索
	auto it = std::find_if(
		textureDatas.begin(),
		textureDatas.end(),
		[&](TextureData& textureData) {
			return textureData.filePath == filePath;
		}
	);
	if (it != textureDatas.end()) {
		uint32_t textureIndex = static_cast<uint32_t>(std::distance(textureDatas.begin(), it));
		return textureIndex;
	}
	assert(0);
	return 0;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSRVHandleGPU(uint32_t textureIndex)
{
	//範囲外指定違反チェック
	assert(textureIndex < textureDatas.size());

	TextureData& textureData = textureDatas[textureIndex];
	return textureData.srvHandleGPU;
}

void TextureManager::Finalize()
{
    delete instance;
    instance = nullptr;
}

void TextureManager::Initialize(DirectXCommon* dxcCommon)
{
	dxcCommon_ = dxcCommon;
    //SRVの数と同数
    textureDatas.reserve(DirectXCommon::kMaxSRVCount);
}

void TextureManager::LoadTexture(const std::string& filePath)
{
	//読み込みテクスチャを検索
	auto it = std::find_if(
		textureDatas.begin(),
		textureDatas.end(),
		[&](TextureData& textureData) {
			return textureData.filePath == filePath;
		}
	);
	if (it != textureDatas.end()) {
		//読み込み済みなら早期リターン
		return;
	}
	//テクスチャ枚数上限チェック
	assert(textureDatas.size() + kSRVIndexTop < DirectXCommon::kMaxSRVCount);
	


	//テクスチャファイルを読んでプログラムを扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = StringUtility::ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	
	assert(SUCCEEDED(hr));

	//ミップマップの作成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));


	//テクスチャデータを追加
	textureDatas.resize(textureDatas.size() + 1);
	TextureData& textureData = textureDatas.back();

	textureData.filePath = filePath;
	textureData.metaData = mipImages.GetMetadata();
	textureData.resource = dxcCommon_->CreateTextureResource(textureData.metaData);
	textureData.intermediateResource=dxcCommon_->UploadTextureData(textureData.resource, mipImages);

	dxcCommon_->ReturnCommand();
	
	//SRVINdex
	uint32_t srvIndex = static_cast<uint32_t>(textureDatas.size() - 1 + kSRVIndexTop);

	textureData.srvHandleCPU = dxcCommon_->GetSRVCPUDescriptorHandle(srvIndex);
	textureData.srvHandleGPU = dxcCommon_->GetSRVGPUDescriptorHandle(srvIndex);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureData.metaData.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(textureData.metaData.mipLevels);

	dxcCommon_->GetDevice()->CreateShaderResourceView(textureData.resource.Get(), &srvDesc, textureData.srvHandleCPU);
}
