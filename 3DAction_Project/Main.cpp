#include "Main.h"

// �������[���[�N���o�p
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

int main(void)
{
#if defined(DEBUG) || defined(_DEBUG)
	// �f�o�b�O���Ƀ��������[�N�𔭌��ł���悤�ɂ���
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// �E�B���h�E�Y�v���b�g�t�H�[�����쐬
	PlatformWindowsSystem system{ SCREEN_WIDTH,SCREEN_HEIGHT,WINDOW_NAME,WINDOW_CLASS_NAME };
	system.Execute(); // �N��

	return 0;
}