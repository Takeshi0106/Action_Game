#pragma once

// ヘッダー
#include <string>
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Color.h"

// 構造体
struct Vertex {
	Vector3 pos;
	Color color;
};
struct TransformCB
{
	Matrix4x4 WorldMatrix;
	Matrix4x4 ViewMatrix;
	Matrix4x4 ProjMatrix;
};

class Triangle
{
private:
	// 使用するシェーダーの名前
	std::string VsName = "VS_TriangleDebug";
	std::string PsName = "PS_TriangleDebug";

public:

};