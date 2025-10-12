// ==================================================
// 頂点入力
// ==================================================
struct VS_IN
{
    float3 pos : POSITION; // 頂点位置
    float4 color : COLOR; // 頂点カラー
};

// ==================================================
// 頂点シェーダーからピクセルシェーダーへの出力
// ==================================================
struct VS_OUT
{
    float4 pos : SV_POSITION; // クリッピング後の座標
    float4 color : COLOR; // 頂点カラー
};

// ==================================================
// 定数バッファ
// ==================================================
cbuffer CameraInfo : register(b0)
{
    float4x4 view;
    float4x4 proj;
};

cbuffer Transform1 : register(b1)
{
    float4x4 world;
};


// ==================================================
// 頂点シェーダー
// ==================================================
VS_OUT main(VS_IN vin)
{
    // PixelShaderに渡すデータ
    VS_OUT vout;

    // スクリーン座標を作成
    vout.pos = float4(vin.pos, 1.0f); // ローカル座標
    vout.pos = mul(vout.pos, world); // ワールド座標
    vout.pos = mul(vout.pos, view); // ビュー座標
    vout.pos = mul(vout.pos, proj); // プロジェクション座標
    
    // そのまま返す
    vout.color = vin.color;

    return vout;
}
