
// =============================
// 入力情報
// =============================
struct PSInput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
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
