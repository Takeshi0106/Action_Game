#include "PlatformWindowsSystem.h"
#include <Windows.h>
#include "Timer.h"

// =====================================================
// �ÓI�����o�[�ϐ�
// =====================================================
HINSTANCE PlatformWindowsSystem::m_AppInstance = nullptr;
HWND         PlatformWindowsSystem::m_WinInstance = nullptr;
uint16_t     PlatformWindowsSystem::m_Width = 0;
uint16_t     PlatformWindowsSystem::m_Height = 0;
std::wstring PlatformWindowsSystem::m_WindowName;
std::wstring PlatformWindowsSystem::m_WindowClassName;


// =====================================================
// �f�o�b�O�p
// =====================================================
#if defined(DEBUG) || defined(_DEBUG)
namespace {
    bool IsUninit = false;
}
#endif


// =====================================================
// �v���g�^�C�v�錾
// =====================================================
static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);


// =====================================================
// �R���X�g���N�^
// =====================================================
PlatformWindowsSystem::PlatformWindowsSystem(uint16_t Width, uint16_t Height, std::wstring WindowClassName, std::wstring WindowName)
{
    // ������
    m_Width = Width;
    m_Height = Height;
    m_WindowClassName = WindowClassName;
    m_WindowName = WindowName;
}


// =====================================================
// �f�X�g���N�^
// =====================================================
PlatformWindowsSystem::~PlatformWindowsSystem()
{
#if defined(DEBUG) || defined(_DEBUG)
    if (IsUninit)
    {
        std::cout << "PlatformWindowsSystem : �㏈�������s����Ă��܂���" << std::endl;
        Uninit();
    }
#endif
}


// =====================================================
// ����������
// =====================================================
bool PlatformWindowsSystem::Init()
{
    // �C���X�^���X�n���h���擾
   m_AppInstance = GetModuleHandle(nullptr);
    if (m_AppInstance == nullptr) { return false; } // �擾�Ɏ��s������false��Ԃ�

    // �E�B���h�E���ݒ�
    WNDCLASSEX windClass = {};
    windClass.cbSize = sizeof(WNDCLASSEX);                           // �\���̂̃T�C�Y�ݒ� (�o�[�W�����𔻒肵�Ă��邽�ߕK�{)  
    windClass.style = CS_HREDRAW | CS_VREDRAW;                       // �T�C�Y�ύX���ɍĕ`��L���ɂ���
    windClass.lpfnWndProc = WndProc;                                 // �E�B���h�E�v���V�[�W���֐��̃|�C���^�[
    windClass.hIcon = LoadIcon(m_AppInstance, IDI_APPLICATION);   // �E�B���h�E�̍���̃A�C�R���@�i�W���A�C�R���ō쐬 .ico�ŕύX�\)
    windClass.hCursor = LoadCursor(m_AppInstance, IDC_CROSS);     // �N���X�J�[�\���\������ �i�f�U�C���j
    windClass.hbrBackground = GetSysColorBrush(COLOR_BACKGROUND);    // �V�X�e���̃f�t�H���g�w�i�F�ŃE�B���h�E�̔w�i��h��
    windClass.lpszMenuName = nullptr;                                // �E�B���h�E�̃��j���[���쐬���Ȃ�
    windClass.lpszClassName = m_WindowClassName.c_str();             // �E�B���h�E�̖��O�ݒ�
    windClass.hIconSm = LoadIcon(m_AppInstance, IDI_APPLICATION); // �^�X�N�o�[�ɕ\�������A�C�R�� (�W���A�C�R���ō쐬 .ico�ŕύX�\)

    // �E�B���h�E�̓o�^ ���s������false��Ԃ�
    if (!RegisterClassEx(&windClass)) { return false; }

    // �`�悷��傫����ݒ�
    RECT rect = {};
    rect.right = static_cast<LONG>(m_Width);   // ��
    rect.bottom = static_cast<LONG>(m_Height); // �c

    // �E�B���h�E�̑傫�����v�Z�@�i�`�悷��傫���{�g�j
    DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX; // �g��ݒ� (�W���ȃE�B���h�E�A�^�C�g���o�[����A�V�X�e�����j���[����)
    AdjustWindowRect(&rect, style, FALSE); // rect�Ɍv�Z�����傫������

    // �E�B���h�E�쐬
    m_WinInstance = CreateWindowEx(
        0,                          // �E�B���h�E�̓���〈���ځi�f�t�H���g�j
        m_WindowClassName.c_str(),  // �E�B���h�E�̃N���X��
        m_WindowName.c_str(),       // �E�B���h�E�̃^�C�g���o�[�ŕ\������閼�O
        style,                      // �E�B���h�E�̃X�^�C��
        CW_USEDEFAULT,              // �E�B���h�E�̕\���ʒu��OS�Ɍ��߂Ă��炤
        CW_USEDEFAULT,              // �E�B���h�E�̕\���ʒu��OS�Ɍ��߂Ă��炤
        rect.right - rect.left,     // �E�B���h�E�̉����ݒ�
        rect.bottom - rect.top,     // �E�B���h�E�̏c���ݒ�
        nullptr,                    // �e�E�B���h�E�Ȃ�
        nullptr,                    // ���j���[�Ȃ�
        m_AppInstance,              // �C���X�^���X�n���h��
        nullptr);                   // �ǉ��p�����[�^�[

    // �E�B���h�E���쐬�ł������̃`�F�b�N
    if (m_WinInstance == nullptr) { return false; }

    // �E�B���h�E��\��
    ShowWindow(m_WinInstance, SW_SHOWNORMAL);

    // �E�B���h�E���X�V
    UpdateWindow(m_WinInstance);

    // �E�B���h�E�ɓ��͏����擾������
    SetFocus(m_WinInstance);

    // ����I��.
    return true;
}


// =====================================================
// �Q�[�����[�v�@�i�Q�[���{�ҁj
// =====================================================
void PlatformWindowsSystem::GameLoop()
{
    MSG msg = {}; // ���b�Z�[�W

    Timer::Init(); // �^�C�}�[������
    Timer::Start(); // �^�C�}�[�J�n

    while (true)
    {
#if defined(DEBUG) || defined(_DEBUG)
        Timer::Debug_CheckUpdate(); // �^�C�}�[�f�o�b�O
#endif
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE) // ���b�Z�[�W���󂯎��
        {
            TranslateMessage(&msg); // �L�[���͂Ȃǂ𕶎���ɕϊ�����֐�
            DispatchMessage(&msg); // �E�B���h�E�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
        }
        else
        {


        }
        Timer::LastUpdate(); // �^�C�}�[�X�V����
    }
}


// =====================================================
// �㏈��
// =====================================================
void PlatformWindowsSystem::Uninit()
{
    // �E�B���h�E�̓o�^������
    if (m_AppInstance != nullptr)
    {
        UnregisterClass(m_WindowClassName.c_str(), m_AppInstance);
    }

    m_AppInstance = nullptr;
    m_WinInstance = nullptr;
}


// =====================================================
// �E�B���h�E�v���V�[�W��
// =====================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_DESTROY: // �E�B���h�E�j��
        PostQuitMessage(0); // �I������
        break;

    case WM_CLOSE:
    {
        int res = MessageBoxA(NULL, "�I�����܂����H", "�m�F", MB_OKCANCEL);
        if (res == IDOK) // ���b�Z�[�W�{�b�N�X
        {
            DestroyWindow(hWnd); // �E�B���h�E�폜
        }
        break;
    }

    case WM_KEYDOWN:
        if (LOWORD(wp) == VK_ESCAPE)
        {
            PostMessage(hWnd, WM_CLOSE, wp, lp);//�E�B���h�E�v���V�[�W����WM_CLOSE�𑗂�
        }
        break;

    default:
        return DefWindowProc(hWnd, msg, wp, lp);
        break;

    }

    return 0;
}