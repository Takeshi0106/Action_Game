
// ===============================
// 頂点バッファ
// ===============================
struct VS_IN
{
    float3 pos : POSITION; // 頂点位置
    float3 normal : NORMAL; // 法線
    float2 uv : TEXCOORD; // UV
    float4 color : COLOR; // 頂点カラー
};


// ===============================
// 出力情報
// ===============================
struct VS_OUT
{
    float4 pos : SV_POSITION; // クリッピング後の座標
    float3 normal : NORMAL; // 法線
    float2 uv : TEXCOORD; // UV
    float4 color : COLOR; // 頂点カラー
};


// ==================================================
// 定数バッファ
// ==================================================
cbuffer UIOrthoMatrix : register(b0)
{
    float4x4 ortho;
};

cbuffer Transform : register(b1)
{
    float4x4 srt;
};


// ===============================
// 頂点シェーダー
// ===============================
VS_OUT main( VS_IN vin) 
{
    VS_OUT vout;
    
    // スクリーン座標を作成
    float4 worldPos = mul(float4(vin.pos, 1.0f), srt);
    
    // 正射影変換
    vout.pos = mul(worldPos, ortho);
    
    // そのまま
    vout.normal = vin.normal;
    vout.uv = vin.uv;
    vout.color = vin.color;

    
	return vout;
}
