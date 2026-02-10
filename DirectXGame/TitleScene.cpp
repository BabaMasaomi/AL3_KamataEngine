#include "TitleScene.h"

// KamataEngine::を毎回入力しなくてもいい様にする
using namespace KamataEngine;

// 初期化
void TitleScene::Initialize() {
	// タイトルづくり用変数の初期化
	//// プレイヤーモデルの生成
	//modelPlayer_ = Model::CreateFromOBJ("player", true);
}

// 更新
void TitleScene::Update() { 
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		finished_ = true;
	}

	// タイトルのアニメーション等

	//// 行列を定数バッファに転送
	//transform_.worldMatrixUpdate(worldTransform_);
}

// 描画
void TitleScene::Draw() {
	Model::PreDraw();
	// タイトルの文字

	// タイトルのモデル
	// プレイヤー


	Model::PostDraw();

}