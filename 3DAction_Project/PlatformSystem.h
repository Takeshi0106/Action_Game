#pragma once
#include "NonCopyable.h"

#if defined(DEBUG) || defined(_DEBUG)
#include <cassert> // �f�o�b�O���Ƀw�b�_�[���R���p�C���@(DebugLog)���o������ύX
#endif

// =============================================================================================
// �v���b�g�t�H�[�����ƂŌp�����āA�������E�㏈�����|�����[�t�B�Y��������N���X
// ���̃N���X���p�������N���X�͂P���������ł��Ȃ����Ƃ�ۏ؂���
// =============================================================================================

class PlatformSystem : public NonCopyable
{
public:
#if defined(DEBUG) || defined(_DEBUG)
	static bool m_IsCreated; // �P�����Ȃ����Ƃ�ۏ؂���
#endif

private:
	// -----------------------------------------------------------------
	// �v���b�g�t�H�[�����ƂɃI�[�o�[���C�h����
	// -----------------------------------------------------------------
	virtual bool Init() = 0;	    // ������ (�߂�l�ŏ�������������Ԃ�)
	virtual void Uninit() = 0;		// �㏈��
	virtual void GameLoop() = 0;    // �Q�[�����[�v

public:
	explicit PlatformSystem() // �R���X�g���N�^
	{
#if defined(DEBUG) || defined(_DEBUG)
		// �f�o�b�O���̂݊m�F����
		if (m_IsCreated)
		{
			assert(false && "PlatformSystem�N���X��������������Ă��܂�");
		}
		m_IsCreated = true;
#endif
	}

	~PlatformSystem()// �f�X�g���N�^
	{
#if defined(DEBUG) || defined(_DEBUG)
		m_IsCreated = false;
#endif
	}

	void Execute(); // �N������
};