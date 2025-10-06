// ==================================================
// ���_����
// ==================================================
struct VSInput
{
    float3 pos : POSITION; // ���_�ʒu
    float4 color : COLOR; // ���_�J���[
};

// ==================================================
// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̏o��
// ==================================================
struct PSInput
{
    float4 pos : SV_POSITION; // �N���b�s���O��̍��W
    float4 color : COLOR; // ���_�J���[
};

// ==================================================
// �萔�o�b�t�@
// ==================================================
cbuffer Transform1 : register(b0)
{
    float4x4 WorldMatrix;
    float4x4 ViewMatrix;
    float4x4 ProjMatrix;
};

// ==================================================
// ���_�V�F�[�_�[
// ==================================================
PSInput main(VSInput input)
{
    PSInput vsOut;

    // ���[���h�ϊ�
    vsOut.pos = mul(WorldMatrix, float4(input.pos, 1.0f));

    // �r���[�ϊ�
    vsOut.pos = mul(ViewMatrix, vsOut.pos);

    // ���e�ϊ�
    vsOut.pos = mul(ProjMatrix, vsOut.pos);

    // ���_�J���[�͂��̂܂ܓn��
    vsOut.color = input.color;

    return vsOut;
}
