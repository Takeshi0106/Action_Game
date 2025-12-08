#pragma once

// ヘッダー
#include "BaseObject.h"
#include <string>
#include "Vertex.h"


class Triangle2D : public BaseObject
{
private:
	// 使用するシェーダーの名前
	std::string m_VsName = "VS_TriangleDebug";
	std::string m_PsName = "PS_TriangleDebug";

    // 頂点バッファ名
	std::string m_VsBufferName = "Triangle2D";

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

    void LateInit() override final;

public:
	void Update() override final;
	void Draw() override final;
	void Uninit() override final;
};
