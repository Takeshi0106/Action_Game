// ===================================================
// デバッグ用頂点シェーダ
// ===================================================

// 入力情報（頂点シェーダー用）
struct VSInput
{
    float3 Position : POSITION; // 位置座標
    float3 Normal : NORMAL; // 法線ベクトル
    float4 Color : COLOR; // 頂点カラー（RGBA）
    float2 TexCoord : TEXCOORD0; // テクスチャ座標
    float3 Tangent : TANGENT; // 接ベクトル（必要に応じて）
};

// 出力情報
struct PSInput
{
    float4 Position : SV_POSITION; // 位置座標です.
    float4 ModifiedColor : COLOR0; // 色修正後の色を出力
};

// 定数バッファ1
cbuffer Transform : register(b0)
{
    float4x4 WorldViewProjection; // 変換行列
};

// 定数バッファ2
cbuffer DebugParams : register(b1)
{
    float4 colorModifier; // 色修正用（頂点シェーダ内で使うかは別）
    float brightness; // 明るさ調整用
};

// エントリーポイント
PSInput main(VSInput input)
{
    PSInput output;

    // 変換行列で座標を変換
    output.Position = mul(float4(input.Position, 1.0f), WorldViewProjection);

    // 何か色の計算（例として）を行う
    float4 baseColor = float4(1.0f, 1.0f, 1.0f, 1.0f); // 白色
    float4 modColor = baseColor * colorModifier * brightness;

    output.ModifiedColor = modColor;

    return output;

    return output;
}