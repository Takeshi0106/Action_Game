#include "PlatformWindowsSystem.h"
#include "Timer.h"
#include "DirectX.h"

#if defined(DEBUG) || defined(_DEBUG)
#include <iostream>
#endif


// =====================================================
// 前方宣言を構造体として定義 　（ラッパー構造体）
// =====================================================
struct APPLICATIONHANDLE {
private: // 直接アクセス不可
    HINSTANCE hInstance; // アプリケーションハンドル

public:
    // コンストラクタ
    APPLICATIONHANDLE(const HINSTANCE& _hInstance) : hInstance(_hInstance) {}
    // 代入演算子のオーバーロード
    APPLICATIONHANDLE& operator=(const HINSTANCE& _hInstance) 
    {
        hInstance = _hInstance;
        return *this;
    }
    // 暗黙的に変換を禁止にする
    explicit operator HINSTANCE() const { return hInstance; }
    // ラップしているHINSTANCEを返す
    HINSTANCE Get() const { return hInstance; }
};

struct WINDOWHANDLE {
private: // 直接アクセス不可
    HWND hWnd; // ウィンドウハンドル

public:
    // コンストラクタ
    WINDOWHANDLE(const HWND& _hWnd) : hWnd(_hWnd) {}
    // 代入演算子オーバーロード
    WINDOWHANDLE& operator=(const HWND& _hWnd) 
    {
        hWnd = _hWnd;
        return *this;
    }
    // 暗黙的に変換を禁止にする
    explicit operator HWND() const { return hWnd; }
    // ラップしているHWNDを返す
    HWND Get() const { return hWnd; }
};


// =====================================================
// 静的メンバー変数
// =====================================================
APPLICATIONHANDLE PlatformWindowsSystem::m_AppInstance = nullptr;
WINDOWHANDLE      PlatformWindowsSystem::m_WinInstance = nullptr;
uint16_t          PlatformWindowsSystem::m_Width = 0;
uint16_t          PlatformWindowsSystem::m_Height = 0;
std::wstring      PlatformWindowsSystem::m_WindowName;
std::wstring      PlatformWindowsSystem::m_WindowClassName;


// =====================================================
// デバッグ用
// =====================================================
#if defined(DEBUG) || defined(_DEBUG)
namespace {
    bool IsUninit = false;
}
#endif


// =====================================================
// プロトタイプ宣言
// =====================================================
static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);


// =====================================================
// コンストラクタ
// =====================================================
PlatformWindowsSystem::PlatformWindowsSystem(uint16_t Width, uint16_t Height, std::wstring WindowClassName, std::wstring WindowName)
{
    // 初期化
    m_Width = Width;
    m_Height = Height;
    m_WindowClassName = WindowClassName;
    m_WindowName = WindowName;
}


// =====================================================
// デストラクタ
// =====================================================
PlatformWindowsSystem::~PlatformWindowsSystem()
{
#if defined(DEBUG) || defined(_DEBUG)
    if (IsUninit)
    {
        std::cout << "PlatformWindowsSystem : 後処理が実行されていません" << std::endl;
        Uninit();
    }
#endif
}


// =====================================================
// 初期化処理
// =====================================================
bool PlatformWindowsSystem::Init()
{
    // インスタンスハンドル取得
   m_AppInstance = GetModuleHandle(nullptr);
    if (m_AppInstance.Get() == nullptr) { return false; } // 取得に失敗したらfalseを返す

    // ウィンドウ情報設定
    WNDCLASSEX windClass = {};
    windClass.cbSize = sizeof(WNDCLASSEX);                                  // 構造体のサイズ設定 (バージョンを判定しているため必須)  
    windClass.style = CS_HREDRAW | CS_VREDRAW;                              // サイズ変更時に再描画有効にする
    windClass.lpfnWndProc = WndProc;                                        // ウィンドウプロシージャ関数のポインター
    windClass.hIcon = LoadIcon(m_AppInstance.Get(), IDI_APPLICATION);       // ウィンドウの左上のアイコン　（標準アイコンで作成 .icoで変更可能)
    windClass.hCursor = LoadCursor(m_AppInstance.Get(), IDC_ARROW);         // クロスカーソル表示する （デザイン）
    windClass.hbrBackground = GetSysColorBrush(COLOR_BACKGROUND);           // システムのデフォルト背景色でウィンドウの背景を塗る
    windClass.lpszMenuName = nullptr;                                       // ウィンドウのメニューを作成しない
    windClass.lpszClassName = m_WindowClassName.c_str();                    // ウィンドウの名前設定
    windClass.hIconSm = LoadIcon(m_AppInstance.Get(), IDI_APPLICATION);     // タスクバーに表示されるアイコン (標準アイコンで作成 .icoで変更可能)

    // ウィンドウの登録 失敗したらfalseを返す
    if (!RegisterClassEx(&windClass)) { return false; } 

    // 描画する大きさを設定
    RECT rect = {};
    rect.right = static_cast<LONG>(m_Width);   // 横
    rect.bottom = static_cast<LONG>(m_Height); // 縦

    // ウィンドウの大きさを計算　（描画する大きさ＋枠）
    DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME;; // 枠を設定 (標準なウィンドウ、タイトルバーあり、システムメニューあり)
    AdjustWindowRect(&rect, style, FALSE); // rectに計算した大きさを代入

    // ウィンドウ作成
    m_WinInstance = CreateWindowEx(
        0,                          // ウィンドウの動作や見た目（デフォルト）
        m_WindowClassName.c_str(),  // ウィンドウのクラス名
        m_WindowName.c_str(),       // ウィンドウのタイトルバーで表示される名前
        style,                      // ウィンドウのスタイル
        CW_USEDEFAULT,              // ウィンドウの表示位置をOSに決めてもらう
        CW_USEDEFAULT,              // ウィンドウの表示位置をOSに決めてもらう
        rect.right - rect.left,     // ウィンドウの横幅設定
        rect.bottom - rect.top,     // ウィンドウの縦幅設定
        nullptr,                    // 親ウィンドウなし
        nullptr,                    // メニューなし
        m_AppInstance.Get(),        // インスタンスハンドル
        nullptr);                   // 追加パラメーター

    // ウィンドウを作成できたかのチェック
    if (m_WinInstance.Get() == nullptr) { return false; }

    // ウィンドウを表示
    ShowWindow(m_WinInstance.Get(), SW_SHOWNORMAL);

    // ウィンドウを更新
    UpdateWindow(m_WinInstance.Get());

    // ウィンドウに入力情報を取得させる
    SetFocus(m_WinInstance.Get());

    // 正常終了.
    return true;
}


// =====================================================
// ゲームループ　（ゲーム本編）
// =====================================================
void PlatformWindowsSystem::GameLoop()
{
    MSG msg = {}; // メッセージ

    DirectX11::Init(m_Width, m_Height, m_WinInstance.Get());

    Timer::Init(); // タイマー初期化
    Timer::Start(); // タイマー開始

    while (true)
    {
#if defined(DEBUG) || defined(_DEBUG)
        Timer::Debug_CheckUpdate(); // タイマーデバッグ
#endif
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE) // メッセージを受け取る
        {
            if (msg.message == WM_QUIT) { break; } // ウィンドウ削除を受け取ったらループを抜ける

            TranslateMessage(&msg); // キー入力などを文字列に変換する関数
            DispatchMessage(&msg); // ウィンドウプロシージャにメッセージを送る
        }
        else
        {
            DirectX11::DebugDraw(Timer::GetElapsedTime());

        }
        Timer::LastUpdate(); // タイマー更新処理
    }
}


// =====================================================
// 後処理
// =====================================================
void PlatformWindowsSystem::Uninit()
{
    // ウィンドウの登録を解除
    if (m_AppInstance.Get() != nullptr)
    {
        UnregisterClass(m_WindowClassName.c_str(), m_AppInstance.Get());
    }

    m_AppInstance = nullptr;
    m_WinInstance = nullptr;
}


// =====================================================
// ウィンドウプロシージャ
// =====================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_DESTROY: // ウィンドウ破棄
        PostQuitMessage(0); // 終了処理
        break;

    case WM_CLOSE:
    {
        int res = MessageBoxA(NULL, "終了しますか？", "確認", MB_OKCANCEL);
        if (res == IDOK) // メッセージボックス
        {
            DestroyWindow(hWnd); // ウィンドウ削除
        }
        break;
    }

    case WM_KEYDOWN:
        if (LOWORD(wp) == VK_ESCAPE)
        {
            PostMessage(hWnd, WM_CLOSE, wp, lp);//ウィンドウプロシージャにWM_CLOSEを送る
        }
        break;

    default:
        return DefWindowProc(hWnd, msg, wp, lp);
        break;

    }

    return 0;
}