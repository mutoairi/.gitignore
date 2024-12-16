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
#include<list>
#include"Input.h"
#include"WinApp.h"
#include "DirectXCommon.h"
#include"D3DResourceLeakChecker.h"
#include"SpriteCommon.h"
#include"Sprite.h"

#include"makeMatrix.h"
#include"externals/imugui/imgui.h"
#include"externals/imugui/imgui_impl_dx12.h"
#include"externals/imugui/imgui_impl_win32.h"
#include"externals/DirectXTex/DirectXTex.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxcompiler.lib")


struct MaterialData
{
	std::string texturFilePath;
};
struct ModelData
{
	std::vector<VertexData> vertices;
	MaterialData material;
};

std::wstring ConvertString(const std::string& str) {
	if (str.empty()) {
		return std::wstring();
	}

	auto sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), NULL, 0);
	if (sizeNeeded == 0) {
		return std::wstring();
	}
	std::wstring result(sizeNeeded, 0);
	MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), &result[0], sizeNeeded);
	return result;
}

std::string ConvertString(const std::wstring& str) {
	if (str.empty()) {
		return std::string();
	}

	auto sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), NULL, 0, NULL, NULL);
	if (sizeNeeded == 0) {
		return std::string();
	}
	std::string result(sizeNeeded, 0);
	WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), sizeNeeded, NULL, NULL);
	return result;
}
void Log(const std::string& messeage) {
	OutputDebugStringA(messeage.c_str());
}





MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {
	MaterialData materialData;
	std::string line;

	std::ifstream file(directoryPath + "/" + filename);
	assert(file.is_open());

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;

			materialData.texturFilePath = directoryPath + "/" + textureFilename;
		}
	}
	return materialData;
}
ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename) {
	ModelData modelData;
	std::vector<Vector4> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> texcoords;
	std::string line;

	std::ifstream file(directoryPath + "/" + filename);
	assert(file.is_open());

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;
		if (identifier == "v") {
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.x *= -1.0f;
			position.w = 1.0f;
			positions.push_back(position);
		}else if(identifier=="vt"){
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			texcoord.x = 1.0f - texcoord.x;
			texcoord.y = 1.0f - texcoord.y;
			texcoords.push_back(texcoord);
		}else if(identifier=="vn") {
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normal.x *= -1.0f;
			normals.push_back(normal);
		}else if (identifier == "f") {

			VertexData triangle[3];
			for (uint32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;

				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/');
					elementIndices[element] = std::stoi(index);

				}
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				VertexData vertex = { position,texcoord,normal };
				modelData.vertices.push_back(vertex);
				triangle[faceVertex] = { position,texcoord,normal };
			}
			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);
		}
		else if (identifier == "mtllib") {
			std::string materialFilename;
			s >> materialFilename;
			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}
	}
	return modelData;
}


//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	CoInitializeEx(0, COINIT_MULTITHREADED);
	D3DResourceLeakChecker leakChack;
#pragma region"ウィンドウの生成"

	WinApp* winApp = nullptr;
	winApp = new WinApp();
	winApp->Initialize();
	

;

#pragma endregion

#pragma region"DXCCommonの生成"

//ポインタ
DirectXCommon* dxcCommon = nullptr;
//DirectXの初期化
dxcCommon = new DirectXCommon();
dxcCommon->Initialize(winApp);






	//ポインタ
	Input* input = nullptr;
	
	//入力の初期化
	input = new Input();
	input->Initialize(winApp);

	

	Log("HelloDIrectX!\n");

	MSG msg{};
	
	//SpriteCommon
	SpriteCommon* spriteCommon = nullptr;

	//初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxcCommon);


	//const uint32_t kSubdivision = 16;
	ModelData modelData = LoadObjFile("resources", "plane.obj");
	Microsoft::WRL::ComPtr < ID3D12Resource> vertexResource = dxcCommon->CreateBufferResource( sizeof(VertexData) * modelData.vertices.size());
	Microsoft::WRL::ComPtr < ID3D12Resource> materialResource = dxcCommon->CreateBufferResource( sizeof(Material));
	//	マテリアルにデータを書き込む
	Material* materialData = nullptr;
	//書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	//今回は赤を書き込んでみる
	materialData->color = { Vector4(1.0f, 1.0f, 1.0f, 1.0f) };
	materialData->enableLighting = true;
	materialData->uvTransform = MakeIdentity4x4();
	//wvp用のリソースを作る
	Microsoft::WRL::ComPtr < ID3D12Resource> wvpResource = dxcCommon->CreateBufferResource( sizeof(TransformationMatrix));
	//データを書き込む
	TransformationMatrix* wvpData = nullptr;
	//Matrix4x4* transformationMatrixData = nullptr;
	//書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	//単位行列を書き込んでおく
	wvpData->WVP = MakeIdentity4x4();
	wvpData->World = MakeIdentity4x4();
	//*transformationMatrixData = MakeIdentity4x4();
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ文のサイズ
	vertexBufferView.SizeInBytes =UINT( sizeof(VertexData)*modelData.vertices.size());

	//1頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	//頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	//書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData)* modelData.vertices.size());

	
	//Spriteを作成

	Sprite* sprite = nullptr;

	//初期化
	sprite = new Sprite();
	sprite->Initialize(spriteCommon);
	

	//平行光源用
	Microsoft::WRL::ComPtr < ID3D12Resource> directionalLightResource = dxcCommon->CreateBufferResource( sizeof(DirectionalLight));
	DirectionalLight* directionalLightData = nullptr;
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
	directionalLightData->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData->direction = { 0.0f,1.0f,1.0f };
	directionalLightData->intensity = 1.0f;

	
	//Textureを読んで転送する
	DirectX::ScratchImage mipImages =dxcCommon-> LoadTexture("resources/uvChecker.png");
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	Microsoft::WRL::ComPtr < ID3D12Resource> textureResource = dxcCommon->CreateTextureResource( metadata);
	dxcCommon->UploadTextureData(textureResource.Get(), mipImages);

	//2枚目のTextureを読んで転送する
	DirectX::ScratchImage mipImages2 = dxcCommon->LoadTexture(modelData.material.texturFilePath);
	const DirectX::TexMetadata& metadata2 = mipImages2.GetMetadata();
	Microsoft::WRL::ComPtr < ID3D12Resource> textureResource2 = dxcCommon->CreateTextureResource( metadata2);
	dxcCommon->UploadTextureData(textureResource2.Get(), mipImages2);
	
	//metaDataを基にSRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);


	
	//SRVを作成するDescriptorHeapの場所を決める
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU = dxcCommon->GetSRVCPUDescriptorHandle(0);
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU = dxcCommon->GetSRVGPUDescriptorHandle(0);
	//先頭はImGuiが使っているのでその次を使う
	textureSrvHandleCPU.ptr += dxcCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU.ptr += dxcCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//SRVの生成
	dxcCommon->GetDevice()->CreateShaderResourceView(textureResource.Get(), &srvDesc, textureSrvHandleCPU);

	// metaDataを基にSRV設定
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};
	srvDesc2.Format = metadata2.format;
	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc2.Texture2D.MipLevels = UINT(metadata2.mipLevels);
	//SRVを作成するDescriptorHeapの場所を決める
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU2 = dxcCommon->GetSRVCPUDescriptorHandle(2);
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU2 = dxcCommon->GetSRVGPUDescriptorHandle(2);


	dxcCommon->GetDevice()->CreateShaderResourceView(textureResource2.Get(), &srvDesc2, textureSrvHandleCPU2);


	Transform transform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Transform cameraTransform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f} };
	
	
	bool useMonsterBall = true;
	//ウィンドウのボタンが押されるまでループ
	while (msg.message != WM_QUIT) {

		

		//windowにメッセージが来てたら最優先で処理させる
		if (winApp->ProcessMessage()) {
			break;
		}
		else {

			//入力更新
			input->Update();

			if (input->TriggerKey(DIK_0)) {
				OutputDebugStringA("Hit 0\n");
			}


			ImGui_ImplDX12_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			ImGui::Begin("WIndow");
			ImGui::DragFloat3("Color", &materialData->color.x, 0.01f);
			ImGui::SliderAngle("SpherRotate", &transform.rotate.y);
			ImGui::DragFloat3("LightingColor", &directionalLightData->color.x, 0.01f);
			ImGui::DragFloat3("LightingDir", &directionalLightData->direction.x, 0.01f);
			ImGui::DragFloat("lightinyencity", &directionalLightData->intensity, 0.01f);
			ImGui::Checkbox("useMonsterBall", &useMonsterBall);
			/*ImGui::DragFloat2("UVTranslate", &uvTransformSprite.translate.x, 0.01f, -10.0f, 10.0f);
			ImGui::DragFloat2("UVScale", &uvTransformSprite.scale.x, 0.01f, -10.0f, 10.0f);
			ImGui::SliderAngle("UVRotate", &uvTransformSprite.rotate.z);*/

			// カラーパレットを縮小表示するためのflag
			static bool showColorPicker = false;

			// カラーパレットの縮小表示
			if (!showColorPicker)
			{
				if (ImGui::Button("color palette"))
					showColorPicker = true;
			}

			// カラーパレットの拡大表示
			if (showColorPicker)
			{
				ImGui::ColorPicker4("Color", (float*)&materialData->color.x, ImGuiColorEditFlags_Float);


			}


			//ゲームの処理

			Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
			Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
			Matrix4x4 viewMatrix = Inverse(cameraMatrix);
			Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.0f);
			Matrix4x4 worldviewProjectionMatrix = MatrixMultiply(worldMatrix, MatrixMultiply(viewMatrix, projectionMatrix));
			wvpData->WVP = worldviewProjectionMatrix;
			wvpData->World = worldMatrix;


			

			sprite->Update();

			
			ImGui::End();

			//ImGUi内部のコマンドを生成する
			ImGui::Render();

			//描画前処理
			dxcCommon->PreDraw();

			//Spriteの描画準備
			spriteCommon->CommonDrawing();
			
			dxcCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);//VBVを設定
			//マテリアルCBufferの場所を設定
			dxcCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
			//wvp用のCBufferの場所を設定
			dxcCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
			//SRVのDescriptorTableの先頭を設定
			dxcCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2,useMonsterBall? textureSrvHandleGPU2:textureSrvHandleGPU);
			//平行光源
			dxcCommon->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());

			//描画！(DrawCall/ドローコール)。3頂点出一つのインスタンス。インスタンスについては今後
			dxcCommon->GetCommandList()->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);

			

			
			//平行光源
			dxcCommon->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());

			//Sprite描画！(DrawCall/ドローコール)。3頂点出一つのインスタンス。インスタンスについては今後
			sprite->Draw(textureSrvHandleGPU);
			//
			ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), dxcCommon->GetCommandList());

			dxcCommon->PostDraw();



			

			//ゲーム処理

		}
	}
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	OutputDebugStringA("HelloDirectX!\n");
	
	//解放処理
	//CloseHandle(fenceEvent);
	
	//入力解放
	delete input;
	delete dxcCommon;
	
	//CloseWindow
	winApp->Finalize();
	
	delete winApp;



	return 0;
}