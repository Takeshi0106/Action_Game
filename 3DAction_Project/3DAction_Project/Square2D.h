#pragma once
#include "BaseObject.h"
#include "Matrix4x4.h"
#include "Vertex.h"
#include <string>


class Square2D : public BaseObject
{
private:
	// 使用するシェーダーの名前
	const std::string m_VSName = "VS_Object";
	const std::string m_PSName = "PS_Texture";
	// このオブジェクトが更新する定数バッファ名
	const std::string m_CBName = "Transform1";

	// 使用する画像名
	const std::string m_TextureName = "pipo-halloweenchara2016_02.png";

	// 頂点データ
	const Vertex m_Vertices[4] = {
		{ { 1.0f, 0.0f, 0.0f, 1.0f },{ -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f },{  0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f } },
		{ { 0.0f, 0.0f, 1.0f, 1.0f },{ -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f, 1.0f },{  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f } } };

	// SRT情報
	Matrix4x4 m_SRT;

public:
	// 基本関数
	void Init(BaseDrawManager* _drawManager) override final;
	void Update() override final;
	void Draw() override final;
	void Uninit() override final;

};

