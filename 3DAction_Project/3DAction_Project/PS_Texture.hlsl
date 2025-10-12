
struct PSInput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

// �T���v���[�ƃe�N�X�`���̐錾
Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PSInput input) : SV_TARGET
{
    // �e�N�X�`���J���[���擾
    float4 texColor = tex.Sample(samp, input.uv);
    
    return texColor;
}