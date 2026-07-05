
// ==================================
// ヘッダー
// ==================================
#include "UIDrawModule.h"



// ==================================
// 頂点情報初期化
// ==================================
Vertex UIDrawModule::m_Vertices[4] = {
	// 左上
	{ { 0.0f, 0.0f, 0.1f }, {0,0,-1}, {0,0}, {1,1,1,1} },
	// 右上
	{ { 1.0f, 0.0f, 0.1f }, {0,0,-1}, {1,0}, {1,1,1,1} },
	// 左下
	{ { 0.0f, 1.0f, 0.1f }, {0,0,-1}, {0,1}, {1,1,1,1} },
	// 右下
	{ { 1.0f, 1.0f, 0.1f }, {0,0,-1}, {1,1}, {1,1,1,1} },
};


// ==================================
// 初期化
// ==================================
void UIDrawModule::Init(IDrawManager& _draw, const Hashed_String& _textureName)
{
	// 描画マネージャー保存
	m_Draw = &_draw;

	// 頂点情報更新
	m_Vertices[0].position = Vector3(0.0f, 0.0f, 0.0f);
	m_Vertices[1].position = Vector3((float)m_Draw->GetScreenWidth(), 0.0f, 0.0f);
	m_Vertices[2].position = Vector3(0.0f, (float)m_Draw->GetScreenHeight(), 0.0f);
	m_Vertices[3].position = Vector3((float)m_Draw->GetScreenWidth(), (float)m_Draw->GetScreenHeight(), 0.0f);

	// 頂点バッファ作成
	m_Draw->CreateVertexBuffer(
		m_VSBufferName,
		m_Vertices,
		sizeof(m_Vertices),
		sizeof(m_Vertices) / sizeof(Vertex),
		sizeof(m_Vertices) / sizeof(Vertex),
		PrimitiveType::TriangleStrip,
		BufferUsage::Dynamic,
		CPUAccess::Write);

	// 正射影行列作成
	Matrix4x4 orthoMatrix;	
	orthoMatrix = Matrix4x4::CreateUIOrthoMatrix_LH(
		(float)m_Draw->GetScreenWidth(),
		(float)m_Draw->GetScreenHeight(),
		0.0f,
		1.0f);
	orthoMatrix = orthoMatrix.toGPU();

	// 定数バッファ作成
	m_Draw->CreateConstantBuffer(
		m_CSBufferName,
		&orthoMatrix,
		sizeof(orthoMatrix),
		BufferUsage::Dynamic,
		CPUAccess::Write);

	// テクスチャ読み込み
	LoadTexture(_textureName);
}


// ==================================
// テクスチャ読み込み
// ==================================
bool UIDrawModule::LoadTexture(const Hashed_String& _textureName)
{
	return m_Draw->LoadTexture(_textureName);
}


// ==================================
// 描画
// ==================================
void UIDrawModule::Draw(const Hashed_String& _textureName)
{
	// プリミティブ描画
	m_Draw->PrimitiveDraw(
		m_VSShaderName,
		m_PSShaderName,
		m_VSBufferName,
		_textureName);
}
