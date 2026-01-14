#define NOMINMAX
#include "Player.h"
#include "MapChipField.h"
#include <algorithm>
#include <cassert>
#include <numbers>

// コンストラクタ&デストラクタ
Player::Player() {}
Player::~Player() {}

// KamataEngine::を毎回入力しなくてもいい様にする
using namespace KamataEngine;

/// <summary>
/// 自機の初期化関数
/// </summary>
/// <param name="model">3Dモデル</param>
/// <param name="camera">カメラ</param>
void Player::Intialize(Model* model, Camera* camera, const Vector3 pos) {
	// ぬるぽチェック
	assert(model);

	// 引き数の内容をメンバ変数に記録
	camera_ = camera;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// メンバ変数への代入処理
	// プレイヤーの拡縮,回転,平行移動情報
	worldTransform_.scale_ = {2, 2, 2};
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	// 3Dモデルの生成
	model_ = model;
}

/// <summary>
/// 自機の更新
/// </summary>
void Player::Update() {
	/*========== ①移動入力 ==========*/
	// 接地している時
	if (onGround_) {
		// 移動入力
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			// 加速
			Vector3 acceleration = {};

			// 左右移動
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				// 速度と逆方向の時は急ブレーキ
				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}

				// 右移動
				acceleration.x += kAcceleration;

				// 体を右に
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					// 旋回開始時の角度を記録
					turnFirstRotationY_ = worldTransform_.rotation_.y;

					// 旋回タイマーをリセット
					turnTimer_ = kTimeTurn;
				}

			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				// 速度と逆方向の時は急ブレーキ
				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}

				// 左移動
				acceleration.x -= kAcceleration;

				// 体を左に
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					// 旋回開始時の角度を記録
					turnFirstRotationY_ = worldTransform_.rotation_.y;

					// 旋回タイマーをリセット
					turnTimer_ = kTimeTurn;
				}
			}

			// 加速/減速
			velocity_.x += acceleration.x;

			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

		} else {
			// 入力していない時は減速
			velocity_.x *= (1.0f - kAttenuation);
		}

		// ジャンプ入力
		if (Input::GetInstance()->TriggerKey(DIK_UP)) {
			// ジャンプ初速
			velocity_.y += kJumpAcceleration_;
		}

	} else { // 空中
		// 落下速度
		velocity_.y -= kGravityAcceleration;

		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed_);
	}

	/*========== ②移動量を加味して衝突判定する ==========*/
	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	// 移動量に速度の値をコピー
	collisionMapInfo.MovementAmount = velocity_;

	// マップ衝突チェック
	MapCollisionCheck(collisionMapInfo);

	/*========== ③判定結果を反映して移動 ==========*/
	MoveReflectingResult(collisionMapInfo);

	/*========== ④天井に接触している時の処理 ==========*/
	ContactWithCeiling(collisionMapInfo);

	/*========== ⑤壁に接触している時の処理 ==========*/


	/*========== ⑥接地状態の切り替え ==========*/
	// 着地フラグ
	bool landing = false;

	// 地面との当たり判定
	// 降下中か
	if (velocity_.y < 0) {
		// Y座標が地面以下になったら着地
		if (worldTransform_.translation_.y <= 2.0f) {
			landing = true;
		}
	}

	// 接地判定
	if (onGround_) {
		if (velocity_.y > 0.0f) {
			// 空中状態に移行
			onGround_ = false;
		}

	} else {
		// 着地
		if (landing) {
			// めり込み対策
			worldTransform_.translation_.y = 2.0f;
			// 摩擦で横方向の速度が減衰
			velocity_.x *= (1.0f - kAttenuation);
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 接地状態に移行
			onGround_ = true;
		}
	}

	/*========== ⑦旋回制御 ==========*/
	if (turnTimer_ > 0.0f) {
		// 旋回タイマーを1/60秒だけカウントダウン
		turnTimer_ -= 1.0f / 60.0f;
		turnTimer_ = std::max(turnTimer_, 0.0f);

		// 補間係数t
		float t = 1.0f - (turnTimer_ / kTimeTurn);
		t = std::clamp(t, 0.0f, 1.0f);

		// EaseInOutの形にする
		float easeT = t * t * (3.0f - 2.0f * t);

		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {
		    worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f,
		    worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f,
		};

		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		//	自キャラの角度を調整する
		// 旋回タイマーを使って角度を線形補間する
		worldTransform_.rotation_.y = turnFirstRotationY_ + (destinationRotationY - turnFirstRotationY_) * easeT;
	}

	/*========== ⑧行列計算 ==========*/
	// 行列を定数バッファに転送
	transform_.worldMatrixUpdate(worldTransform_);
}

/// <summary>
/// 自機の描画
/// </summary>
void Player::Draw() {
	// 自機を描画
	model_->Draw(worldTransform_, *camera_);
}

/// <summary>
/// マップ衝突判定
/// </summary>
/// <param name="info">マップとの当たり判定情報</param>
void Player::MapCollisionCheck(CollisionMapInfo& info) {
	// 方向別のマップ衝突判定
	MapCollisionCheckTop(info);
	/*MapCollisionCheckBottom(info);
	MapCollisionCheckRight(info);
	MapCollisionCheckLeft(info);*/
}

/// <summary>
/// 指定した角の座標計算
/// </summary>
/// <param name="center">指定したい角の矩形の座標</param>
/// <param name="corner">指定した角</param>
Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 result = {};

	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0.0f}, // 右下
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0.0f}, // 左下
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0.0f}, // 右上
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0.0f}  // 左上
	};

	result.x = center.x + offsetTable[static_cast<uint32_t>(corner)].x;
	result.y = center.y + offsetTable[static_cast<uint32_t>(corner)].y;
	result.z = center.z + offsetTable[static_cast<uint32_t>(corner)].z;

	return result;
}

// 方向別のマップ衝突判定
// 上方向
void Player::MapCollisionCheckTop(CollisionMapInfo& info) {
	// 上昇しているか
	if (info.MovementAmount.y <= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew = {};

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(
		    {worldTransform_.translation_.x + info.MovementAmount.x, worldTransform_.translation_.y + info.MovementAmount.y, worldTransform_.translation_.z + info.MovementAmount.z},
		    static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	// 真上の当たり判定を取る
	bool hit = false;

	// 左上点の判定
	MapChipField::IndexSet indexSet = {};
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 右上点の判定
	// MapChipField::IndexSet indexSet = {};
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックに当たっているか
	if (hit) {
		// めり込みを解消する方向に移動量を設定
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(
		    (Vector3)(worldTransform_.translation_.x + velocity_.x, worldTransform_.translation_.y + kHeight / 2.0f + velocity_.y, worldTransform_.translation_.z + velocity_.z));

		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.MovementAmount.y = std::max(0.0f, (rect.bottom - worldTransform_.translation_.y) - (kHeight + kBlank));
		// 天井に当たった
		info.isCeilingCollide = true;
	}
}
// void Player::MapCollisionCheckBottom(CollisionMapInfo& info) {}
// void Player::MapCollisionCheckRight(CollisionMapInfo& info) {}
// void Player::MapCollisionCheckLeft(CollisionMapInfo& info) {}

// 判定結果を反映させて移動させる
void Player::MoveReflectingResult(const CollisionMapInfo& info) {
	// 移動
	worldTransform_.translation_.x += info.MovementAmount.x;
	worldTransform_.translation_.y += info.MovementAmount.y;
}

// 天井に接触している時の処理
void Player::ContactWithCeiling(const CollisionMapInfo& info) {
	// 天井に当たったか
	if (info.isCeilingCollide) {
		DebugText::GetInstance()->ConsolePrintf("hit ceiking\n");
		velocity_.y = 0.0f;
	}
}