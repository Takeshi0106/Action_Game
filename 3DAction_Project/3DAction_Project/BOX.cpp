
// ==================================
// ヘッダー
// ==================================
// 必須絵ヘッダー
#include "BOX.h"
// 文字列ヘッダー
#include <string>
// 描画マネージャーヘッダー
#include "BaseDrawManager.h"
// 計算ヘッダー
#include "Vector3.h"
#include "Matrix4x4.h"
// ログ出力ヘッダー
#include "ReportMessage.h"
// メッシュマテリアル情報
#include "MaterialData.h"


// ==================================
// 静的メンバー変数定義
// ==================================
const std::string BOX::m_VSName = "VS_Object";
const std::string BOX::m_PSName = "PS_NoTexture";
const std::string BOX::m_VBName = "BOX_VertexBuffer";
const std::string BOX::m_IBName = "BOX_IndexBuffer";
const std::string BOX::m_MaterialName = "Material";
const std::string BOX::m_TransformCBName = "Transform";

const Vertex BOX::m_Vertex[24] = {
	// 前面 (Z-)
	{{-0.5f,  0.5f, -0.5f}, {0,0,-1}, {0,0}, {1,0,0,1}}, // 左上
	{{ 0.5f,  0.5f, -0.5f}, {0,0,-1}, {1,0}, {1,0,0,1}}, // 右上
	{{-0.5f, -0.5f, -0.5f}, {0,0,-1}, {0,1}, {1,0,0,1}}, // 左下
	{{ 0.5f, -0.5f, -0.5f}, {0,0,-1}, {1,1}, {1,0,0,1}}, // 右下

	// 背面 (Z+)
	{{-0.5f,  0.5f,  0.5f}, {0,0,1}, {1,0}, {0,1,0,1}},  // 右上
	{{-0.5f, -0.5f,  0.5f}, {0,0,1}, {0,0}, {0,1,0,1}},  // 左上
	{{ 0.5f,  0.5f,  0.5f}, {0,0,1}, {1,1}, {0,1,0,1}},  // 右下
	{{ 0.5f, -0.5f,  0.5f}, {0,0,1}, {0,1}, {0,1,0,1}},  // 左下

	// 左面 (X-)
	{{-0.5f,  0.5f,  0.5f}, {-1,0,0}, {0,0}, {0,0,1,1}}, // 左上
	{{-0.5f,  0.5f, -0.5f}, {-1,0,0}, {1,0}, {0,0,1,1}}, // 右上
	{{-0.5f, -0.5f,  0.5f}, {-1,0,0}, {0,1}, {0,0,1,1}}, // 左下
	{{-0.5f, -0.5f, -0.5f}, {-1,0,0}, {1,1}, {0,0,1,1}}, // 右下

	// 右面 (X+)
	{{ 0.5f,  0.5f,  0.5f}, {1,0,0}, {0,0}, {0,0,0,1}},  // 左上
	{{ 0.5f, -0.5f,  0.5f}, {1,0,0}, {1,0}, {0,0,0,1}},  // 右上
	{{ 0.5f,  0.5f, -0.5f}, {1,0,0}, {0,1}, {0,0,0,1}},  // 左下
	{{ 0.5f, -0.5f, -0.5f}, {1,0,0}, {1,1}, {0,0,0,1}},  // 右下

	// 上面 (Y+)
	{{-0.5f,  0.5f,  0.5f}, {0,1,0}, {0,1}, {0,0.5,0.5,1}},  // 左後
	{{ 0.5f,  0.5f,  0.5f}, {0,1,0}, {1,1}, {0,0.5,0.5,1}},  // 右後
	{{-0.5f,  0.5f, -0.5f}, {0,1,0}, {0,0}, {0,0.5,0.5,1}},  // 左前
	{{ 0.5f,  0.5f, -0.5f}, {0,1,0}, {1,0}, {0,0.5,0.5,1}},  // 右前

	// 底面 (Y-)
	{{-0.5f, -0.5f,  0.5f}, {0,-1,0}, {0,0}, {0.5,0,0.5,1}}, // 左前
	{{-0.5f, -0.5f, -0.5f}, {0,-1,0}, {1,0}, {0.5,0,0.5,1}}, // 右前
	{{ 0.5f, -0.5f,  0.5f}, {0,-1,0}, {0,1}, {0.5,0,0.5,1}}, // 左後
	{{ 0.5f, -0.5f, -0.5f}, {0,-1,0}, {1,1}, {0.5,0,0.5,1}}, // 右後
};

const uint32_t BOX::m_Index[36] = {
	// 前面 (Z-)
		0, 1, 2,
		2, 1, 3,

		// 背面 (Z+)
		4, 5, 6,
		6, 5, 7,

		// 左面 (X-)
		8, 9, 10,
		10, 9, 11,

		// 右面 (X+)
		12, 13, 14,
		14, 13, 15,

		// 上面 (Y+)
		16, 17, 18,
		18, 17, 19,

		// 底面 (Y-)
		20, 21, 22,
		22, 21, 23
};


// ==================================
// 初期化
// ==================================
bool BOX::Init(BaseDrawManager* drawManager)
{
	// 頂点バッファ作成
	if (!drawManager->CreateVertexBuffer(
		m_VBName.c_str(),
		m_Vertex,
		sizeof(Vertex),
		sizeof(m_Vertex) / sizeof(Vertex),
		sizeof(m_Vertex) / sizeof(Vertex),
		PrimitiveType::TriangleList,
		BufferUsage::Dynamic,
		CPUAccess::Write))
	{
		ErrorLog::OutputToConsole("BOXの頂点バッファ作成に失敗");
		return false;
	}

	// インデックスバッファ作成
	if(!drawManager->CreateIndexBuffer(
		m_IBName.c_str(),
		m_Index,
		sizeof(m_Index) / sizeof(uint16_t)))
	{
		ErrorLog::OutputToConsole("BOXのインデックスバッファ作成に失敗");
		return false;
	}

	return true;
}


// ==================================
// 描画
// ==================================
void BOX::Draw(BaseDrawManager* drawManager, const Color& color)
{
	MeshMaterialData material;
	material.diffuse = color;

	// マテリアル設定
	drawManager->UpdateShaderConstants(
		m_MaterialName.c_str(),
		&material,
		sizeof(MeshMaterialData));

	// 描画
	drawManager->IndexedDraw(m_VSName.c_str(), m_PSName.c_str(), m_VBName.c_str(),m_IBName.c_str());
}


// AABB描画
void BOX::DrawAABB(BaseDrawManager* drawManager, const AABBCollider* aabb, const Color& color)
{
	// サイズ計算
	Vector3 size = aabb->max - aabb->min;

	// 中心位置計算
	Vector3 center = (aabb->min + aabb->max) * 0.5f;

	// ワールド行列計算
	Matrix4x4 world =
		Matrix4x4::CreateTranslationMatrix_LH(center) *
		Matrix4x4::CreateScalingMatrix_LH(size);

	// 定数バッファ更新
	world = world.toGPU();
	drawManager->UpdateShaderConstants(m_TransformCBName.c_str(), &world, sizeof(world));
	
	// マテリアル設定
	MeshMaterialData material;
	material.diffuse = color;

	// マテリアル更新
	drawManager->UpdateShaderConstants(
		m_MaterialName.c_str(),
		&material,
		sizeof(MeshMaterialData));

	// 描画
	drawManager->IndexedDraw(m_VSName.c_str(), m_PSName.c_str(), m_VBName.c_str(), m_IBName.c_str());
}
