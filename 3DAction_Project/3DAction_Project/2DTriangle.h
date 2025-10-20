#pragma once

// ヘッダー
#include "BaseObject.h"
#include <string>
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Color.h"
#include "Vertex.h"


class Triangle2D : public BaseObject
{
private:
	// 使用するシェーダーの名前
	std::string VsName = "VS_TriangleDebug";
	std::string PsName = "PS_TriangleDebug";

	// 頂点
	const Vertex m_Vertices[3] = {
		{ { 1.0f, 0.0f, 0.0f, 1.0f } ,{ 0.0f, 0.57735f, 0.0f } },
		{  { 0.0f, 1.0f, 0.0f, 1.0f },{ 0.5f, -0.288675f, 0.0f } },
		{ { 0.0f, 0.0f, 1.0f, 1.0f } ,{ -0.5f, -0.288675f, 0.0f } }
	};

	// 位置
	Matrix4x4 m_SRT;

public:
	void Init(BaseDrawManager* _drawManager) override;
	void Update() override;
	void Draw() override;
	void Uninit() override;
};