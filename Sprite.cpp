#include "Sprite.h"
#include"SpriteCommon.h"
using namespace Microsoft::WRL;
void Sprite::Initialize(SpriteCommon* spriteCommon, std::string textureFilePath)
{
	this->spriteCommon = spriteCommon;
	
	
	//vertex
	VertexInitialize();
	
	//index
	IndexInitialize();

	//material
	MaterialInitialize();
	
	//transformation
	TransformationInitialize();

	textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(textureFilePath);
}

void Sprite::Update()
{
	//アンカーポイント
	float left = 0.0f - anchorPoint.x;
	float right = 1.0f - anchorPoint.x;
	float top = 0.0f - anchorPoint.y;
	float bottom = 1.0f - anchorPoint.y;

	//左右反転
	if (isFlipX_) {
		left = -left;
		right = -right;
	}
	//上下反転
	if (isFlipY_) {
		top = -top;
		bottom = -bottom;
	}



	//============================  頂点座標   =============================

	//頂点リソースにデータを書き込む
	vertexData[0].position = { left,bottom,0.0f,1.0f };
	vertexData[0].texcoord = { 0.0f,1.0f };
	vertexData[0].normal = { 0.0f,0.0f,-1.0f };
	vertexData[1].position = { left,top,0.0f,1.0f };
	vertexData[1].texcoord = { 0.0f,0.0f };
	vertexData[1].normal = { 0.0f,0.0f,-1.0f };
	vertexData[2].position = { right,bottom,0.0f,1.0f };
	vertexData[2].texcoord = { 1.0f,1.0f };
	vertexData[2].normal = { 0.0f,0.0f,-1.0f };
	vertexData[3].position = { right,top,0.0f,1.0f };
	vertexData[3].texcoord = { 1.0f,0.0f };
	vertexData[3].normal = { 0.0f,0.0f,-1.0f };

	//============================トランスフォーム==========================


	Transform transform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	transform.translate = { position.x,position.y,0.0f };
	transform.rotate = { 0.0f,0.0f,rotation };
	transform.scale = { size.x,size.y,1.0f };

	TransformationUpdate(transform);

	//==============================UVTransform================================
	Transform uvTransformSprite{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	UVTransform(uvTransformSprite);
}

void Sprite::Draw()
{

	
	
	//VertexBufferViewを設定
	spriteCommon->GetDxcCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	////IBVを設定
	spriteCommon->GetDxcCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferViewSprite);
	//マテリアルCBufferの場所を設定
	spriteCommon->GetDxcCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResourceSprite->GetGPUVirtualAddress());
	//TransformationMatrixCbufferの場所を設定
	spriteCommon->GetDxcCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSprite->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定
	spriteCommon->GetDxcCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSRVHandleGPU(textureIndex));

	//描画！(DrawCall/ドローコール)。3頂点出一つのインスタンス。インスタンスについては今後
	spriteCommon->GetDxcCommon()->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void Sprite::VertexInitialize()
{
	//vertexResource
	vertexResource = spriteCommon->GetDxcCommon()->CreateBufferResource(sizeof(VertexData) * 6);
	//=============vertexBufferViewを作成===================

	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点6つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 6;
	//1頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	//========================================================

	//vertexDataにアドレスを書き込む
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	//=========================================================
	
	

}

void Sprite::IndexInitialize()
{
	//indexResource
	indexResourceSprite = spriteCommon->GetDxcCommon()->CreateBufferResource((sizeof(uint32_t) * 6));

	//===============indexBufferViewを作成===================

	//リソースの先頭のアドレスから使う
	indexBufferViewSprite.BufferLocation = indexResourceSprite->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点6つ分のサイズ
	indexBufferViewSprite.SizeInBytes = (sizeof(uint32_t) * 6);
	//1頂点当たりのサイズ
	indexBufferViewSprite.Format = DXGI_FORMAT_R32_UINT;


	//=======================================================


	indexResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSprite));


	//=======================================================

	//indexリソースにデータを書き込む
	indexDataSprite[0] = 0;
	indexDataSprite[1] = 1;
	indexDataSprite[2] = 2;
	indexDataSprite[3] = 1;
	indexDataSprite[4] = 3;
	indexDataSprite[5] = 2;

}

void Sprite::MaterialInitialize()
{
	//マテリアルソースを作る
	materialResourceSprite = spriteCommon->GetDxcCommon()->CreateBufferResource(sizeof(Material));
	//書き込むためのアドレスを取得
	materialResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSprite));
	//マテリアルリソースにデータを書き込むためのアドレスを取得してDataに割り当てる
	materialDataSprite->color = { Vector4(1.0f, 1.0f, 1.0f, 1.0f) };
	materialDataSprite->enableLighting = false;
	materialDataSprite->uvTransform = MakeIdentity4x4();
}

void Sprite::TransformationInitialize()
{
	//トランスフォーメーションリソースを作る
	transformationMatrixResourceSprite = spriteCommon->GetDxcCommon()->CreateBufferResource(sizeof(TransformationMatrix));
	//書き込むためのアドレスを取得
	transformationMatrixResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite));
	//単位行列を書き込んでく
	transformationMatrixDataSprite->WVP = MakeIdentity4x4();
	transformationMatrixDataSprite->World = MakeIdentity4x4();
}

void Sprite::TransformationUpdate(Transform transformSprite)
{
	Matrix4x4 worldMatrixSprite = MakeAffineMatrix(transformSprite.scale, transformSprite.rotate, transformSprite.translate);
	Matrix4x4 viewMatrixSprite = MakeIdentity4x4();
	Matrix4x4 projectionMatrixSprite = MakeOrthographicMatrix(0.f, 0.0f, float(WinApp::kClientWidth), float(WinApp::kClientHeight), 0.0f, 100.0f);
	Matrix4x4 worldviewProjectionMatrixSprite = MatrixMultiply(worldMatrixSprite, MatrixMultiply(viewMatrixSprite, projectionMatrixSprite));
	transformationMatrixDataSprite->WVP = worldviewProjectionMatrixSprite;
	transformationMatrixDataSprite->World = worldMatrixSprite;
}

void Sprite::UVTransform(Transform uvTransformSprite)
{
	//UVTransform用の行列
	Matrix4x4 uvTransformMatrix = MakeScaleMatrix(uvTransformSprite.scale);
	uvTransformMatrix = MatrixMultiply(uvTransformMatrix, MakeRotateZMatrix(uvTransformSprite.rotate.z));
	uvTransformMatrix = MatrixMultiply(uvTransformMatrix, MakeTranslateMatrix(uvTransformSprite.translate));
	materialDataSprite->uvTransform = uvTransformMatrix;
}
