#pragma once
#include <filesystem>           // �t�@�C���p�X�Ȃǁ@C++17�ȍ~
#include <string>               // ������
#include <unordered_map>        // �n�b�V���l�z��

// ==================================================================
// DirectX�Ŏg�p����}�l�[�W���[�̊��N���X
// �V�F�[�_�[�E�e�N�X�`���E�I�u�W�F�N�g�Ȃǂɔh��������
// ==================================================================

class BaseDirectXManager
{
protected:
	const  std::filesystem::path kFilePath;     // �t�@�C�����Q�Ƃ��ɍs���p�X
	const  std::filesystem::path kAssetLogPath; // �A�Z�b�g�̃��O �f�o�b�O�r���h���ɏ��o���A�����[�X�r���h���ɃA�Z�b�g�����邩���m�F���� 

public:
	BaseDirectXManager(std::filesystem::path file, std::filesystem::path assetLog) : kFilePath(file), kAssetLogPath(assetLog) {}; // �R���X�g���N�^
	~BaseDirectXManager() = default;                                   // �f�X�g���N�^


};

