// ==================================================
// ���_����
// ==================================================
struct VS_IN
{
    float3 pos : POSITION; // ���_�ʒu
    float4 color : COLOR; // ���_�J���[
};

// ==================================================
// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̏o��
// ==================================================
struct VS_OUT
{
    float4 pos : SV_POSITION; // �N���b�s���O��̍��W
    float4 color : COLOR; // ���_�J���[
};

// ==================================================
// �萔�o�b�t�@
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
// ���_�V�F�[�_�[
// ==================================================
VS_OUT main(VS_IN vin)
{
    // PixelShader�ɓn���f�[�^
    VS_OUT vout;

    // �X�N���[�����W���쐬
    vout.pos = float4(vin.pos, 1.0f); // ���[�J�����W
    vout.pos = mul(vout.pos, world); // ���[���h���W
    vout.pos = mul(vout.pos, view); // �r���[���W
    vout.pos = mul(vout.pos, proj); // �v���W�F�N�V�������W
    
    // ���̂܂ܕԂ�
    vout.color = vin.color;

    return vout;
}
