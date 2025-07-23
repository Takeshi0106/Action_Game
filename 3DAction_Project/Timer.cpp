#include "Timer.h"
#include <Windows.h>

#if defined(DEBUG) || defined(_DEBUG)
// �f�o�b�O���Ƀw�b�_�[���R���p�C��
#include <cassert>
#endif

// �ÓI�����o�[�ϐ�
namespace Timer {
	namespace {
		// =====================================================
		// �ϐ�
		// =====================================================
		LARGE_INTEGER frequency; // ��b�Ԃ̌o�ߎ���
		LARGE_INTEGER lastTime;  // �O��̌v�����Ԃ�ۑ�����
		LARGE_INTEGER startTime; // �v���J�n����

#if defined(DEBUG) || defined(_DEBUG)
		// �f�o�b�O���ȊO�ł̓R���p�C�����Ȃ�
		bool IsInit = false;
		bool IsStart = false;
		bool IsCheck = false;
		unsigned int CollCount = 0;
		unsigned int LastCount = 1;
#endif
	}


	// =====================================================
    // �O���錾���\���̂Ƃ��Ē�`
    // =====================================================
	struct TimePoint {
		LARGE_INTEGER time;

		// �R���X�g���N�^
		TimePoint(const LARGE_INTEGER& largeInteger) : time(largeInteger) {}
	};


	// =====================================================
	// ������
	// =====================================================
	void Init()
	{
		QueryPerformanceFrequency(&frequency); // ���g�����擾
		QueryPerformanceCounter(&lastTime);    // ���������Ă΂ꂽ���Ԃ��擾
#if defined(DEBUG) || defined(_DEBUG)
		IsInit = true; // �f�o�b�O���ȊO�ł̓R���p�C������Ȃ�
#endif
	}


	// =====================================================
	// �v���J�n���Ԃ��擾����
	// =====================================================
	void Start()
	{
		QueryPerformanceCounter(&startTime); // �v���J�n���Ԃ��擾
#if defined(DEBUG) || defined(_DEBUG)
		IsStart = true; // �f�o�b�O���ȊO�ł̓R���p�C������Ȃ�
#endif
	}


#if defined(DEBUG) || defined(_DEBUG)
	// =====================================================
	// �f�o�b�N���ɂ̂ݗL��
	// ���t���[��LastUpdate���Ă΂�Ă��邩�`�F�b�N����N���X
	// =====================================================
	void Debug_CheckUpdate()
	{
		if (LastCount == CollCount)
		{
			assert(false && "LastUpdate()���O�̃t���[���ŌĂ΂�Ă��܂���");
		}
		assert(!IsCheck && "Debug_CheckeUpdate()��������Ă΂�Ă��܂�");
		LastCount = CollCount;
		IsCheck = true;
	}
#endif


	// =====================================================
	// �Ō�ɌĂяo��
	// =====================================================
	void LastUpdate()
	{
#if defined(DEBUG) || defined(_DEBUG)
		if (LastCount < CollCount)
		{
			assert(false && "LastUpdate()��������Ă΂�Ă��܂�");
		}
		CollCount++;
		IsCheck = false;
#endif
		QueryPerformanceCounter(&lastTime);    // ���������Ă΂ꂽ���Ԃ��擾
	}


	// =====================================================
	// �O�̃t���[������̌o�ߎ��Ԃ�Ԃ�
	// =====================================================
	float GetDeltaTime()
	{
#if defined(DEBUG) || defined(_DEBUG)
		assert(IsInit && "�������������Ă΂�Ă��܂���"); // �������t���O��false�̂Ƃ��v���O�����������I��
#endif
		// ���̎��Ԃ��擾
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);
		// �O�̃J�E���g�ƍ��̃J�E���g�������Ď��g���Ŋ���A�O�̃t���[���Ƃ̎��Ԃ��擾
		return static_cast<float>(currentTime.QuadPart - lastTime.QuadPart) / frequency.QuadPart;
	}


	// =====================================================
	// �������œn���ꂽ���Ԃ���̌v�����Ԃ�Ԃ�
	// =====================================================
	float GetElapsedTime(const TimePoint& _startTime)
	{
#if defined(DEBUG) || defined(_DEBUG)
		assert(IsInit && "�������������Ă΂�Ă��܂���");    // �������t���O��false�̂Ƃ��v���O�����������I��
#endif
		// ���̎��Ԃ��擾
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);
		// �J�E���g�����g���Ŋ����Čo�ߎ��Ԃ����߂�
		return static_cast<float>(currentTime.QuadPart - _startTime.time.QuadPart) / frequency.QuadPart;
	}


	// =====================================================
	// �v���J�n���Ԃ���̌o�ߎ��Ԃ�Ԃ��֐�
	// =====================================================
	float GetElapsedTime()
	{
#if defined(DEBUG) || defined(_DEBUG)
		assert(IsInit && "�������������Ă΂�Ă��܂���");    // �������t���O��false�̂Ƃ��v���O�����������I��
		assert(IsStart && "�v���J�n��������΂�Ă��܂���"); // �v���J�n�t���O��false�̂Ƃ��v���O�����������I��
#endif
		// ���̎��Ԃ��擾
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);
		// �J�E���g�����g���Ŋ����Čo�ߎ��Ԃ����߂�
		return static_cast<float>(currentTime.QuadPart - startTime.QuadPart) / frequency.QuadPart;
	}
}