#pragma once
#include <memory>
#include "IScene.h"
#include "TitleScene.h"
#include "StageScene.h"
#include "ClearScene.h"
#include"Input.h"
#include"SpriteCommon.h"

/// GameManegerクラス
class GameManeger
{
private:

	void SceneChange(int prev, int current);//シーン入れ替え

	IScene* sceneArr_[3];
	uint32_t SceneNum = 3;


	int currentSceneNo_;//現在シーン
	int prevSceneNo_;//前シーン

	SpriteCommon* spriteCommon_;
	Input* input_;
public:
	GameManeger(); //コンストラクタ
	~GameManeger(); //デストラクタ

	void Initialize(Input*input,SpriteCommon*spriteCommon);

	//ゲームループで呼び出す関数
	void Update();
	void Draw();
};
