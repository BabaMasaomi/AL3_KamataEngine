#pragma once
#include "KamataEngine.h"
#include "Player.h"
//#include "Transform.h"

class TitleScene {
private:
	// 終了フラグ
	bool finished_ = false;

	//// Translateクラス内の関数を使える様にする
	//Transform transform_;

	//// ワールド変換データ
	//KamataEngine::WorldTransform worldTransform_;

	//// タイトルづくり用変数
	//KamataEngine::Model* modelPlayer_ = nullptr;

public:
	/// <summary>
	/// タイトルシーンの初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// タイトルシーンの更新
	/// </summary>
	void Update();

	/// <summary>
	/// タイトルシーンの描画
	/// </summary>
	void Draw();

	/*-------------------- アクセッサ --------------------*/
	bool GetIsFinished() const { return finished_; }
};
