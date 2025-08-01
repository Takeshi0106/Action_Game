// ===================================================
// デバッグ用頂点シェーダ
// ===================================================

// 入力情報
struct VSInput
{
    float3 Position : POSITION; // 位置座標
};

// 出力情報
struct PSInput
{
    float4 Position : SV_POSITION; // 位置座標です.
};

// エントリーポイント
PSInput main(VSInput input)
{

    PSInput output;
  
      // 入力データをそのまま流す.
    output.Position = float4(input.Position, 1.0f);
  
    return output;
}