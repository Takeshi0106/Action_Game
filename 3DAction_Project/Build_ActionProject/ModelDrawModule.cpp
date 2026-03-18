
// ===================================
// ヘッダー
// ===================================
// 必須ヘッダー
#include "ModelDrawModule.h"



// ===================================
// 初期化
// ===================================
void ModelDrawModule::Init(
	BaseDrawManager& _draw, 
	const Hashed_String& _modelName,
	const String& _filyName)
{
	// 描画マネージャー保存
	m_Draw = &_draw;
	CreateModel(_modelName, _filyName);
}


// ====================================
// 描画
// ====================================
void ModelDrawModule::Draw(
	const Hashed_String& _modelName,
	const Hashed_String& _vsName,
	const Hashed_String& _psName)
{
	m_Draw->ModelDraw(
		_vsName,
		_psName,
		_modelName);
}


// ====================================
// 後処理
// ====================================
void ModelDrawModule::Uninit()
{
	// 今はなにも行わない
}


// ===================================
// モデル名セッター
// ===================================
void ModelDrawModule::CreateModel(
	const Hashed_String& _modelName,
	const String& _fileName)
{
	// モデルのロード
	if (m_Draw != nullptr)
	{
		// ファイル名が空かチェックはモデルマネージャー側が行っています。
		m_Draw->LoadModel(_modelName, _fileName);
	}
}
