#include "PlatformSystem.h"

// ========================================================
// �v���b�g�t�H�[�����Ƃɏ�����ύX�ł�����N���X
// ========================================================

#if defined(DEBUG) || defined(_DEBUG)
bool PlatformSystem::m_IsCreated = false; // �f�o�b�O���ɃR���p�C��
#endif

// �N��
void PlatformSystem::Execute()
{
	if (Init()) // ������
	{
		GameLoop(); // �Q�[�����[�v
	}
	Uninit(); // �I������
}