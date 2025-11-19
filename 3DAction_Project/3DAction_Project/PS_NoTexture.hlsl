
struct PSInput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

cbuffer Material : register(b0)
{
    float4 diffse : COLOR0;
    float4 ambient : COLOR1;
    float4 specular : COLOR2;
};

float4 main(PSInput input) : SV_TARGET
{
    return diffse * input.color;
}
