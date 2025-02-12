#include "TitleScene.h"
#include<Windows.h>
#include <iostream>


TitleScene::~TitleScene()
{
	delete sprite_;
}

void TitleScene::Initialize(Input* input, SpriteCommon* spriteCommon) {
	input_ = input;
	spriteCommon_ = spriteCommon;
	sprite_ = new Sprite();
	sprite_->Initialize(spriteCommon_, "resources/uvChecker.png");
}

void TitleScene::Update() {
	sprite_->Update();
	//スペースキーを押した瞬間
	if (input_->TriggerKey(DIK_SPACE)) {
		//シーンをSTAGEに切り替える
		sceneNo = STAGE;
	}
}

void TitleScene::Draw() {
	spriteCommon_->CommonDrawing();
	sprite_->Draw();
}