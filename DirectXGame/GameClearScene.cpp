#include "GameClearScene.h"
#include <numbers>

// KamataEngine::を毎回入力しなくてもいい様にする
using namespace KamataEngine;

/*-------------------- コンストラクタ&デストラクタ --------------------*/
GameClearScene::GameClearScene() {}
GameClearScene::~GameClearScene() {
	delete modelGameClear_;  // ゲームクリアフォントの3Dモデルの解放
	delete modelPlayer_; // プレイヤーの3Dモデルの解放
	delete fade_;        // フェードの解放
}

/*-------------------- 初期化 --------------------*/
void GameClearScene::Initialize() {

	// カメラ初期化
	camera_.farZ = 550.0f;
	camera_.Initialize();

	// ゲームクリア用フォントのモデル
	modelGameClear_ = Model::CreateFromOBJ("titleFont", true); // ゲームクリアフォントの3Dモデルを生成
	worldTransformGameClear_.Initialize();                     // ゲームクリアのワールドトランスフォームの初期化
	worldTransformGameClear_.translation_ = {0.0f, 5.0f, 0.0f};
	worldTransformGameClear_.scale_ = {2.0f, 2.0f, 2.0f};

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
void GameClearScene::Update() {
	switch (phase_) {
	case GameClearScene::Phase::kFadeIn:
		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		break;

	case GameClearScene::Phase::kMain:
		// シーン切り替え
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kFadeOut;
		}
		break;

	case GameClearScene::Phase::kFadeOut:
		// フェードアウトが終わったらゲームシーンに移行
		if (fade_->IsFinished()) {
			finished_ = true;
		}
		worldTransformPlayer_.rotation_.y += 1.0f;
		break;

	default:
		break;
	}

	// プレイヤーを回す
	worldTransformPlayer_.rotation_.y += 0.02f;

	// 更新を反映
	transform_.worldMatrixUpdate(worldTransformGameClear_);
	transform_.worldMatrixUpdate(worldTransformPlayer_);
	camera_.UpdateMatrix();

	// フェードを更新
	fade_->Update();
}

/*-------------------- 描画 --------------------*/
void GameClearScene::Draw() {
	Model::PreDraw();

	// モデルを描画
	modelGameClear_->Draw(worldTransformGameClear_, camera_);
	modelPlayer_->Draw(worldTransformPlayer_, camera_);

	// フェードを描画
	fade_->Draw();

	Model::PostDraw();
}