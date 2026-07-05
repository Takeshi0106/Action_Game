#pragma once

// ========================================
// クラス概要
// 原点を中心に惑星を動かすクラス
// ========================================


// ========================================
// ヘッダー
// ========================================
// 描画マネージャー
#include "IDrawManager.h"
// 数学ヘッダー
#include "SRT.h"
#include "Vector3.h"
// 文字列ヘッダー
#include "UTF8_String.h"


// ========================================
// クラス
// ========================================
class MovePlanet
{
private:
	// 描画マネージャー
	IDrawManager* m_Draw = nullptr;
	// SRT情報
	SRT m_SRT = {};
	// 回転の原点位置
	Vector3 m_MoveOriginPos = {};
	// 原点からの距離
	float m_Distance = {};

	Vector3 m_InitialPos = {};
	float m_RotateSpeed = 0.0f;
	float m_TotalAngle = 0.0f;
	Vector3 m_Axis = { 0.0f,1.0f,0.0f };

	// シェーダー情報
	const Hashed_String m_VSName = Hashed_String(u8"VS_Object");
	const Hashed_String m_PSName = Hashed_String(u8"PS_TextureModel");

	// 定数バッファ名
	const Hashed_String m_TransformCBName = Hashed_String(u8"Transform");

	// モデル名
	Hashed_String m_ModelName = Hashed_String(u8"");

public:
	// --------------------------------
	// コンストラクタ・デストラクタ
	// --------------------------------
	MovePlanet() = default;
	~MovePlanet() = default;
	

	// --------------------------------
	// 基本関数
	// --------------------------------
	// 初期化
	void Init(IDrawManager* _draw, const Hashed_String& _modelName, const String& _modelFile = u8"");
	// 更新
	void Update(float _time);
	// 描画
	void Draw();
	// 後処理
	void Uninit();


	// --------------------------------
	// セッター
	// --------------------------------
	// SRT設定
	void SetSRT(const SRT& _srt) { m_SRT = _srt; }
	void SetPosition(const Vector3& _pos) { m_SRT.position = _pos; }
	void SetRotation(const Quaternion& _rot) { m_SRT.rotation = _rot; }
	void SetScale(const Vector3& _scale) { m_SRT.scale = _scale; }

	// 回転の原点位置設定
	void SetMoveOriginPos(const Vector3& _pos) { m_MoveOriginPos = _pos; }
	void SetDistance(const float& _distance) { m_Distance = _distance; }
	void SetRotateSpeed(const float& _speed) { m_RotateSpeed = _speed; }
	void SetAxis(const Vector3& _axis) { m_Axis = _axis.Normalize(); }

	// ---------------------------------
	// ゲッター
	// ---------------------------------
	const SRT& GetSRT() const { return m_SRT; }
	const Vector3& GetMoveOriginPos() const { return m_MoveOriginPos; }
	const float& GetDistance() const { return m_Distance; }

};

