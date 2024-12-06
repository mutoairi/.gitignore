#pragma once
#include "Windows.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <array>
#include <dxcapi.h>
#include <string>
#include"externals/DirectXTex/DirectXTex.h"
#include"Logger.h"
#include"StringUtility.h"
#include"WinApp.h"

class DirectXCommon
{
public:

	void Initialize(WinApp*);

	//デスクリプタヒープ生成
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> CreateDescriptorHeap
	( D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);
	//srv専用
	//CPU
	D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUDescriptorHandle(uint32_t index);
	//GPU
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUDescriptorHandle(uint32_t index);

	//コンパイルシェーダー
	Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(const std::wstring& filePath, const wchar_t* profile);

	//リソース生成
	Microsoft::WRL::ComPtr < ID3D12Resource> CreateBufferResource( size_t sizeInBytes);
	//テクスチャ生成
	Microsoft::WRL::ComPtr < ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);
	//テクスチャ読み込み	
	static DirectX::ScratchImage LoadTexture(const std::string& filePath);
	//アップロードテクスチャ
	void UploadTextureData(Microsoft::WRL::ComPtr < ID3D12Resource> texture, const DirectX::ScratchImage& mipImages);
	//深度テクスチャ
	Microsoft::WRL::ComPtr < ID3D12Resource> CreateDepthStencilTextureResource( int32_t width, int32_t height);
	//描画前処理
	void PreDraw();
	//描画後処理
	void PostDraw();
	//getter
	ID3D12Device* GetDevice()const { return device.Get(); }
	ID3D12GraphicsCommandList* GetCommandList()const { return commandList.Get(); }

	
private:
	void DeviceInitialize();
	void CommandInitialize();
	void SwapChainInitialize();
	void DepthInitialize();
	void DescriptorHeapGenerate();
	void RTVInitialize();
	void DepthStencilInitialize();
	void FenceInitialize();
	void ViewPortInitialize();
	void ScissoringInitialize();
	void DXCCompilerInitialize();
	void ImGuiInitialize();
	//CPU
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);
	//GPU
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);
	//Direct12デバイス
	Microsoft::WRL::ComPtr < ID3D12Device> device;
	//DXGIファクトリ
	Microsoft::WRL::ComPtr<IDXGIFactory7>dxgiFactory;

	//commandQueue
	Microsoft::WRL::ComPtr < ID3D12CommandQueue> commandQueue ;
   //commandAllocator
	Microsoft::WRL::ComPtr < ID3D12CommandAllocator> commandAllocator;
	//commandList
	Microsoft::WRL::ComPtr < ID3D12GraphicsCommandList> commandList ;
	//swapChain
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	//resource
	Microsoft::WRL::ComPtr < ID3D12Resource> resource;
	//SwapChainからResourceを引っ張ってくる(2)
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, 2> swapChainResources;
	//DescriptorSize
	uint32_t descriptorSizeSRV;
	uint32_t descriptorSizeRTV;
	uint32_t descriptorSizeDSV;
	//DescriptorHeap
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> rtvDescriptorHeap;
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> srvDescriptorHeap;
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> dsvDescriptorHeap;
	///深度ステンシルリソース
	Microsoft::WRL::ComPtr <ID3D12Resource> depthStencilResource;
	//Fence
	Microsoft::WRL::ComPtr < ID3D12Fence> fence ;
	//ビューポート変換
	D3D12_VIEWPORT viewport{};
	//シザー矩形
	D3D12_RECT scissorRect{};
	//dxcCompilerを初期化
	IDxcUtils* dxcUtils ;
	IDxcCompiler3* dxcCompiler;
	//rtvdesc
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	//スワップチェーン
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	//rtvHandle
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
	//Transitionの設定
	D3D12_RESOURCE_BARRIER barrier{};
	//フェンスの値
	uint64_t fenceValue;
	//フェンスイベント
	HANDLE fenceEvent;
	//includeHandler
	IDxcIncludeHandler* includeHandler;
	

	HRESULT hr;
	//WindowsAPI
	WinApp* winApp= nullptr;

};

