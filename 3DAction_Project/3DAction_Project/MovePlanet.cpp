
// =================================
// ヘッダー
// =================================
#include "MovePlanet.h"


// =================================
// 初期化
// =================================
void MovePlanet::Init(
	BaseDrawManager* _draw,
	const Hashed_String& _modelName,
	const String& _modelFile)
{
	m_Draw = _draw;

	// 定数バッファ作成
	m_Draw->CreateConstantBuffer(
		m_TransformCBName,
		&m_SRT.world,
		sizeof(m_SRT.world),
		BufferUsage::Dynamic,
		CPUAccess::Write);

	// モデルのロード
	m_Draw->LoadModel(_modelName,_modelFile);
	m_ModelName = _modelName;

	// 初期位置（X方向に距離分）
	m_InitialPos = Vector3(m_Distance, 0, 0);
}


// =================================
// 更新
// =================================
void MovePlanet::Update(float _time)
{
	m_TotalAngle += m_RotateSpeed * _time;

	Quaternion q = Quaternion::CreateQuaternionFromAxisAngle(
		m_Axis,
		m_TotalAngle);

	m_SRT.position = m_MoveOriginPos + q.RotateVector(m_InitialPos);
	// m_SRT.rotation = q;
}


// =================================
// 描画
// =================================
void MovePlanet::Draw()
{
	Matrix4x4 worldMatrix = m_SRT.UpdateWorldMatrix().toGPU();

	// 定数バッファ更新
	m_Draw->UpdateShaderConstants(
		m_TransformCBName,
		&worldMatrix,
		sizeof(Matrix4x4));

	// モデル描画
	m_Draw->ModelDraw(
		m_VSName,
		m_PSName,
		m_ModelName);
}


// =================================
// 後処理
// =================================
void MovePlanet::Uninit()
{

}
