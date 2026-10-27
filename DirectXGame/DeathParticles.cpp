#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "DeathParticles.h"
#include <cmath>
#include <algorithm>
#include <cassert>

using namespace KamataEngine;

void DeathParticles::Initialize(Model* model, Camera* camera, const Vector3 pos) {

	assert(model);
	assert(camera);

	model_ = model;
	camera_ = camera;

	isFinished_ = false;
	counter_ = 0.0f;

	worldTransform_.Initialize();

	// 死亡したプレイヤーの中心から広がる
	worldTransform_.translation_ = pos;
	worldTransform_.translation_.z -= kFrontOffset;

	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.scale_ = {
	    kStartScale,
	    kStartScale,
	    1.0f,
	};

	objectColor_.Initialize();
	color_ = {1.0f, 1.0f, 1.0f, 1.0f};
	objectColor_.SetColor(color_);

	// 生成したフレームから正しい位置に描画する
	transform_.worldMatrixUpdate(worldTransform_);
}

void DeathParticles::Update() {
	if (isFinished_) {
		return;
	}

	counter_ = std::min(counter_ + 1.0f / 60.0f, kLifeTime);

	const float t = std::clamp(counter_ / kLifeTime, 0.0f, 1.0f);

	// 最初は速く広がり、終わりに向けて減速する
	const float easedT = 1.0f - (1.0f - t) * (1.0f - t);

	const float scale = kStartScale + (kEndScale - kStartScale) * easedT;

	worldTransform_.scale_ = {scale, scale, 1.0f};

	// 拡大しながら透明になる
	color_.w = 1.0f - t;
	objectColor_.SetColor(color_);

	transform_.worldMatrixUpdate(worldTransform_);

	if (counter_ >= kLifeTime) {
		isFinished_ = true;
	}
}

void DeathParticles::Draw() {
	if (isFinished_) {
		return;
	}

	// リングを1つだけ描画
	model_->Draw(worldTransform_, *camera_, &objectColor_);
}