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

cbuffer Transform1 : register(b0)
{
    float4x4 WorldMatrix;
    float4x4 ViewMatrix;
    float4x4 ProjMatrix;
};


PSInput main(VSInput input)
{
    PSInput output;

    float4 worldPos = mul(float4(input.pos, 1.0f), WorldMatrix);
    float4 viewPos = mul(worldPos, ViewMatrix);
    output.pos = mul(viewPos, ProjMatrix); // �� ����� z,w ���������Ȃ�

    output.color = input.color;
    return output;
}