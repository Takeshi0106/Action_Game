#pragma once

// ============================
// 【クラス概要】
// ベースカメラ
// カメラの基底クラス
// ============================


// ============================
//  ヘッダー
// ============================
// 計算ヘッダー
#include "Vector3.h"
#include "Matrix4x4.h"
// 描画マネージャ
#include "IDrawManager.h"
// 文字列ヘッダー
#include "UTF8_String.h"


// ============================
// 構造体定義
// ============================
// View情報
struct CameraViewInfo
{
	Vector3 CameraPos = { 0.0f,0.0f,-5.0f }; // カメラ位置
	Vector3 LookPoint = { 0.0f,0.0f,0.0f }; // 注視点
	Vector3 UpDir = { 0.0f,1.0f,0.0f }; // 上方向
};

// Projection情報
struct CameraProjInfo
{
	float FovY = 3.14159265f / 4.0f; // 視野角Y方向
	float Aspect = 1280.0f / 720.0f; // アスペクト比
	float NearZ = 0.1f; // ニアクリップ距離
	float FarZ = 100.0f; // ファークリップ距離
};


// ============================
// クラス
// ============================
class BaseCamera
{
protected:
	// ビュー情報
	CameraViewInfo m_ViewInfo;
	// プロジェクション情報
	CameraProjInfo m_ProjInfo;

	// プロジェクション行列
	Matrix4x4 m_ProjMatrix;

	// 描画マネージャー
	IDrawManager* m_DrawManager = nullptr;
	// 更新するカメラ情報名
	const Hashed_String kCameraInfoBufferName = Hashed_String(u8"CameraInfo");

	// 派生初期化
	virtual bool DerivativInit() { return true; }
	// 派生情報をGPUに送る関数
	virtual void DerivativUpdateToGPU() {}

public:
	// コンストラクタ・デストラクタ
	BaseCamera() = default;
	virtual ~BaseCamera() = default;

	// 初期化
	bool Init(IDrawManager* drawManager);
	// GPUに情報をおくる
	void UpdateToGPU();
	
	// 更新
	virtual void Update() {}
	// 後処理
	virtual void Uninit() {}

	// セッター
	// View情報
	void SetCameraPos(const Vector3& pos) { m_ViewInfo.CameraPos = pos; }
	void SetLookPoint(const Vector3& look) { m_ViewInfo.LookPoint = look; }
	void SetUpDir(const Vector3& up) { m_ViewInfo.UpDir = up; }
	void SetViewInfo(const CameraViewInfo& info) { m_ViewInfo = info; }
	// Projection情報
	void SetProjection(const CameraProjInfo& info) 
	{ 
		// 情報保存
		m_ProjInfo = info; 
		
		// プロジェクション行列計算
		m_ProjMatrix = Matrix4x4::CreateProjectionMatrix_LH(
			m_ProjInfo.FovY,
			m_ProjInfo.Aspect,
			m_ProjInfo.NearZ,
			m_ProjInfo.FarZ);
	}

	// ゲッター
	// View情報
	const Vector3& GetCameraPos() const { return m_ViewInfo.CameraPos; }
	const Vector3& GetLookPoint() const { return m_ViewInfo.LookPoint; }
	const Vector3& GetUpDir() const { return m_ViewInfo.UpDir; }
	const CameraViewInfo& GetViewInfo() const { return m_ViewInfo; }
	// Projection情報
	const CameraProjInfo& GetProjection() const { return m_ProjInfo; }

};

