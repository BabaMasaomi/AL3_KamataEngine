#pragma once
#include "KamataEngine.h"
#include "Transform.h"
#include <array>
#include <numbers>

class DeathParticles {
private:
	// Translateクラス内の関数を使える様にする
	Transform transform_;

	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	KamataEngine::Model* model_ = nullptr;

	// パーティクルの個数
	static inline const uint32_t kNumParticles = 8;

	std::array<KamataEngine::WorldTransform, kNumParticles> worldTransforms_;

	// パーティクルの動きに関数変数
	// 持続時間(秒)
	static inline const float kParticleLifeTime = 2.0f;

	// 移動の速さ
	static inline const float kParticleMoveSpeed = 0.1f;

	// 分割した1個分の角度
	static inline const float kParticleAngleDivide = std::numbers::pi_v<float> * 2.0f / static_cast<float>(kNumParticles);

	// パーティクルを終了させるフラグ
	bool isFinished_ = false;

	// 経過時間カウント
	float counter_ = 0.0f;

	// 色変更用変数
	KamataEngine::ObjectColor objectColor_ = {};

	KamataEngine::Vector4 color_;

public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3 pos);

	void Update();

	void Draw();
};
