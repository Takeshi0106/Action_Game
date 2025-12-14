
// ==================================
// ヘッダー
// ==================================
// 必須ヘッダー
#include "Board.h"
// 描画マネージャーヘッダー
#include "BaseDrawManager.h"
// メッシュマテリアル情報
#include "MaterialData.h"
// ログ出力ヘッダー
#include "ReportMessage.h"


// ==================================
// 静的メンバー変数定義
// ==================================
const std::string Board::m_VSName = "VS_Object";
const std::string Board::m_PSName = "PS_NoTexture";
const std::string Board::m_VBName = "Board_VertexBuffer";
const std::string Board::m_MaterialName = "Material";
const std::string Board::m_TransformCBName = "Transform";

const Vertex Board::m_Vertex[4] = {
	// 位置　　　　　　　法線　　　　　　UV
	{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
	{{ 0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
	{{-0.5f,-0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
	{{ 0.5f,-0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
};



// ==================================
// 初期化
// ==================================
bool Board::Init(BaseDrawManager* _drawManager)
{
	// 頂点バッファ作成
	if (!_drawManager->CreateVertexBuffer(
		m_VBName.c_str(),
		m_Vertex,
		sizeof(Vertex),
		sizeof(m_Vertex) / sizeof(Vertex),
		sizeof(m_Vertex) / sizeof(Vertex),
		PrimitiveType::TriangleStrip,
		BufferUsage::Dynamic,
		CPUAccess::Write))
	{
		ErrorLog::OutputToConsole("BOXの頂点バッファ作成に失敗");
		return false;
	}

	return true;
}


// ==================================
// 描画
// ==================================
void Board::Draw(BaseDrawManager* drawManager, const Color& color)
{
	MeshMaterialData material;
	material.diffuse = color;

	// マテリアル設定
	drawManager->UpdateShaderConstants(
		m_MaterialName.c_str(),
		&material,
		sizeof(MeshMaterialData));

	// 描画
	drawManager->PrimitiveDraw(m_VSName.c_str(), m_PSName.c_str(), m_VBName.c_str());
}
