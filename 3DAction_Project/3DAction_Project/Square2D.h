#pragma once
#include "BaseObject.h"
#include "Vertex.h"
#include <string>


class Square2D : public BaseObject
{
private:
	// 使用するシェーダーの名前
	const std::string m_VSName = "VS_Object";
	const std::string m_PSName = "PS_Texture";
	// 頂点バッファ名
	const std::string m_VsBufferName = "Square2D";

	// 使用する画像名
	const std::string m_TextureName = "pipo-halloweenchara2016_02.png";

	// 頂点情報
	const Vertex m_Vertices[4] =
	{
		// 左上
		{
			{ -0.5f, 0.5f, 0.0f },        // 位置
			{ -0.5f,  0.5f, 0.0f },      // 法線
			{ 0.0f, 0.0f },              // UV
			{ 1.0f, 0.0f, 0.0f, 1.0f }   // カラー(赤)
		},

		// 右上
		{
			{ 0.5f, 0.5f, 0.0f },
			{  0.5f,  0.5f, 0.0f },
			{ 1.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f, 1.0f }   // カラー(緑)
		},

		// 左下
		{
			{ -0.5f, -0.5f, 0.0f },
			{ -0.5f, -0.5f, 0.0f },
			{ 0.0f, 1.0f },
			{ 0.0f, 0.0f, 1.0f, 1.0f }   // カラー(青)
		},

		// 右下
		{
			{ 0.5f, -0.5f, 0.0f },
			{  0.5f, -0.5f, 0.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f, 0.0f, 1.0f }   // カラー(黄色)
		}
	};

	// 基本関数
	void DerivationInit() override final;

public:
	void Update() override final;
	void Draw() override final;
	void Uninit() override final;

};

