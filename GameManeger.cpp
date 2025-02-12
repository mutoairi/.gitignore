#include "GameManeger.h"
#include<memory>


void GameManeger::SceneChange(int prev, int current)
{
	delete sceneArr_[prev];
	sceneArr_[prev] = nullptr;

	//scene_ = current;
	switch (current)
	{
	case TITLE:
		sceneArr_[current] = new TitleScene();
		break;
	case STAGE:
		sceneArr_[current] = new StageScene();
		break;
	case CLEAR:
		sceneArr_[current] = new ClearScene();
		break;
	
		break;
	}
}

/// コンストラクタによる初期化処理
GameManeger::GameManeger() {

	sceneArr_[TITLE] = new TitleScene();

	//初期シーンをタイトルに設定する
	currentSceneNo_ = TITLE;
	prevSceneNo_ = 0;
}

//デストラクタによるdelete処理
GameManeger::~GameManeger() {}

void GameManeger::Initialize(Input* input, SpriteCommon* spriteCommon)
{
	this->input_ = input;
	
	this->spriteCommon_ = spriteCommon;
}

void GameManeger::Update()
{
	// 現在のシーンの更新
	int prevSceneNo = currentSceneNo_;
	currentSceneNo_ = sceneArr_[currentSceneNo_]->GetSceneNo();

	if (prevSceneNo != currentSceneNo_) {
		// シーン切り替え
		SceneChange(prevSceneNo, currentSceneNo_);
		sceneArr_[currentSceneNo_]->Initialize(input_,spriteCommon_);
	}

	sceneArr_[currentSceneNo_]->Update();
}

void GameManeger::Draw()
{
	sceneArr_[currentSceneNo_]->Draw();
}

