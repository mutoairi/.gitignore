#include "ClearScene.h"
#include<Windows.h>
#include <iostream>

ClearScene::~ClearScene()
{
	delete sprite_;
}

void ClearScene::Initialize(Input* input, SpriteCommon* spriteCommon) {
	
	input_ = input;
	spriteCommon_ = spriteCommon;
	sprite_ = new Sprite();
	sprite_->Initialize(spriteCommon_, "resources/uvChecker.png");


}

void ClearScene::Update() {

	sprite_->Update();
	//スペースキーを押した瞬間
	if (input_->TriggerKey(DIK_SPACE)) {
		//シーンをTITLEに切り替える
		sceneNo = TITLE;
	}
}

void ClearScene::Draw() {
	spriteCommon_->CommonDrawing();
	sprite_->Draw();
}