#include "GameClearScene.h"
#include "GameOverScene.h"
#include "GameScene.h"
#include "KamataEngine.h"
#include "TitleScene.h"
#include <Windows.h>

// KamataEngine::を毎回入力しなくてもいい様にする
using namespace KamataEngine;

/*-------------------- グローバル --------------------*/
// 変数
TitleScene* titleScene = nullptr;
GameScene* gameScene = nullptr;
GameClearScene* gameClearScene = nullptr;
GameOverScene* gameOverScene = nullptr;

// シーン (型)
enum class Scene {

	kUnKnown = 0,

	kTitle,
	kGame,
	kGameClear,
	kGameOver,
};

// 現在のシーン (型)
Scene scene = Scene::kUnKnown;

/*--------------- 関数の宣言 ---------------*/
// シーン変更
void ChangeScene();

// シーン更新
void UpdateScene();

// シーンの描画
void DrawScene();

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	Initialize(L"LE2B_22_ババ_マサオミ_AL4_1回目");

	// DirectXCommonインスタンスを取得(画面描画に使用)
	DirectXCommon* dxCoommon = DirectXCommon::GetInstance();

	// ImGuiのインスタンスを取得
	ImGuiManager* imGuiManager = ImGuiManager::GetInstance();

	// 最初のシーンの初期化
	scene = Scene::kTitle;

	titleScene = new TitleScene;
	titleScene->Initialize();

	// メインループ
	while (true) {
		// エンジンの更新
		if (Update()) {
			break;
		}

		/*--------------- 更新処理 ---------------*/
		// ImGui受付開始
		imGuiManager->Begin();

		// シーンの切り替え
		ChangeScene();

		// シーンの更新
		UpdateScene();

		// ImGui受付終了
		imGuiManager->End();

		/*--------------- 描画処理 ---------------*/
		// 描画開始
		dxCoommon->PreDraw();

		// シーンの描画
		DrawScene();

		// ImGui描画
		imGuiManager->Draw();

		// 描画終了
		dxCoommon->PostDraw();
	}

	// 解放処理
	delete titleScene;
	delete gameScene;
	delete gameClearScene;
	delete gameOverScene;

	// エンジンの終了
	Finalize();

	return 0;
}

/*--------------- 関数の定義 ---------------*/
// シーン変更
void ChangeScene() {
	switch (scene) {
	case Scene::kTitle:
		// タイトルシーンが終了したら
		if (titleScene->GetIsFinished()) {
			// シーン変更
			scene = Scene::kGame;
			// 旧シーンの解放
			delete titleScene;
			titleScene = nullptr;
			// 新シーンの生成と初期化
			gameScene = new GameScene;
			gameScene->Initialize();
		}

		break;
	case Scene::kGame:

		// リトライ要求があればゲームシーンを作り直す
		if (gameScene->GetIsRetryRequested()) {
			// 古いゲームシーンを解放
			delete gameScene;
			gameScene = nullptr;

			// 新しいゲームシーンを生成・初期化
			gameScene = new GameScene;
			gameScene->Initialize();

			break;
		}

		// ゲームシーンが終了したら
		if (gameScene->GetIsFinished()) {
			// 削除する前に結果を保存する
			const bool isClear = gameScene->GetIsClear();

			// 旧シーンの解放
			delete gameScene;
			gameScene = nullptr;
			// 新シーンの生成と初期化
			if (isClear) {
				// シーン変更
				scene = Scene::kGameClear;

				gameClearScene = new GameClearScene;
				gameClearScene->Initialize();
			} else {
				// シーン変更
				scene = Scene::kGameOver;

				gameOverScene = new GameOverScene;
				gameOverScene->Initialize();
			}
		}

		break;
	case Scene::kGameClear:
		// ゲームクリアシーンが終了したら
		if (gameClearScene->GetIsFinished()) {
			// シーン変更
			scene = Scene::kTitle;
			// 旧シーンの解放
			delete gameClearScene;
			gameClearScene = nullptr;
			// 新シーンの生成と初期化
			titleScene = new TitleScene;
			titleScene->Initialize();
		}

		break;

	case Scene::kGameOver:
		// ゲームオーバーシーンが終了したら
		if (gameOverScene->GetIsFinished()) {
			// シーン変更
			scene = Scene::kTitle;
			// 旧シーンの解放
			delete gameOverScene;
			gameOverScene = nullptr;
			// 新シーンの生成と初期化
			titleScene = new TitleScene;
			titleScene->Initialize();
		}

		break;
	default:
		break;
	}
}

// シーン更新
void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		// タイトルシーンの更新
		titleScene->Update();

		break;
	case Scene::kGame:
		// ゲームシーンの更新
		gameScene->Update();

		break;
	case Scene::kGameClear:
		// ゲームクリアシーンの更新
		gameClearScene->Update();

		break;
	case Scene::kGameOver:
		// ゲームオーバーシーンの更新
		gameOverScene->Update();

		break;
	default:
		break;
	}
}

// シーンの描画
void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		// タイトルシーンの描画
		titleScene->Draw();

		break;
	case Scene::kGame:
		// ゲームシーンの描画
		gameScene->Draw();

		break;
	case Scene::kGameClear:
		// ゲームクリアシーンの描画
		gameClearScene->Draw();

		break;
	case Scene::kGameOver:
		// ゲームクオーバーシーンの描画
		gameOverScene->Draw();

		break;
	default:
		break;
	}
}