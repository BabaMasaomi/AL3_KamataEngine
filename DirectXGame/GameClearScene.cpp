#include "GameClearScene.h"
#include <numbers>

// KamataEngine::を毎回入力しなくてもいい様にする
using namespace KamataEngine;

/*-------------------- コンストラクタ&デストラクタ --------------------*/
GameClearScene::GameClearScene() {}
GameClearScene::~GameClearScene() {
	delete creditSprite_;
	delete returnToTitleSprite_;
	if (resultBgmStarted_) {
		Audio::GetInstance()->StopWave(resultVoiceHandle_);
	}

	delete modelGameClear_;  // ゲームクリアフォントの3Dモデルの解放
	delete skydome_;
	delete modelSkydome_;
	delete modelPlayer_; // プレイヤーの3Dモデルの解放
	delete fade_;        // フェードの解放
}

/*-------------------- 初期化 --------------------*/
void GameClearScene::Initialize() {
	static const uint32_t resultSound = Audio::GetInstance()->LoadWave("ResultBGM.mp3");
	resultSoundHandle_ = resultSound;

	// Screen-fixed credits, right aligned with a 24-pixel margin.
	const uint32_t creditTexture = TextureManager::Load("Cledit.png");
	creditSprite_ = Sprite::Create(creditTexture, {1256.0f, 500.0f},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.5f});
	creditSprite_->SetSize({512.0f, 512.0f});

	// Place the text strip below the left-side player.
	const uint32_t returnTexture = TextureManager::Load("returnToTitle.png");
	returnToTitleSprite_ = Sprite::Create(returnTexture, {466.0f, 640.0f},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
	returnToTitleSprite_->SetTextureRect({0.0f, 224.0f}, {700.0f, 72.0f});
	returnToTitleSprite_->SetSize({560.0f, 57.6f});

	// カメラ初期化
	camera_.farZ = 550.0f;
	camera_.Initialize();

	// 天球の生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &camera_);
	skydome_->Update();

	// ゲームクリア用フォントのモデル
	modelGameClear_ = Model::CreateFromOBJ("clearFont", true); // ゲームクリアフォントの3Dモデルを生成
	worldTransformGameClear_.Initialize();                     // ゲームクリアのワールドトランスフォームの初期化
	worldTransformGameClear_.translation_ = {0.0f, 8.0f, 0.0f};
	worldTransformGameClear_.scale_ = {2.0f, 2.0f, 2.0f};

	// プレイヤーのモデル
	modelPlayer_ = Model::CreateFromOBJ("player", true); // プレイヤーの3Dモデルを生成
	worldTransformPlayer_.Initialize();                  // プレイヤーのワールドトランスフォームの初期化
	worldTransformPlayer_.translation_ = {-10.0f, -10.0f, 0.0f};
	worldTransformPlayer_.scale_ = {6.0f, 6.0f, 6.0f};

	// フェード用
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 2.0f);

	resultVoiceHandle_ = Audio::GetInstance()->PlayWave(resultSoundHandle_, false, 1.0f);
	resultBgmStarted_ = true;
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
	worldTransformPlayer_.rotation_.x += 0.02f;

	// 更新を反映
	transform_.worldMatrixUpdate(worldTransformGameClear_);
	transform_.worldMatrixUpdate(worldTransformPlayer_);
	camera_.UpdateMatrix();

	// 天球の更新
	skydome_->Update();

	// フェードを更新
	fade_->Update();
}

/*-------------------- 描画 --------------------*/
void GameClearScene::Draw() {
	Model::PreDraw();

	// 背景
	skydome_->Draw();

	// モデルを描画
	modelGameClear_->Draw(worldTransformGameClear_, camera_);
	modelPlayer_->Draw(worldTransformPlayer_, camera_);

	Model::PostDraw();

	Sprite::PreDraw();
	creditSprite_->Draw();
	returnToTitleSprite_->Draw();
	Sprite::PostDraw();

	// Fade covers the credits as well.
	fade_->Draw();
}