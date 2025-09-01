#pragma once

// ==========================================
// Math構造体　Vector3
// 
// 中身をプラットフォームごとに変更する
// C++17で作成しています
// noexcept はthrow例外を投げないことを保証するものです
// もし、他のプラットフォームで使うことがある場合は外してください
// ==========================================


// ==========================================
// Vector3 型
// ==========================================
struct Vector3
{
	// メンバー変数
	float x, y, z;

	// コンストラクタ / デストラクタ
	constexpr Vector3() noexcept : x(0.0f), y(0.0f), z(0.0f) {}
	constexpr Vector3(float _x, float _y, float _z) noexcept : x(_x), y(_y), z(_z) {}
	constexpr Vector3(const Vector3& vec) noexcept : x(vec.x), y(vec.y), z(vec.z) {}
	~Vector3() noexcept = default;

	// 代入演算子
	Vector3& operator=(const Vector3& vec3) noexcept = default;

	// チェック演算子
	constexpr bool operator==(const Vector3& vec) const noexcept {
		return x == vec.x && y == vec.y && z == vec.z;
	}
	constexpr bool operator!=(const Vector3& vec) const noexcept {
		return !(*this == vec);
	}

	// 単項演算子
	Vector3 operator+ () const noexcept { return *this; }
	Vector3 operator- () const noexcept { return Vector3(-x, -y, -z); }

	// 計算関数// 計算関数
	float Dot(const Vector3& vec1) const noexcept; // 内積
	Vector3 Cross(const Vector3& vec1) const noexcept; // 外積
	float Length() const noexcept; // 長さ
	Vector3 Normalize() const noexcept; // 正規化
	float Distance(const Vector3& vec1) const noexcept; // 距離
	float DistanceSquared(const Vector3& vec1) const noexcept; // 距離の２乗
	Vector3 Lerp(const Vector3& vec1, float t) const noexcept; // 線形補間

	Vector3 toGPU() const noexcept; // GPUに渡す値に変換する
};


// 演算子
Vector3 operator+(const Vector3& vec1, const Vector3& vec2) noexcept;
Vector3 operator-(const Vector3& vec1, const Vector3& vec2) noexcept;
Vector3 operator*(const Vector3& vec1, float scalar) noexcept;
Vector3 operator/(const Vector3& vec1, float scalar) noexcept;