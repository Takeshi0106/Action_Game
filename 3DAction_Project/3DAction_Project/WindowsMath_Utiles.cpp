#include "WindowsMath_Utiles.h"

// ============================================
// 補助関数
// ============================================
namespace DirectXMathUtiles {

    // FloatをXMVECTORに変換
    DirectX::XMVECTOR ToXMVECTOR(const float& a, const float& b, const float& c, const float& d)
    {
        return DirectX::XMVectorSet(a, b, c, d);
    }

    DirectX::XMVECTOR ToXMVECTOR(const float& a, const float& b, const float& c)
    {
        return ToXMVECTOR(a, b, c, 0.0f);
    }

    DirectX::XMVECTOR ToXMVECTOR(const float& a, const float& b)
    {
        return ToXMVECTOR(a, b, 0.0f);
    }

}