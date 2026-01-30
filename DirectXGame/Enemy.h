#pragma once
#include "KamataEngine.h"
#include "Transform.h"

// 前方宣言
class MapChipField;

// 左右の向き
//enum class LRDirection {
//	kRight,
//	kLeft,
//};

class Enemy {
private:
	// Translateクラス内の関数を使える様にする
	Transform transform_;

	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	KamataEngine::Model* modelEnemy_ = nullptr;

	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	// 以下、移動などに使う変数をまとめる
	KamataEngine::Vector3 velocity_ = {};

	// 基礎移動速度
	static inline const float kMoveSpeed = 0.05f;


	// アニメーション用変数
	// 最初の角度(度)
	static inline const float kWalkMotionAngleStart = -30.f;

	// 最後の角度(度)
	static inline const float kWalkMotionAngleEnd = 30.f;

	// アニメーション周期となる時間（秒）
	static inline const float kWalkMotionTime = 0.5f;

	// 経過時間
	float walkTimer_ = 0.0f;

public:
	// コンストラクタ&デストラクタ
	Enemy();
	~Enemy();

	/// <summary>
	/// 敵の初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="pos"></param>
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3 pos);

	/// <summary>
	/// 敵の更新
	/// </summary>
	void Update();

	/// <summary>
	/// 敵の描画
	/// </summary>
	void Draw();

};
