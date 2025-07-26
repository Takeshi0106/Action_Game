#pragma once
#include "BaseDirectXManager.h" // �}�l�[�W���[�N���X

// ==============================================
// �O���錾
// ==============================================
struct ID3D11Device;     // DirectX�̃f�o�C�X
class VertexShaderData;  // ���_�V�F�[�_�[
class PixelShaderData;   // �s�N�Z���V�F�[�_
class ComputeShaderData; // �R���s���[�g�V�F�[�_


// ==============================================
// �V�F�[�_�[�̏��
// ==============================================
struct ShaderInfo {
    std::filesystem::path name;  // �V�F�[�_�[�̖��O .hlsl���܂߂Ă�������
    std::string entryPoint;      // �V�F�[�_�[�̃G���g���[�|�C���g
    std::string shaderTypeModel; // �V�F�[�_�[�^�C�v�ƃ��f��������
};


// ===================================================================================================
// �V�F�[�_�[���Ǘ�����N���X  �i�V�F�C�_�[�̃R���p�C���A�o�C�i���[�t�@�C���̃��[�h�A�L���b�V���j
// unordered_map(std::string,shader) �z��ō쐬����
// ===================================================================================================
class ShaderManager : public BaseDirectXManager
{
private:
    const std::string kHlslFailePath;    // .hlsl�������Ă���t�H���_�[�̃p�X

    static std::unordered_map<std::string, VertexShaderData>  m_Vertexs;  // ���_�V�F�[�_�[������z��
    static std::unordered_map<std::string, PixelShaderData>   m_Pixels;   // �s�N�Z���V�F�[�_������z��
    static std::unordered_map<std::string, ComputeShaderData> m_Computes; // �R���s���[�g�V�F�[�_�[������z��

    bool OutputCompileShader(const ShaderInfo info); // �V�F�[�_�[���R���p�C�����ĊO���t�@�C���ɏ����o��

#if defined(DEBUG) || defined(_DEBUG)
    // �f�o�b�O���̂ݗL���ɂ���֐�

#endif

public:

    // �Q�b�^�[
    static VertexShaderData*  GetFindVertexShader (const std::string& name);
    static PixelShaderData*   GetFindPixelShader  (const std::string& name);
    static ComputeShaderData* GetFindComputeShader(const std::string& name);
};

