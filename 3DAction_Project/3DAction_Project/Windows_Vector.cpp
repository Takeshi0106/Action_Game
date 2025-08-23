
// ======================================
// ヘッダー
// ======================================
// 必須ヘッダー
#include "Vector.h"
// ユーリアリティー関数
#include "WindowsMath_Utiles.h"
// 計算ヘッダー
#include <DirectXMath.h>


// ======================================
// DirectXTKと同じ、毎計算floatをXMVECTORに変換して、
// 計算、floatに変換を行っています
// 
// ＊注意
// SIMDに対応していますが、毎計算時に変換しているため、
// そこまで速くなっていないと思います。
// ======================================


// ========================================
// プロトタイプ宣言
// ========================================
// XMVECTORをVector3に変換
Vector3 FromXMVECTOR3(const DirectX::XMVECTOR& vec);


// ============================
// コンストラクタ
// ============================
Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
Vector3::Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
Vector3::Vector3(const Vector3& vec) : x(vec.x), y(vec.y), z(vec.z) {}


// ============================
// 演算子
// ============================
// 代入演算子
Vector3& Vector3::operator=(const Vector3& vec)
{
    x = vec.x;
    y = vec.y;
    z = vec.z;

    return *this;
}

// 加算演算子
Vector3 Vector3::operator+(const Vector3& vec) const
{    
    // 計算
    DirectX::XMVECTOR add = DirectX::XMVectorAdd(
        DirectXMathUtiles::ToXMVECTOR(x, y, z), 
        DirectXMathUtiles::ToXMVECTOR(vec.x, vec.y, vec.z));

    return FromXMVECTOR3(add);
}

// 減算代入子
Vector3 Vector3::operator-(const Vector3& vec) const
{
    // 計算
    DirectX::XMVECTOR subtract = DirectX::XMVectorSubtract(
        DirectXMathUtiles::ToXMVECTOR(x, y, z), 
        DirectXMathUtiles::ToXMVECTOR(vec.x, vec.y, vec.z));

    return FromXMVECTOR3(subtract);
}

// 乗算演算子
Vector3 Vector3::operator*(float scalar) const
{ 
    // 計算
    DirectX::XMVECTOR multiplication = DirectX::XMVectorScale(
        DirectXMathUtiles::ToXMVECTOR(x,y,z), 
        scalar);

    return FromXMVECTOR3(multiplication);
}


// ============================
// 計算関数
// ============================
// 内積
float Vector3::Dot(const Vector3& vec) const
{
    // 計算
    return DirectX::XMVectorGetX(
        DirectX::XMVector3Dot(
        DirectXMathUtiles::ToXMVECTOR(x, y, z),
        DirectXMathUtiles::ToXMVECTOR(vec.x, vec.y, vec.z)));
}

// 外積
Vector3 Vector3::Cross(const Vector3& vec) const
{
    // 計算
    DirectX::XMVECTOR cross = DirectX::XMVector3Cross(
        DirectXMathUtiles::ToXMVECTOR(x, y, z),
        DirectXMathUtiles::ToXMVECTOR(vec.x, vec.y, vec.z));

    return FromXMVECTOR3(cross);
}

// 長さ
float Vector3::Length() const
{
    // 計算
    return DirectX::XMVectorGetX(
        DirectX::XMVector3Length(
        DirectXMathUtiles::ToXMVECTOR(x,y,z)));
}

// 正規化
Vector3 Vector3::Normalize() const
{
    // 計算
    DirectX::XMVECTOR normalize = DirectX::XMVector3Normalize(
    DirectXMathUtiles::ToXMVECTOR(x,y,z));

    // 戻す
    return FromXMVECTOR3(normalize);
}


// =====================================
// 関数
// =====================================
// XMVECTORをVector3に変換
Vector3 FromXMVECTOR3(const DirectX::XMVECTOR& vec)
{
    // 変換
    DirectX::XMFLOAT3 out;
    DirectX::XMStoreFloat3(&out, vec);
    return Vector3(out.x, out.y, out.z);
}