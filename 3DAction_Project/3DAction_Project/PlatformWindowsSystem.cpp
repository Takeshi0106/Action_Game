

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
// 描画マネージャー
#include "DirectX11_DrawManager.h"
// 入力ヘッダー
#include "DirectX_Input.h"  // DirectX用入力情報取得クラス
// カーソル制御クラス
#include "DirectX_CursorController.h" // DirectX用カーソル制御クラス
// モジュールをまとめるクラス
#include "GameModule.h"
// ログ出力用ヘッダー
#include "ReportMessage.h"  // デバッグ出力やメッセージボックス出力

#if defined(DEBUG) || defined(_DEBUG)
// Imgui用ヘッダー
#include "4_Imgui/imgui.h"
#include "4_Imgui/imgui_impl_win32.h"
#include "4_Imgui/imgui_impl_dx11.h"
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
    // ラップしているHINSTANCEを返す
    HINSTANCE Get() const { return hInstance; }
};


// =====================================================
// 静的メンバー変数
// =====================================================
APPLICATIONHANDLE PlatformWindowsSystem::m_AppInstance = nullptr;
HWND              PlatformWindowsSystem::m_WinInstance = nullptr;

std::unique_ptr<DirectX_DrawManager> PlatformWindowsSystem::m_DrawManager = nullptr;
std::unique_ptr<DirectX_Input> PlatformWindowsSystem::m_Input = nullptr;
std::unique_ptr<DirectX_CursorController> PlatformWindowsSystem::m_CursorController = nullptr;


// =====================================================
// プロトタイプ宣言
// =====================================================
// ウィンドウプロシージャ関数
static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

#if defined(DEBUG) || defined(_DEBUG)
// Imgui用　＊リリース時は何も実行しません
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif


// =====================================================
// コンストラクタ・デストラクタ
// ===================================================== 
#if defined(DEBUG) || defined(_DEBUG)
    // コンストラクタ・デストラクタ
PlatformWindowsSystem::PlatformWindowsSystem(
    uint16_t _width, uint16_t _height,
    const wchar_t* _windowClassName, const wchar_t* _windowName,
    const DrawPathConfig& _config,
    const DevelopmentPath& _devConfig,
    const String& _dxcPath) :
    m_Width(_width), m_Height(_height),
    m_WindowName(_windowName), m_WindowClassName(_windowClassName),
    m_PathConfig(_config),
    m_DevCompileModule(
        _config.shaderSourcePath,
        _config.shaderBinaryPath,
        _devConfig.kSPIRVFolderPath,
        _devConfig.kSPIRVReflectionInfoFolderPath,
        _dxcPath) {
}
#else
// コンストラクタ
PlatformWindowsSystem::PlatformWindowsSystem(uint16_t _width, uint16_t _height,
    const wchar_t* WindowClassName, const wchar_t* WindowName,
    const DrawPathConfig& _config)
    :m_Width(_width), m_Height(_height), m_WindowName(WindowName), m_WindowClassName(WindowClassName), m_PathConfig(_config)
{}
#endif

// デストラクタ
PlatformWindowsSystem::~PlatformWindowsSystem()
{

}


// =====================================================
// ウィンドウの初期化処理
// =====================================================
bool PlatformWindowsSystem::Init()
{
	// コンソールの文字コードをUTF-8に設定
    SetConsoleOutputCP(CP_UTF8);

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
    windClass.lpszClassName = (LPCWSTR)m_WindowClassName;                            // ウィンドウの名前設定
    windClass.hIconSm = LoadIcon(m_AppInstance.Get(), IDI_APPLICATION);     // タスクバーに表示されるアイコン (標準アイコンで作成 .icoで変更可能)

    // ウィンドウの登録 失敗したらfalseを返す
    if (!RegisterClassEx(&windClass)) {
        ErrorLog::OutputToConsole(u8"ウィンドウの登録に失敗しました");
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
        (LPCWSTR)m_WindowClassName,          // ウィンドウのクラス名
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
        ErrorLog::OutputToConsole(u8"ウィンドウが作成されませんでした");
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
    // メッセージ
    MSG msg = {};

    // ゲームの初期化処理
    if (GameInit())
    {
        while (true)
        {
            // プロシージャー受け取り
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE) // メッセージを受け取る
            {
                if (msg.message == WM_QUIT) { break; } // ウィンドウ削除を受け取ったらループを抜ける

                TranslateMessage(&msg); // キー入力などを文字列に変換する関数
                DispatchMessage(&msg); // ウィンドウプロシージャにメッセージを送る
            }
            else
            {
                // ゲームメイン
                if (!GameMain()) {
                    break;
                }
            }
        }
    }

    // ゲームの後処理
    GameUninit();
}


// =====================================================
// ウィンドウの後処理
// =====================================================
void PlatformWindowsSystem::Uninit()
{
    // ウィンドウの登録を解除
    if (m_AppInstance.Get() != nullptr)
    {
        UnregisterClass((LPCWSTR)m_WindowClassName, m_AppInstance.Get());
    }

    m_AppInstance = nullptr;
    m_WinInstance = nullptr;
}


// =====================================================
// ゲームの初期化処理
// =====================================================
bool PlatformWindowsSystem::GameInit()
{
#if defined(DEBUG) || defined(_DEBUG)
	// シェーダーコンパイル
    if (!m_DevCompileModule.ShaderCompile(DX11_CompileMode::Debug)) {
		ErrorLog::OutputToConsole(u8"シェーダーのコンパイルに失敗しました");
		return false;
    }
#endif

    // 描画マネージャー作成
    m_DrawManager = std::make_unique<DirectX_DrawManager>(m_PathConfig);
    m_DrawManager->Init(m_Width,m_Height,m_WinInstance);
	m_DrawManager->SetDrawSetting(FillModeSetting::Solid, CullingSetting::Back_Culling);

    // 入力情報作成
    m_Input = std::make_unique<DirectX_Input>();
    m_Input->Init();

	// カーソル制御クラス作成
	m_CursorController = std::make_unique<DirectX_CursorController>(&m_WinInstance);
	m_CursorController->SetCursorMode(CursorMode::CursorMode_Normal);

    // Imgui初期化
    InitImGui();

    // モジュールデータ作成
    GameModules modules = {
        static_cast<BaseDrawManager*>(m_DrawManager.get()),
        static_cast<Input*>(m_Input.get()),
        static_cast<CursorController*>(m_CursorController.get()) };

    // ゲームの初期化
    if (!m_Game->Init(modules)) {
        ErrorLog::OutputToConsole(u8"ゲームの初期化に失敗しました");
        return false;
    }

    return true;
}


// =====================================================
// ゲームの更新処理
// =====================================================
bool PlatformWindowsSystem::GameMain()
{
	// Imgui更新
	UpdateImGui();

    // ゲーム更新処理
    if (!m_Game->Update()) {
        DebugLog::OutputToConsole(u8"ゲームを終了します");
        return false;
    }


    // 描画前処理
    m_DrawManager->BegingDraw();

    // ゲームの描画処理
    m_Game->Draw();
    // Imgui描画
	DrawImGui();

    // 描画後処理
    m_DrawManager->EndDraw();

    // 入力情報更新
    m_Input->Update();
	// カーソル更新
	m_CursorController->Update();

    return true;
}


// =====================================================
// ゲームの後処理
// =====================================================
void PlatformWindowsSystem::GameUninit()
{
    // ゲームの後処理
    m_Game->Uninit();
    // Imguiの後処理
    UninitImGui();

    // 描画マネージャーの後処理
    m_DrawManager->Uninit();

    // 開放
	m_DrawManager.reset();
	m_Input.reset();
    m_CursorController.reset();
}



#if defined(DEBUG) || defined(_DEBUG)
// =====================================================
// Imgui用関数
// =====================================================
// 初期化
void PlatformWindowsSystem::InitImGui()
{
    // IMGUI初期化
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // 入力設定
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // スタイル設定
    ImGui::StyleColorsDark();

    // Win32 + DirectX11 初期化
    ImGui_ImplWin32_Init(m_WinInstance);
    ImGui_ImplDX11_Init(m_DrawManager->GetDevice(),
        m_DrawManager->GetDeviceContext());

    // フォントをロード
    ImFont* font = io.Fonts->AddFontFromFileTTF(
        "0_Asset/Font/LINESeedJP-Regular.ttf",
        18.0f,
        nullptr,
        io.Fonts->GetGlyphRangesJapanese());

    // 失敗時
    if (!font) {
        ErrorLog::OutputToConsole(u8"フォントを読み込めませんでした");
    }
}

// 更新
void PlatformWindowsSystem::UpdateImGui()
{
    // フレーム開始
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

// 描画
void PlatformWindowsSystem::DrawImGui()
{
    // 描画
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

// 後処理
void PlatformWindowsSystem::UninitImGui()
{
    // IMGUI後処理
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

#else
// リリース時は何も実行しない
void PlatformWindowsSystem::InitImGui() {}
void PlatformWindowsSystem::UpdateImGui() {}
void PlatformWindowsSystem::DrawImGui() {}
void PlatformWindowsSystem::UninitImGui() {}
#endif


// =====================================================
// ウィンドウプロシージャ
// =====================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{

#if defined(DEBUG) || defined(_DEBUG)
	// Imgui用のウィンドウプロシージャ呼び出し
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wp, lp)) {
        return true;
    }
#endif

    switch (msg)
    {
    case WM_DESTROY: // ウィンドウ破棄
        PostQuitMessage(0); // 終了処理
        break;

    case WM_INPUT:
        // 入力イベント受け取り時に呼び出す。
        DirectX_Input::HandleRawInput(lp);
        break;

    default:
        return DefWindowProc(hWnd, msg, wp, lp);
        break;

    }

    return 0;
}
