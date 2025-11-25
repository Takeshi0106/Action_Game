
// =============================
// 入力情報
// =============================
struct PSInput
{
    float4 pos : SV_POSITION; // クリッピング後の座標
    float3 normal : NORMAL; // 法線
    float2 uv : TEXCOORD; // UV
    float4 color : COLOR; // 頂点カラー
};


// =============================
// 定数バッファ
// =============================
cbuffer Material : register(b0)
{
    float4 diffse;
    float4 ambient;
    float4 specular;
};


// =============================
// ピクセルシェーダー
// =============================
float4 main(PSInput input) : SV_TARGET
{
    return diffse;
}
