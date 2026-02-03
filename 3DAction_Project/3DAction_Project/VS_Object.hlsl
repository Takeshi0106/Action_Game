// ==================================================
// 頂点入力
// ==================================================
struct VS_IN
{
    float3 pos : POSITION0; // 頂点位置
    float3 normal : NORMAL0; // 法線
    float2 uv : TEXCOORD0; // UV
    float4 color : COLOR0; // 頂点カラー
};

// ==================================================
// 頂点シェーダーからピクセルシェーダーへの出力
// ==================================================
struct VS_OUT
{
    float4 pos : SV_POSITION0; // クリッピング後の座標
    float3 normal : NORMAL0; // 法線
    float2 uv : TEXCOORD0; // UV
    float4 color : COLOR0; // 頂点カラー
};

// ==================================================
// 定数バッファ
// ==================================================
cbuffer CameraInfo : register(b0)
{
    float4x4 view;
    float4x4 proj;
};

cbuffer Transform : register(b1)
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
    vout.pos = mul(world,vout.pos); // ワールド座標
    vout.pos = mul(view, vout.pos); // ビュー座標
    vout.pos = mul(proj, vout.pos); // プロジェクション座標
    
    // 法線をワールド変換する
    vout.normal = mul((float3x3) world, vin.normal);
    
    // そのまま返す
    vout.color = vin.color;
    vout.uv = vin.uv;

    return vout;
}
