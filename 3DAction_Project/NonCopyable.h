#pragma once

// ==============================================
// �R�s�[���֎~����N���X
// �����E�j���͉\
// ==============================================

class NonCopyable
{
protected:
	// �����������Ȃ����Ƃ�����邽�߂ɖ�������
	NonCopyable() = default;
	~NonCopyable() = default;


	NonCopyable(const NonCopyable&) = delete; // �R�s�[�R���X�g���N�^�֎~


	NonCopyable& operator=(const NonCopyable&) = delete; // �R�s�[���Z�q���֎~
};

