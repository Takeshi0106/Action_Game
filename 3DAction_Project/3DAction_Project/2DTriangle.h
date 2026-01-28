#pragma once

// ヘッダー
#include "BaseObject.h"
#include "UTF8_String.h"
#include "Vertex.h"


class Triangle2D : public BaseObject
{
private:
	// 使用するシェーダーの名前
	String m_VsName = u8"VS_TriangleDebug";
	String m_PsName = u8"PS_TriangleDebug";

    // 頂点バッファ名
	String m_VsBufferName = u8"Triangle2D";

	// 頂点
    const Vertex m_Vertices[3] =
    {
        // 1つ目の頂点
        {
            { 1.0f, 0.0f, 0.0f },            // 位置
            { 0.0f, 0.57735f, 0.0f },        // 法線
            { 0.0f, 0.0f },                  // UV（とりあえず左下）
            { 1.0f, 0.0f, 0.0f, 1.0f }       // 赤
        },

        // 2つ目の頂点
        {
            { 0.0f, 1.0f, 0.0f },
            { 0.5f, -0.288675f, 0.0f },
            { 1.0f, 0.0f },                  // UV（右下）
            { 0.0f, 1.0f, 0.0f, 1.0f }       // 緑
        },

        // 3つ目の頂点
        {
            { 0.0f, 0.0f, 1.0f },
            { -0.5f, -0.288675f, 0.0f },
            { 0.5f, 1.0f },                  // UV（上中央）
            { 0.0f, 0.0f, 1.0f, 1.0f }       // 青
        }
    };

    void DerivationInit() override final;

public:
	void Update() override final;
	void Draw() override final;
	void Uninit() override final;
};
