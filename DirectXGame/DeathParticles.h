#pragma once
#include "KamataEngine.h"
#include "Transform.h"

class DeathParticles {
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3 pos);

	void Update();
	void Draw();

	bool GetIsFinished() const { return isFinished_; }

private:
	Transform transform_;

	KamataEngine::Camera* camera_ = nullptr;
	KamataEngine::Model* model_ = nullptr;

	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::ObjectColor objectColor_;
	KamataEngine::Vector4 color_ = {1.0f, 1.0f, 1.0f, 1.0f};

	bool isFinished_ = false;
	float counter_ = 0.0f;

	// 演出時間
	static constexpr float kLifeTime = 0.6f;

	// リングの開始・終了サイズ
	static constexpr float kStartScale = 0.1f;
	static constexpr float kEndScale = 4.0f;

	// 手前に表示するための補正
	static constexpr float kFrontOffset = 1.2f;
};