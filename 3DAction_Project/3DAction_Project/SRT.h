#pragma once

// ==================================
// オブジェクトが基本定期に持つ情報
// ==================================


// ==================================
// ヘッダー
// ==================================
#include "Vector3.h"
#include "Quaternionh.h"
#include "Matrix4x4.h"


// ==================================
// SRT構造体
// ==================================
struct SRT {
	// --------------------------------
	// SRT情報
	// --------------------------------
	Vector3 position = { };
	Quaternion rotation = {};
	Vector3 scale = { 1.0f,1.0f,1.0f };

	// ワールド行列
	Matrix4x4 world = {};

	// --------------------------------
	// 関数
	// --------------------------------
	// ワールド行列更新
	Matrix4x4& UpdateWorldMatrix()
	{
		// ワールド行列計算
		world =
			Matrix4x4::CreateTranslationMatrix_LH(position) *
			Matrix4x4::CreateRotationQuaternion_LH(rotation) *
			Matrix4x4::CreateScalingMatrix_LH(scale);

		return world;
	}
};
