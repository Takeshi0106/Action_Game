#pragma once

// ===============================
// 【クラス概要】
// ===============================


// ===============================
// ヘッダー
// ===============================
#include "Vector2.h"
#include "Vector3.h"
#include <vector>


// ===============================
// クラス
// ===============================
// 頂点情報
struct Vertex {
	Vector3 position;
	Vector3 normal;
	Vector2 uv;
};


// メッシュ情報
struct MeshData
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::string texturePath;
};
