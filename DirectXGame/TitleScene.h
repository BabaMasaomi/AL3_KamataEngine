#pragma once
#include "KamataEngine.h"

class TitleScene {
private:
	// 終了フラグ
	bool finished_ = false;

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
