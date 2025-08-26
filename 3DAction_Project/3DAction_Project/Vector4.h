#pragma once

// ==========================================
// Math構造体　Vector4
// 
// 中身をプラットフォームごとに変更する
// C++17基準
// noexcept はthrow例外を投げないことを保証するものです
// もし、他のプラットフォームで使うことがある場合は外してください
// ==========================================


// ==========================================
// Vector4 型
// ==========================================
struct Vector4
{
	// メンバー変数
	float x, y, z, w;

	// コンストラクタ / デストラクタ
	Vector4() noexcept : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	Vector4(float _x, float _y, float _z, float _w)  noexcept : x(_x), y(_y), z(_z), w(_w) {}
	Vector4(const Vector4& vec) noexcept : x(vec.x), y(vec.y), z(vec.z), w(vec.w) {}
	~Vector4() noexcept = default;

	// 代入演算子
	Vector4& operator=(const Vector4& vec4) noexcept = default;

	// チェック演算子
	bool operator==(const Vector4& vec) const noexcept {
		return x == vec.x && y == vec.y && z == vec.z && w == vec.w;
	}
	bool operator!=(const Vector4& vec) const noexcept {
		return !(*this == vec);
	}

	// 単項演算子
	Vector4 operator+ () const noexcept { return *this; }
	Vector4 operator- () const noexcept { return Vector4(-x, -y, -z, -w); }

	// 計算関数
	// 計算関数
	float Dot(const Vector4& vec1) const noexcept; // 内積
	float Length() const noexcept; // 長さ
	Vector4 Normalize() const noexcept; // 正規化
	float Distance(const Vector4& vec1) const noexcept; // 距離
	float DistanceSquared(const Vector4& vec1) const noexcept; // 距離の２乗
	Vector4 Lerp(const Vector4& vec1, float t) const noexcept; // 線形補間
};


// 演算子
Vector4 operator+(const Vector4& vec1, const Vector4& vec2) noexcept;
Vector4 operator-(const Vector4& vec1, const Vector4& vec2) noexcept;
Vector4 operator*(const Vector4& vec1, float scalar) noexcept;
Vector4 operator/(const Vector4& vec1, float scalar) noexcept;