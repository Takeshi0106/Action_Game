#pragma once

// ===================================
// 【構造体概要】
//  軸平行バウンディングボックスの衝突判定クラス
// ===================================


// ===================================
// ヘッダー
// ===================================
// 計算ヘッダー
#include "Vector3.h"
#include "SRT.h"
#include "Matrix3x3.h"
// コライダー設定ヘッダー
#include "ColliderConfig.h"


// ===================================
// 構造体
// ===================================
// AABBコライダー構造体
struct AABBCollider
{
	// 最小位置 最大位置
	Vector3 min = {};
	Vector3 max = {};

	// コライダー設定
	ColliderPresetConfig config = {};

	// コライダー更新
	const AABBCollider& UpdateAABB(const SRT& _srt)
	{
		// 当たり判定の半径
		Vector3 localExtent = (_srt.scale * config.sizeScale) * 0.5;

		// ローカル中心位置
		Vector3 localCenter = config.offset * _srt.scale;
		Vector3 rotatedCenter = _srt.rotation.RotateVector(localCenter);

		// ワールド中心位置を計算
		Vector3 worldCenter = _srt.position + rotatedCenter;

		// 回転行列の絶対値計算

		Matrix3x3 R = Matrix3x3::CreateRotationQuaternion_LH(_srt.rotation);
		Matrix3x3 absR = R.Abs();

		// ワールドの拡張ベクトル計算
		Vector3 worldExtent = absR * localExtent;

		// AABB
		min = worldCenter - worldExtent;
		max = worldCenter + worldExtent;

		return *this;
	}

};


// ====================================
// コライダー作成関数
// ====================================
inline AABBCollider CreateAABB(const SRT& _srt, const ColliderPresetConfig& _config)
{
	AABBCollider aabb;

	// 情報セット
	aabb.config = _config;
	// コライダー更新
	aabb.UpdateAABB(_srt);

	return aabb;
}


// ====================================
// ファットAABB作成関数
// ====================================
inline AABBCollider CreateFatAABB(const AABBCollider& aabb, const Vector3& velocity, float padding = 0.1f)
{
	AABBCollider fat = aabb;

	Vector3 expand = velocity * 1.0f; // 次フレームで動きそうな距離
	fat.min -= expand;
	fat.max += expand;

	// さらに少し余裕を持たせる
	fat.min -= Vector3(padding,padding,padding);
	fat.max += Vector3(padding,padding,padding);

	return fat;
}
