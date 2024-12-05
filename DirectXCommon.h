#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<wrl.h>
#include<array>
#include<dxcapi.h>
#include"Logger.h"
#include"StringUtility.h"
#include"WinApp.h"

class DirectXCommon
{
public:

	void Initialize(WinApp*);

	//デスクリプタヒープ生成
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> CreateDescriptorHeap
	(Microsoft::WRL::ComPtr < ID3D12Device> device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);
	//srv専用
	//CPU
	D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUDescriptorHandle(uint32_t index);
	//GPU
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUDescriptorHandle(uint32_t index);
	
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
	Microsoft::WRL::ComPtr < IDXGISwapChain4> swapChain ;
	//resource
	Microsoft::WRL::ComPtr < ID3D12Resource> resource = nullptr;
	//SwapChainからResourceを引っ張ってくる(2)
	Microsoft::WRL::ComPtr < ID3D12Resource> swapChainResources[2] = { nullptr };
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
	//WindowsAPI
	WinApp* winApp_ = nullptr;

};

