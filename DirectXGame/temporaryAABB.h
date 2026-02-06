#pragma once
#include "KamataEngine.h"

// AABBを使うために仮定義
// 自作エンジンを作る時書き換えろ

struct AABB {
	KamataEngine::Vector3 min; // 最小座標
	KamataEngine::Vector3 max; // 最大座標
};