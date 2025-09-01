#pragma once

// ==========================================
// Math構造体 Vector2
// 
// 中身をプラットフォームごとに変更する
// C++17基準
// noexcept はthrow例外を投げないことを保証するものです
// もし、他のプラットフォームで使うことがある場合は外してください
// ==========================================


// ==========================================
// Vector2 型
// ==========================================
struct Vector2
{
	// メンバー変数
	float x, y;

	// コンストラクタ / デストラクタ
	constexpr Vector2() noexcept : x(0.0f), y(0.0f) {}
	constexpr Vector2(float _x, float _y) noexcept : x(_x), y(_y) {}
	constexpr Vector2(const Vector2& vec) noexcept : x(vec.x), y(vec.y) {}
	~Vector2() noexcept = default;

	// 代入演算子
	Vector2& operator=(const Vector2& vec2) noexcept = default;

	// 単項演算子
	Vector2 operator+ () const noexcept { return *this; }
	Vector2 operator- () const noexcept { return Vector2(-x, -y); }

	// チェック演算子
	constexpr bool operator==(const Vector2& vec2) const noexcept
	{
		return x == vec2.x && y == vec2.y;
	}
	constexpr bool operator!=(const Vector2& vec2) const noexcept
	{
		return !(*this == vec2);
	}

	// 計算関数
	float Dot(const Vector2& vec1) const noexcept; // 内積
	float Cross(const Vector2& vec1) const noexcept; // 外積
	float Length() const noexcept; // 長さ
	Vector2 Normalize() const noexcept; // 正規化
	float Distance(const Vector2& vec1) const noexcept; // 距離
	float DistanceSquared(const Vector2& vec1) const noexcept;// 距離の２乗
	Vector2 Lerp(const Vector2& vec1, float t) const noexcept; // 線形補間

	Vector2 toGPU() const noexcept; // GPUに送るデータに変換
};


// 演算
Vector2 operator+(const Vector2& vec1, const Vector2& vec2) noexcept;
Vector2 operator-(const Vector2& vec1, const Vector2& vec2) noexcept;
Vector2 operator*(const Vector2& vec1, float scalar) noexcept;
Vector2 operator/(const Vector2& vec1, float scalar) noexcept;