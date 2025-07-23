#pragma once
#include "PlatformSystem.h"
#include <string>


// =====================================================
// �O���錾
// =====================================================
// �A�v���P�[�V�����n���h���̑O���錾
struct APPLICATIONHANDLE;              // HINSTANCE�̃��b�v�\����
// �E�B���h�E�n���h���̑O���錾
struct WINDOWHANDLE;                   // HWND�̃��b�v�\����

// =====================================================
// �E�B���h�E�v���b�g�t�H�[���̏������E�㏈���N���X
// =====================================================
class PlatformWindowsSystem : public PlatformSystem
{
private:
	static APPLICATIONHANDLE m_AppInstance; // �A�v���P�[�V�����̃n���h��
	static WINDOWHANDLE m_WinInstance;      // �E�B���h�E�n���h��
	static uint16_t  m_Width;	            // �E�B���h�E�̉�ʉ��� 
	static uint16_t  m_Height;	            // �E�B���h�E�̉�ʏc��
	static std::wstring m_WindowName;       // �E�B���h�E�̖��O
	static std::wstring m_WindowClassName;  // �E�B���h�E�̃N���X��

	bool Init() override;     // ����������
	void GameLoop() override; // �Q�[�����[�v
	void Uninit() override;   // �㏈��

public:
	// �R���X�g���N�^�E�f�X�g���N�^
	PlatformWindowsSystem(uint16_t Width, uint16_t Height, std::wstring WindowClassName, std::wstring WindowName);
	~PlatformWindowsSystem();

	// �Q�b�^�[
	static uint16_t GetWidthSize() { return m_Width; }       // �E�B���h�E�̉�����߂�l�ŕԂ�
	static uint16_t GetHeightSize() { return m_Height; }	 // �E�B���h�E�̏c����߂�l�ŕԂ�
};