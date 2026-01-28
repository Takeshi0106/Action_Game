
// =============================
// ヘッダー
// =============================
// 必須ヘッダー
#include "TitleSceneState.h"


// ============================	
// シーンの初期化
// ============================
bool TitleSceneState::DerivativeInit()
{
	// DCCカメラ初期化
	m_DCCCamera.SetInput(m_Modules->input);
	if (!m_DCCCamera.Init(m_Modules->drawManager)) {
		return true;
	}
	// カメラ初期化
	m_Camera.Init(m_Modules->drawManager);

	// プロジェクション行列（透視投影）
	float fov = 3.14159265f / 4.0f;
	float aspect = 1280.0f / 720.0f;
	float nearZ = 0.1f;
	float farZ = 100.0f;
	CameraProjInfo proj = { fov, aspect, nearZ, farZ };

	// カメラに設定
	m_DCCCamera.SetProjection(proj);
	m_Camera.SetProjection(proj);

	m_Camera.SetCameraPos(Vector3(7.7f, 5.7f, -9.9f));
	m_Camera.SetLookPoint(Vector3(7.2f, 5.7f, -9.0f));

	// 太陽光初期化
	if (!m_SunLight.Init(m_Modules->drawManager)) {
		return false;
	}


	// 定数バッファ作成
	Matrix4x4 world;
	world = world.toGPU();

	m_Modules->drawManager->CreateConstantBuffer(
		m_TransformCBName,
		&world,
		sizeof(world),
		BufferUsage::Dynamic,
		CPUAccess::Write);

	// UI描画モジュール初期化
	m_UIDrawModule.Init(*m_Modules->drawManager,
		m_TitleLogoTextureName);

	// モジュール初期化
	m_ModelModule.Init(*m_Modules->drawManager,
		m_FrogModelName,
		u8"Character/Nomal");

	m_ModelModule.CreateModel(
		m_Female,
		u8"Character/Flamingo");

	m_ModelModule.CreateModel(
		m_Terrain,
		u8"Terrain");

	m_ModelModule.CreateModel(
		m_PlanetGroundModel,
		u8"Planet/Planet6");

	// 移動惑星初期化
	m_MovePlanet.SetMoveOriginPos(Vector3(0.0f, -37.5f, 7.0f));
	m_MovePlanet.SetDistance(60.0f);
	m_MovePlanet.SetRotateSpeed(0.5f);
	m_MovePlanet.SetAxis(Vector3(0.0f, 1.0f, -1.3f));
	m_MovePlanet.SetScale(Vector3(2.0f, 2.0f, 2.0f));

	m_MovePlanet.Init(
		m_Modules->drawManager,
		u8"Planet_10",
		u8"Planet/Planet10");

	// スペースシップ初期化
	m_SpaceShip.SetMoveOriginPos(Vector3(0.0f, 0.0f, 0.0f));
	m_SpaceShip.SetDistance(60.0f);
	m_SpaceShip.SetRotateSpeed(1.5f);
	m_SpaceShip.SetAxis(Vector3(0.0f, 0.5f, -0.25f));
	m_SpaceShip.SetScale(Vector3(1.0f, 1.0f, 1.0f));

	m_SpaceShip.Init(
		m_Modules->drawManager,
		u8"Spaceship_BarbaraTheBee",
		u8"Spaceship");

	return true;
}


// ============================
// シーンの更新
// ============================
void TitleSceneState::DerivatIveUpdate(float _delta)
{
	// カメラ更新
	m_DCCCamera.Update();
	// ライト更新
	m_SunLight.Update();

	m_MovePlanet.Update(_delta);
	m_SpaceShip.Update(_delta);

	// シーン遷移
	if (m_Modules->input->GetKeyTrigger(KeyCode_Enter)) {
		m_SceneEvent = SceneEventID::DEBUGSCENE;
	}

	if (m_Modules->input->GetKeyTrigger(KeyCode_Escape)) {
		m_SceneEvent = SceneEventID::STOP_GAME;
	}
}


// ============================
// シーンの描画
// ============================
void TitleSceneState::Draw()
{
	if (m_Modules->input->GetMousePress(MouseCode::Mouse_Right))
	{
		// DCCカメラ情報をGPUに送る
		m_DCCCamera.UpdateToGPU();
	}
	else
	{
		// カメラ情報をGPUに送る
		m_Camera.UpdateToGPU();
	}

	// ライト情報をGPUに送る
	m_SunLight.UpdateToGPU();

	// 描画切り替え
	m_Modules->drawManager->SetDepthStencilSetting(DepthStencilSetting::DepthEnableON_DepthWriteON);
	m_Modules->drawManager->SetDrawSetting(FillModeSetting::Solid, CullingSetting::Back_Culling);

	// 定数バッファ更新
	m_Modules->drawManager->UpdateShaderConstants(
		m_TransformCBName,
		&m_FrogMat,
		sizeof(m_FrogMat));

	// モデル描画
	m_ModelModule.Draw(m_FrogModelName, 
		m_VSName, 
		m_PSTextureModelName);
	
	// 定数バッファ更新
	m_Modules->drawManager->UpdateShaderConstants(
		m_TransformCBName,
		&m_FemaleMat,
		sizeof(m_FemaleMat));

	// モデル描画
	m_ModelModule.Draw(m_Female,
		m_VSName,
		m_PSTextureModelName);

	// 定数バッファ更新
	m_Modules->drawManager->UpdateShaderConstants(
		m_TransformCBName,
		&m_TerrainMat,
		sizeof(m_TerrainMat));

	// モデル描画
	m_ModelModule.Draw(m_Terrain,
		m_VSName,
		m_PSTextureModelName);

	// 定数バッファ更新
	m_Modules->drawManager->UpdateShaderConstants(
		m_TransformCBName,
		&m_PlanetGroundMat,
		sizeof(m_PlanetGroundMat));

	// モデル描画
	m_ModelModule.Draw(
		m_PlanetGroundModel,
		m_VSName,
		m_PSTextureModelName);

	// 移動惑星描画
	m_MovePlanet.Draw();

	// スペースシップ描画
	m_SpaceShip.Draw();

	// 描画切り替え
	m_Modules->drawManager->SetDrawSetting(FillModeSetting::Solid, CullingSetting::Not_Culling);
	m_Modules->drawManager->SetDepthStencilSetting(DepthStencilSetting::DepthEnableOFF_DepthWriteOFF);

	// 定数バッファ更新
	m_Modules->drawManager->UpdateShaderConstants(
		m_TransformCBName,
		&m_TitleLogoMat,
		sizeof(m_TitleLogoMat));
	
	// UI描画
	m_UIDrawModule.Draw(m_TitleLogoTextureName);
}


// ============================
// シーンの終了処理
// ============================
void TitleSceneState::Uninit()
{
	// 移動惑星後処理
	m_MovePlanet.Uninit();
	// スペースシップ後処理
	m_SpaceShip.Uninit();

	// 太陽光後処理
	m_SunLight.Uninit();
	// カメラ後処理
	m_Camera.Uninit();
	m_DCCCamera.Uninit();
	// モジュール後処理
	m_ModelModule.Uninit();
}
