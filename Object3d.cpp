#include "Object3d.h"
#include"Object3dCommon.h"
#include"TextureManager.h"

void Object3d::Initialize(Object3dCommon* object3dCommon)
{
	this->object3dCommon = object3dCommon;
	
	TransformInitialize();
	EnableLightInitialize();
	

	//Transform変数を作る
	transform= { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	cameraTransform= { {1.0f,1.0f,1.0f},{0.3f,0.0f,0.0f},{0.0f,4.0f,-10.0f} };
}

void Object3d::Update()
{
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.0f);
	Matrix4x4 worldviewProjectionMatrix = MatrixMultiply(worldMatrix, MatrixMultiply(viewMatrix, projectionMatrix));
	wvpData->WVP = worldviewProjectionMatrix;
	wvpData->World = worldMatrix;
}

void Object3d::Draw()
{
	
	//wvp用のCBufferの場所を設定
	object3dCommon->GetDxcCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
	//平行光源
	object3dCommon->GetDxcCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
   //3Dモデルが割り当てられていたら描画
	if (model) {
		model->Draw();
	}
}







void Object3d::TransformInitialize()
{
	wvpResource = object3dCommon->GetDxcCommon()->CreateBufferResource(sizeof(TransformationMatrix));
	//書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	//単位行列を書き込んでおく
	wvpData->WVP = MakeIdentity4x4();
	wvpData->World = MakeIdentity4x4();
}

void Object3d::EnableLightInitialize()
{
	//平行光源用
    directionalLightResource = object3dCommon->GetDxcCommon()->CreateBufferResource(sizeof(DirectionalLight));
	
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
	directionalLightData->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData->direction = { 0.0f,1.0f,1.0f };
	directionalLightData->intensity = 8.0f;
}
