// ===============================================
// デバッグ用ピクセルシェーダ 
// ===============================================

// 入力情報
struct PSInput
{
    float4 Position : SV_POSITION0; // 位置座標
};

// エントリーポイント
float4 main(PSInput output) : SV_TARGET0
{
    return float4(0.25f, 1.0f, 0.25f, 1.0f);
}