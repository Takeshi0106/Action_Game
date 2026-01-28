
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
// 文字列ヘッダー
#include "UTF8_String.h"


// ==================================
// 静的メンバー変数定義
// ==================================
const String Board::m_VSName = u8"VS_Object";
const String Board::m_PSName = u8"PS_NoTexture";
const String Board::m_VBName = u8"Board_VertexBuffer";
const String Board::m_MaterialName = u8"Material";
const String Board::m_TransformCBName = u8"Transform";

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
		m_VBName,
		m_Vertex,
		sizeof(Vertex),
		sizeof(m_Vertex) / sizeof(Vertex),
		sizeof(m_Vertex) / sizeof(Vertex),
		PrimitiveType::TriangleStrip,
		BufferUsage::Dynamic,
		CPUAccess::Write))
	{
		ErrorLog::OutputToConsole(u8"BOXの頂点バッファ作成に失敗");
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
		m_MaterialName,
		&material,
		sizeof(MeshMaterialData));

	// 描画
	drawManager->PrimitiveDraw(m_VSName, m_PSName, m_VBName);
}
