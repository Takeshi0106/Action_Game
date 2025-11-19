#pragma once

// =============================
// [クラス概要]
// ナイトクラス
// =============================


// ==============================
// ヘッダー
// ==============================
#include "BaseObject.h"
#include "Matrix4x4.h"
#include <string>

// ==============================
// クラス
// ==============================
class Knight : public BaseObject
{
private:
	// モデル名
	std::string m_ModelName = "Knight_Male";
	
	// シェーダー名
	std::string m_VSName = "VS_Object";
	std::string m_PSName = "PS_NoTexture";

	// 遅延初期化
	void LateInit() override final;

public:
	// 基本関数
	void Update() override final;
	void Draw() override final;
	void Uninit() override final;
};

