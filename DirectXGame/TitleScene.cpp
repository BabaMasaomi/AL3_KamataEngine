#include "TitleScene.h"

// KamataEngine::を毎回入力しなくてもいい様にする
using namespace KamataEngine;

// 初期化
void TitleScene::Initialize() {
	// タイトルシーンの初期化処理
}

// 更新
void TitleScene::Update() { 
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		finished_ = true;
	}
}

// 描画
void TitleScene::Draw() {
	// タイトルシーンの描画処理
}