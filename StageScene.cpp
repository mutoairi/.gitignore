#include "StageScene.h"
#include<Windows.h>
#include <iostream>

StageScene::~StageScene()
{
	delete sprite_;
}

void StageScene::Initialize(Input* input, SpriteCommon* spriteCommon) {
	input_ = input;
	spriteCommon_ = spriteCommon;
	sprite_ = new Sprite();
	sprite_->Initialize(spriteCommon_, "resources/monsterBall.png");

}

void StageScene::Update() {
	sprite_->Update();
	//スペースキーを押した瞬間
	if (input_->TriggerKey(DIK_SPACE)) {
		//シーンをTITLEに切り替える
		sceneNo = TITLE;
	}
}

void StageScene::Draw() {
	spriteCommon_->CommonDrawing();
	sprite_->Draw();
}
