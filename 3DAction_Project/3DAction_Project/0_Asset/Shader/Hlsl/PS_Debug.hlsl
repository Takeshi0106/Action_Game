// ===============================================
// デバッグ用ピクセルシェーダ 
// ===============================================

// 入力情報
struct PSInput
{
    float4 Position : SV_POSITION0; // 位置座標
};

// 定数バッファ（例）
cbuffer DebugParams : register(b0)
{
    float4 colorModifier; // 色に乗算する係数
    float brightness; // 明るさの調整用
};


// エントリーポイント
float4 main(PSInput output) : SV_TARGET0
{
    float4 baseColor = float4(0.25f, 1.0f, 0.25f, 1.0f);
    float4 modColor = baseColor * colorModifier;
    modColor.rgb += brightness;
    return modColor;
}