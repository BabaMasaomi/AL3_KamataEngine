#include "GameOverScene.h"
#include <numbers>

// KamataEngine::を毎回入力しなくてもいい様にする
using namespace KamataEngine;

/*-------------------- コンストラクタ&デストラクタ --------------------*/
GameOverScene::GameOverScene() {}
GameOverScene::~GameOverScene() {
	delete modelGameOver_;  // ゲームオーバーフォントの3Dモデルの解放
	delete modelPlayer_; // プレイヤーの3Dモデルの解放
	delete fade_;        // フェードの解放
}

/*-------------------- 初期化 --------------------*/
void GameOverScene::Initialize() {

	// カメラ初期化
	camera_.farZ = 550.0f;
	camera_.Initialize();

	// ゲームオーバー用フォントのモデル
	modelGameOver_ = Model::CreateFromOBJ("GameOverFont", true); // ゲームオーバーフォントの3Dモデルを生成
	worldTransformGameOver_.Initialize();                     // ゲームオーバーのワールドトランスフォームの初期化
	worldTransformGameOver_.translation_ = {0.0f, 5.0f, 0.0f};
	worldTransformGameOver_.scale_ = {2.0f, 2.0f, 2.0f};

	// プレイヤーのモデル
	modelPlayer_ = Model::CreateFromOBJ("player", true); // プレイヤーの3Dモデルを生成
	worldTransformPlayer_.Initialize();                  // プレイヤーのワールドトランスフォームの初期化
	worldTransformPlayer_.translation_ = {0.0f, -10.0f, 0.0f};
	worldTransformPlayer_.scale_ = {6.0f, 6.0f, 6.0f};

	// フェード用
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 2.0f);
}

/*-------------------- 更新 --------------------*/
void GameOverScene::Update() {
	switch (phase_) {
	case GameOverScene::Phase::kFadeIn:
		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		break;

	case GameOverScene::Phase::kMain:
		// シーン切り替え
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kFadeOut;
		}
		break;

	case GameOverScene::Phase::kFadeOut:
		// フェードアウトが終わったらゲームシーンに移行
		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;

	default:
		break;
	}

	// 更新を反映
	transform_.worldMatrixUpdate(worldTransformGameOver_);
	transform_.worldMatrixUpdate(worldTransformPlayer_);
	camera_.UpdateMatrix();

	// フェードを更新
	fade_->Update();
}

/*-------------------- 描画 --------------------*/
void GameOverScene::Draw() {
	Model::PreDraw();

	// モデルを描画
	modelGameOver_->Draw(worldTransformGameOver_, camera_);
	modelPlayer_->Draw(worldTransformPlayer_, camera_);

	// フェードを描画
	fade_->Draw();

	Model::PostDraw();
}