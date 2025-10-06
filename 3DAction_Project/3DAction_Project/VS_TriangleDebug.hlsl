// ==================================================
// 頂点入力
// ==================================================
struct VSInput
{
    float3 pos : POSITION; // 頂点位置
    float4 color : COLOR; // 頂点カラー
};

// ==================================================
// 頂点シェーダーからピクセルシェーダーへの出力
// ==================================================
struct PSInput
{
    float4 pos : SV_POSITION; // クリッピング後の座標
    float4 color : COLOR; // 頂点カラー
};

// ==================================================
// 定数バッファ
// ==================================================
cbuffer Transform1 : register(b0)
{
    float4x4 WorldMatrix;
    float4x4 ViewMatrix;
    float4x4 ProjMatrix;
};

// ==================================================
// 頂点シェーダー
// ==================================================
PSInput main(VSInput input)
{
    PSInput vsOut;

    // ワールド変換
    vsOut.pos = mul(WorldMatrix, float4(input.pos, 1.0f));

    // ビュー変換
    vsOut.pos = mul(ViewMatrix, vsOut.pos);

    // 投影変換
    vsOut.pos = mul(ProjMatrix, vsOut.pos);

    // 頂点カラーはそのまま渡す
    vsOut.color = input.color;

    return vsOut;
}
