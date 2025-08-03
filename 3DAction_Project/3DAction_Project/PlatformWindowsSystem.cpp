
// ===============================================
// 【クラス概要】
// Windowsプラットフォームの初期化
// ウィンドウの作成、後処理などを行う
// ===============================================


// ===============================================
// ヘッダー
// ===============================================
// 必須ヘッダー
#include "PlatformWindowsSystem.h"  // 自分のヘッダー
// ウィンドウ作成用のヘッダー
#include <Windows.h>  // ウィンドウ作成用
// 標準ヘッダー
#include <cstdint>   // 整数型 uintなど
// ログ出力用ヘッダー
#include "ReportMessage.h"  // デバッグ出力やメッセージボックス出力


// デバッグ用でおいているヘッダー ----------------------------------------------
// DirectXヘッダー
#include "DirectX.h"  // DirectX初期化用　後で描画マネージャーに任せるようにする
// GameMain用ヘッダー
#include "Timer.h"   // デバッグ用
// DrawManagerで使用するヘッダー
#include "ShaderManager.h"  // シェイダーマネージャー
#include "ConstantBufferManager.h" // 定数バッファマネージャー
// -----------------------------------------------------------------------------


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
    // ラップしているHINSTANCEを返す
    HINSTANCE Get() const { return hInstance; }
};


// =====================================================
// 静的メンバー変数
// =====================================================
APPLICATIONHANDLE PlatformWindowsSystem::m_AppInstance = nullptr;
HWND              PlatformWindowsSystem::m_WinInstance = nullptr;

// デバッグ用　DrawManagerに移動する
ShaderManager PlatformWindowsSystem::m_ShaderManager = {
    "Debug/Log/Shader.txt",          // 使用したシェイダーの名前を書き出すログのパス
    "Asset/Debug/Shader",             // デバッグ時のコンパイルしたシェイダーを入れるパス
    "",                               // Debug時には使用しないパス　(.hlslがある場所を示すパス)
    "Asset/Info/ShaderReflection.txt" // リフレクションした情報を出力するファイルパス
};

ConstantBufferManager PlatformWindowsSystem::m_ConstantBufferManager = {
    "Debug/Log/ConstantBuffers.txt"    // 使用したコンスタンスバッファの名前を書き出すパス
};


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
// デストラクタ
// =====================================================
PlatformWindowsSystem::~PlatformWindowsSystem()
{
#if defined(DEBUG) || defined(_DEBUG)
    if (IsUninit)
    {
        ErrorLog::OutputToConsole("PlatformWindowsSystem : 後処理が実行されていません");
        Uninit();
    }
#endif
}


// =====================================================
// ウィンドウの初期化処理
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
    windClass.lpszClassName = m_WindowClassName;                            // ウィンドウの名前設定
    windClass.hIconSm = LoadIcon(m_AppInstance.Get(), IDI_APPLICATION);     // タスクバーに表示されるアイコン (標準アイコンで作成 .icoで変更可能)

    // ウィンドウの登録 失敗したらfalseを返す
    if (!RegisterClassEx(&windClass)) {
        ErrorLog::OutputToConsole("ウィンドウの登録に失敗しました");
        return false; 
    } 

    // 描画する大きさを設定
    RECT rect = {};
    rect.right = static_cast<LONG>(m_Width);   // 横
    rect.bottom = static_cast<LONG>(m_Height); // 縦

    // ウィンドウの大きさを計算　（描画する大きさ＋枠）
    // 枠を設定 (標準なウィンドウ、タイトルバーあり、システムメニューあり)
    DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME;
    AdjustWindowRect(&rect, style, FALSE); // rectに計算した大きさを代入

    // ウィンドウ作成
    m_WinInstance = CreateWindowEx(
        0,                          // ウィンドウの動作や見た目（デフォルト）
        m_WindowClassName,          // ウィンドウのクラス名
        m_WindowName,               // ウィンドウのタイトルバーで表示される名前
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
    if (m_WinInstance == nullptr) {
        ErrorLog::OutputToConsole("ウィンドウが作成されませんでした");
        return false; 
    }

    // ウィンドウを表示
    ShowWindow(m_WinInstance, SW_SHOWNORMAL);

    // ウィンドウを更新
    UpdateWindow(m_WinInstance);

    // ウィンドウに入力情報を取得させる
    SetFocus(m_WinInstance);

    // 正常終了.
    return true;
}


// =====================================================
// ゲームループ　（ゲーム本編）
// =====================================================
void PlatformWindowsSystem::GameLoop()
{
    MSG msg = {}; // メッセージ

    if (GameInit()) // ゲームの初期化処理
    {
        while (true)
        {
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE) // メッセージを受け取る
            {
                if (msg.message == WM_QUIT) { break; } // ウィンドウ削除を受け取ったらループを抜ける

                TranslateMessage(&msg); // キー入力などを文字列に変換する関数
                DispatchMessage(&msg); // ウィンドウプロシージャにメッセージを送る
            }
            else
            {
                GameMain();
            }
        }
    }

    GameUninit();        // ゲームの後処理 多重に呼び出しても問題ないように作成
}


// =====================================================
// ウィンドウの後処理
// =====================================================
void PlatformWindowsSystem::Uninit()
{
    // ウィンドウの登録を解除
    if (m_AppInstance.Get() != nullptr)
    {
        UnregisterClass(m_WindowClassName, m_AppInstance.Get());
    }

    m_AppInstance = nullptr;
    m_WinInstance = nullptr;
}


// =====================================================
// ゲームの初期化処理
// =====================================================
bool PlatformWindowsSystem::GameInit()
{
    if (!DirectX11::Init(m_Width, m_Height, m_WinInstance)) { // DirectXの初期化
        ErrorLog::OutputToMessageBox("DirectXの初期化に失敗しました");
        return false; // 失敗したら戻る
    }
    if (!m_ShaderManager.Init(DirectX11::Get::GetDevice(), m_ConstantBufferManager))
    {
        ErrorLog::OutputToMessageBox("ShaderManagerの初期化に失敗しました");
        return false; // 失敗したら戻る
    }

    Timer::Init(); // タイマー初期化
    Timer::Start(); // タイマー開始

    return true;
}


// =====================================================
// ゲームの更新処理
// =====================================================
void PlatformWindowsSystem::GameMain()
{
    float a[4] = { 1.0f,0.1f,1.0f,0.1f };

    Timer::Debug_CheckUpdate(); // タイマーデバッグ

    // デバッグ時 
    DirectX11::BeginDraw(); // 描画の開始処理

    // m_ConstantBufferManager.BindVS("Transform", DirectX11::Get::GetContext());
    // m_ConstantBufferManager.BindVS("DebugParams", DirectX11::Get::GetContext());
    // m_ConstantBufferManager.UpdateConstantBuffer("PSInput", a, sizeof(a), DirectX11::Get::GetContext());
    // m_ShaderManager.BindVertexShaderSet("VS_Debug", DirectX11::Get::GetContext());
    // m_ShaderManager.BindPixelShaderSet("PS_Debug", DirectX11::Get::GetContext());

    DirectX11::DebugDraw(Timer::GetElapsedTime()); // デバッグ表示

    DirectX11::EndDraw(); // 描画の終わり処理

    Timer::LastUpdate(); // タイマー更新処理
}


// =====================================================
// ゲームの後処理
// =====================================================
void PlatformWindowsSystem::GameUninit()
{
    DirectX11::Uninit();      // Directの後処理
    m_ShaderManager.Uninit(); // シェーダ―マネージャーの後処理
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