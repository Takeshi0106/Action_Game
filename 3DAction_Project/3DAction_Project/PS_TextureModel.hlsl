
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
    float4 Material_Diffse;
    float4 Material_Ambient;
    float4 Material_Specular;
};

// =============================
// ライト情報
// =============================
cbuffer SunLight : register(b1)
{
	// 光の情報
    float4 Light_Diffuse;
    float4 Light_Ambient;
    float4 Light_Specular;
    // ライトの方向
    float3 Light_Direction;
    // パディング
    float pad0;
};


// サンプラーとテクスチャの宣言
Texture2D tex : register(t0);
SamplerState samp : register(s0);

// =============================
// ピクセルシェーダー
// =============================
float4 main(PSInput input) : SV_TARGET
{
    // テクスチャカラーを取得
    float4 color = tex.Sample(samp, input.uv) * Material_Diffse;
    
    float3 N = normalize(input.normal);
    float3 L = normalize(-Light_Direction.xyz);
    
    // ハーフランバート反射
    float lambert = dot(N, L) * 0.5f + 0.5f;
    lambert = saturate(lambert);
    
    // 暗い場所を明るくする
    lambert = max(lambert, 0.7f);
    
    // 拡散反射
    float3 diffuse = lambert * Light_Diffuse.rgb;
    // 
    float3 ambient = Material_Ambient.rgb * Light_Ambient.rgb * 2.0f;
    
    // float4 specular = Light_Specular * Material_Specular;
    
    // 光の明るさを計算
    color.rgb *= (diffuse + ambient);

    return color;
}
