
// ================================
// 入力情報
// ================================
struct PSInput
{
    float4 pos : SV_POSITION; // クリッピング後の座標
    float3 normal : NORMAL; // 法線
    float2 uv : TEXCOORD; // UV
    float4 color : COLOR; // 頂点カラー
};


// ================================
// サンプラーとテクスチャの宣言
// ================================
Texture2D tex : register(t0);
SamplerState samp : register(s0);


// ================================
// ピクセルシェーダー
// ================================
float4 main(PSInput input) : SV_TARGET
{
    // テクスチャカラーを取得
    float4 texColor = tex.Sample(samp, input.uv);
    
    texColor = float4(1.0, 0.0, 0.0, 1.0);
    return texColor;
}
