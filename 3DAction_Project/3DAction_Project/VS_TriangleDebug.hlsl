struct VSInput
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

cbuffer Transform : register(b0)
{
    float4x4 WorldMatrix;
};


PSInput main(VSInput input)
{
    PSInput output;

    // ƒ[ƒ‹ƒh•ÏŠ·
    output.pos = mul(float4(input.pos, 1.0f), WorldMatrix);
    output.color = input.color;

    return output;
}