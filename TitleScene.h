#pragma once
#include "IScene.h"
#include"Input.h"
#include"Sprite.h"
#include"SpriteCommon.h"


/// <summary>
/// ISceneクラスを継承したTitleSceneを宣言
/// </summary>
class TitleScene : public IScene
{
public://メンバ関数
	~TitleScene();
	///初期化処理
	void Initialize(Input* input, SpriteCommon* spriteCommon) override;

	///更新処理
	void Update() override;

	///描画処理
	void Draw() override;

private:
	SpriteCommon* spriteCommon_;
	Sprite* sprite_;
	Input* input_;

};
