#pragma once
#include "BaseObject.h"
#include "Matrix4x4.h"
#include "Vertex.h"
#include <string>


class Square2D : public BaseObject
{
private:
	// 使用するシェーダーの名前
	// std::string VsName = "VS_Object";
	// std::string PsName = "PS_Texture";

	// 使用するシェーダーの名前
	std::string VsName = "VS_Object";
	std::string PsName = "PS_Texture";

	// 頂点データ
	const Vertex m_Vertices[4] = {
		{ { 1.0f, 0.0f, 0.0f, 1.0f },{ -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f },{  0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f } },
		{ { 0.0f, 0.0f, 1.0f, 1.0f },{ -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f, 1.0f },{  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f } } };

	// SRT
	Matrix4x4 m_SRT;

public:
	void Init(BaseDrawManager* _drawManager) override final;
	void Update() override final;
	void Draw() override final;
	void Uninit() override final;
};

