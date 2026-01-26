#pragma once

// ========================================
// タイトルシーン
// ========================================


// ========================================
// ヘッダー
// ========================================
// 基底クラス
#include "BaseSceneState.h"
// 行列ヘッダー
#include "Matrix4x4.h"
// カメラ作成
#include "DCCCamera3D.h"
// 太陽光
#include "SunLight.h"
// カメラ
#include "BaseCamera.h"
// 惑星
#include "MovePlanet.h"
// UI描画用モジュール
#include "UIDrawModule.h"
// モデル描画用モジュール
#include "ModelDrawModule.h"
// 文字列ヘッダー
#include <string>


// ========================================
// クラス
// ========================================
class TitleSceneState final : public BaseSceneState
{
private:
	// --------------------------------------
	// オブジェクト変数
	// --------------------------------------
	// DCCカメラ
	DCCCamera3D m_DCCCamera;
	// カメラ
	BaseCamera m_Camera;
	// 太陽光
	SunLight m_SunLight;
	// 惑星移動クラス
	MovePlanet m_MovePlanet;
	// スペースシップ
	MovePlanet m_SpaceShip;


	// --------------------------------------
	// シェーダー名
	// --------------------------------------
	// 頂点シェーダー
	const std::string m_VSName = "VS_Object";
	// ピクセルシェーダー
	const std::string m_PSTextureModelName = "PS_TextureModel";
	const std::string m_PSNoTextureName = "PS_NoTexture";

	// 定数名
	const std::string m_TransformCBName = "Transform";


	// ---------------------------------------
	// テクスチャ名
	// ---------------------------------------
	const std::string m_TitleLogoTextureName = "pipo-halloweenchara2016_02.png";
	const Matrix4x4 m_TitleLogoMat =
		Matrix4x4::CreateTranslationMatrix_LH(Vector3(0.5f, 0.5f, 0.0f)) *
		Matrix4x4::CreateScalingMatrix_LH(Vector3(100.0f, 100.0f, 1.0f));


	// ---------------------------------------
	// モデル情報
	// ---------------------------------------
	// モデル名
	const std::string m_FrogModelName = "Astronaut_FinnTheFrog";
	const Matrix4x4 m_FrogMat = Matrix4x4::CreateTranslationMatrix_LH(Vector3(0.0f,0.0f,5.0f)).toGPU();

	// モデル名
	const std::string m_Female = "Mech_FernandoTheFlamingo";
	const Matrix4x4 m_FemaleMat = Matrix4x4::CreateTranslationMatrix_LH(Vector3(5.0f, 00.0f, 5.0f)).toGPU();

	// モデル
	const std::string m_Terrain = "Base_Large";
	const Matrix4x4 m_TerrainMat = Matrix4x4::CreateTranslationMatrix_LH(Vector3(0.0f, 0.0f, 10.0f)).toGPU();

	// 宙に浮いている惑星
	const std::string m_PlanetModel = "Planet_10";
	const Matrix4x4 m_PlanetMat =
		(Matrix4x4::CreateTranslationMatrix_LH(Vector3(-10.0f, 5.0f, 5.0f)) *
			Matrix4x4::CreateScalingMatrix_LH(Vector3(2.0f, 2.0f, 2.0f))).toGPU();

	// 地面惑星
	const std::string m_PlanetGroundModel = "Planet_6";
	const Matrix4x4 m_PlanetGroundMat =
		(Matrix4x4::CreateTranslationMatrix_LH(Vector3(0.0f, -17.5f, 5.0f)) *
			Matrix4x4::CreateRotationYawPitchRollMatrix_LH(0.0f, 5.0f, 0.0f) *
			Matrix4x4::CreateScalingMatrix_LH(Vector3(20.0f, 20.0f, 20.0f))).toGPU();


	// ----------------------------------------
	// モジュール
	// ----------------------------------------
	// UI描画モジュール
	UIDrawModule m_UIDrawModule;
	// モデル描画モジュール
	ModelDrawModule m_ModelModule;


	// -----------------------------------
	// メンバー関数
	// -----------------------------------
	// シーンの初期化
	bool DerivativeInit() override final;
	// シーンの更新
	void DerivatIveUpdate(float _delta) override final;

public:
	// -----------------------------------
	// コンストラクタ・デストラクタ
	// -----------------------------------
	TitleSceneState() = default;
	~TitleSceneState() = default;


	// シーンの描画
	void Draw() override final;
	// シーンの終了処理
	void Uninit() override final;
};

