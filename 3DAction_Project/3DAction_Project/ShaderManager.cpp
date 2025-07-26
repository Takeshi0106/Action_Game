#include "ShaderManager.h"

#include "ShaderData.h"  // �V�F�C�_�[�N���X
#include <wrl/client.h>  // �X�}�[�g�|�C���^�[
#include <fstream>       //�@�t�@�C���̏��o�� 
#include <d3dcompiler.h> // �V�F�[�_�[���R���p�C������w�b�_�[

#pragma comment(lib, "d3dcompiler.lib") // �V�F�[�_�[���R���p�C�����邽��API���g�p���邽�߂̃��C�u�����[


#include <Windows.h>     // �G���[�ƃG���[�̎�ނ��󂯎�邽�߂ɂ���@��ŏ���������
#if defined(DEBUG) || defined(_DEBUG)
#include <iostream> // �E�B���h�E�ɏ����o���悤
#endif

// ================================================
// �ÓI�ϐ�
// ================================================
std::unordered_map<std::string, VertexShaderData> ShaderManager::m_Vertexs;
std::unordered_map<std::string, PixelShaderData> ShaderManager::m_Pixels;
std::unordered_map<std::string, ComputeShaderData> ShaderManager::m_Computes;

// =================================================
// �V�F�[�_�[���擾����֐�
// =================================================



// =================================================
// �V�F�[�_�[���R���p�C������֐�
// =================================================
bool ShaderManager::OutputCompileShader(const ShaderInfo info)
{
	HRESULT hr = S_OK;                                      // ���������s��Ԃ�
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;   // �G���[���擾����
	Microsoft::WRL::ComPtr<ID3DBlob> compileBlob = nullptr; // �R���p�C�������o�C�i���[�f�[�^������

	// �R���p�C���t���O
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS; // �R���p�C�����Ɍ������`�F�b�N����t���O
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG; // �R���p�C�������V�F�[�_�[�Ƀf�o�b�O����t����t���O
#endif

#if !defined(DEBUG) && !defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3; // �ō����x���̍œK�����s���t���O
#endif

	hr = D3DCompileFromFile(
		info.name.wstring().c_str(),            // �V�F�[�_�[�̃t�@�C����
		nullptr,                                // GPU�Ŏg�p����}�N����`�i�Ȃ��ꍇnullptr�j
		D3D_COMPILE_STANDARD_FILE_INCLUDE,      // HLSL�ő���HLSL��ǂݍ��ރt���O
		info.entryPoint.c_str(),                // �V�F�[�_�[�Ȃ��ōŏ��Ɏ��s�����֐��̖��O
		info.shaderTypeModel.c_str(),           // �V�F�[�_�[�̎�ނƃo�[�W����
		dwShaderFlags,                          // �R���p�C���̃t���O
		0,                                      // ���͉����Ȃ��t���O
		compileBlob.GetAddressOf(),             // �R���p�C�������V�F�[�_�[���擾����
		errorBlob.GetAddressOf()                // �G���[���b�Z�[�W���擾����
	);

	if (FAILED(hr)) { // �G���[�擾��
		std::string messegeError = info.name.string() + "�̃R���p�C���Ɏ��s";
		MessageBoxA(nullptr, messegeError.c_str(), "�G���[", MB_OK | MB_ICONERROR); // ���b�Z�[�W�{�b�N�X

#if defined(DEBUG) || defined(_DEBUG)
		if (errorBlob != nullptr) {  // �f�o�b�O���̂ݏڍׂȃG���[�o��
			std::cout << static_cast<const char*>(errorBlob->GetBufferPointer()) << std::endl;
		}
#endif
		return false;
	}

	std::string filename = info.name.stem().string();                            // �g���q�Ȃ��̖��O���擾
	std::filesystem::path outputPath = kFilePath / (filename + ".cso");          // �o�̓p�X���쐬

	std::filesystem::create_directories(outputPath.parent_path()); // �p�X����t�H���_���m�F �Ȃ���΂��쐬���Ă����

	// �����o������
	std::ofstream ofs(outputPath, std::ios::binary | std::ios::out);
	if (!ofs) {
		MessageBoxA(nullptr, "CSO�t�@�C���̏������݂Ɏ��s���܂����B", "�G���[", MB_OK | MB_ICONERROR);
		return false;
	}

	ofs.write(static_cast<const char*>(compileBlob->GetBufferPointer()), compileBlob->GetBufferSize());   // �o�C�i���f�[�^�������o��
	ofs.close();                                                                                          // �t�@�C�������

	// �ꉞ���
	errorBlob.Reset();
	compileBlob.Reset();

	return true;
}


// ���_�V�F�[�_�[��T���֐�
VertexShaderData* ShaderManager::GetFindVertexShader(const std::string& name)
{
	auto it = m_Vertexs.find(name);
	if (it != m_Vertexs.end()) {
		return &(it->second);
	}
	return nullptr; // ������Ȃ�����
}
// �s�N�Z���V�F�[�_��T���֐�
PixelShaderData* ShaderManager::GetFindPixelShader(const std::string& name)
{
	auto it = m_Pixels.find(name);
	if (it != m_Pixels.end()) {
		return &(it->second);
	}
	return nullptr; // ������Ȃ�����
}
// �R���s���[�g�V�F�[�_��T���֐�
ComputeShaderData* ShaderManager::GetFindComputeShader(const std::string& name)
{
	auto it = m_Computes.find(name);
	if (it != m_Computes.end()) {
		return &(it->second); 
	}
	return nullptr; // ������Ȃ�����
}