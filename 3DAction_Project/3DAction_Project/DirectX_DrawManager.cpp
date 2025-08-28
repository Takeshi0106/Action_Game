
// =========================================
// ヘッダー
// =========================================
// 必須ヘッダー
#include "DirectX_DrawManager.h" // 自分のヘッダー
// DirectXヘッダー
#include "DirectX.h" // DirectXの関数群ヘッダー
// マネージャーヘッダー
#include "ShaderManager.h" // シェーダーマネージャー
#include "ConstantBufferManager.h" // 定数バッファマネージャー
#include "VertexBufferManager.h"   // 頂点バッファマネージャー

// シェーダーヘッダー
#include "ShaderData.h"

// ログ出力
#include "ReportMessage.h"

// 計算構造体
#include "Matrix4x4.h"
#include "Quaternionh.h"
#include "Vector3.h"
#include "Color.h"

// デバッグ用
#include "Timer.h"


// ==========================================
// 構造体　デバッグ用
// ==========================================
struct Vertex {
	Vector3 pos;
	Color color;
};
float angle;


// ==========================================
// 静的メンバー変数
// ==========================================
#if defined(DEBUG) || defined(_DEBUG)

ShaderManager DirectX_DrawManager::m_ShaderManager = {
	"Debug/Log/Shader.txt",           // 使用したシェイダーの名前を書き出すログのパス
	"Asset/Debug/Shader",             // デバッグ時のコンパイルしたシェイダーを入れるパス
	"",                               // Debug時には使用しないパス　(.hlslがある場所を示すパス)
	"Asset/Info/ShaderReflection.txt" // リフレクションした情報を出力するファイルパス
};

#else

ShaderManager DirectX_DrawManager::m_ShaderManager = {
	"Debug/Log/Shader.txt",             // 使用したシェイダーの名前を書き出すログのパス
	"Asset/Shader/Compile",             // デバッグ時のコンパイルしたシェイダーを入れるパス
	"Asset/Shader/Hlsl",                // Debug時には使用しないパス　(.hlslがある場所を示すパス)
	"Asset/Info/ShaderReflection.txt"   // リフレクションした情報を出力するファイルパス
};

#endif

ConstantBufferManager DirectX_DrawManager::m_CBManager = {
	"Debug/Log/ConstantBuffers.txt"    // 使用したコンスタンスバッファの名前を書き出すパス
};

VertexBufferManager DirectX_DrawManager::m_VBManager = {
	"Debug/Log/VertexBuffers.tex"
};


// ==========================================
// 初期化
// ==========================================
bool DirectX_DrawManager::Init(unsigned int width, unsigned int height, HWND windowHandle)
{
	// DirectXの初期化
	if (!DirectX11::Init(width, height, windowHandle)) {
		ErrorLog::OutputToMessageBox("DirectXの初期化に失敗しました");
		return false; // 失敗したら戻る
	}

	// シェーダー・定数バッファ作成
	if (!m_ShaderManager.Init(DirectX11::Get::GetDevice(), m_CBManager)) {
		ErrorLog::OutputToMessageBox("ShaderManagerの初期化に失敗しました");
		return false; // 失敗したら戻る
	}

	Timer::Init(); // タイマー初期化
	Timer::Start(); // タイマー開始

	return true;
}


// ============================================
// 後処理
// ============================================
void DirectX_DrawManager::Uninit()
{
	DirectX11::Uninit();      // Directの後処理
	m_ShaderManager.Uninit(); // シェーダ―マネージャーの後処理
}


// ===========================================
// 描画
// ===========================================
void DirectX_DrawManager::Draw(const char* drawID, const void* data, const int size)
{

}


// ===========================================
// 定数バッファ更新
// ===========================================
void DirectX_DrawManager::UpdateShaderConstants(const char* constantName, const void* data, const int size)
{
	// 定数バッファ取得
	ID3D11Buffer* buffer = m_CBManager.GetFindConstantBuffer(constantName);

	if (buffer)
	{
		ID3D11DeviceContext* context = DirectX11::Get::GetContext();

		// MapしてCPU→GPUコピー
		D3D11_MAPPED_SUBRESOURCE mapped = {};
		if (SUCCEEDED(context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
		{
			memcpy(mapped.pData, data, size);
			context->Unmap(buffer, 0);
		}
	}
}


// ===========================================
// デバッグ用描画
// ===========================================
void DirectX_DrawManager::DebugDraw()
{
	// タイマーデバッグ
	Timer::Debug_CheckUpdate();

	// 三角形デバッグ描画	

	// デバッグ時 
	DirectX11::BeginDraw(); // 描画の開始処理

	// シェーダー取得
	VertexShaderData* vs = m_ShaderManager.GetFindVertexShader("VS_TriangleDebug");
	PixelShaderData* ps = m_ShaderManager.GetFindPixelShader("PS_TriangleDebug");

	// 入力レイアウト取得
	DirectX11::Get::GetContext()->IASetInputLayout(vs->GetILInfo()); // 入力レイアウト情報

	// 三角形の頂点
	Vertex vertices[3] =
	{
	{ { 0.0f, 0.57735f, 0.0f }, {1, 0, 0, 1} },   // 上頂点 (y = √3/2 * 0.5)
	{ { 0.5f, -0.288675f, 0.0f }, {0, 1, 0, 1} }, // 右下頂点
	{ { -0.5f, -0.288675f, 0.0f }, {0, 0, 1, 1} } // 左下頂点
	};

	// 頂点バッファ作成
	bool result = m_VBManager.CreateVertexBuffer(
		vs->GetName().c_str(),
		DirectX11::Get::GetDevice(),
		vertices,           // 頂点データ
		3,                  // 頂点数
		sizeof(Vertex),     // stride
		0                   // slot（省略可）
	);

	auto vertexBuffer = m_VBManager.GetFindVertexBuffer(vs->GetName());

	// 入力アセンブラ
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	DirectX11::Get::GetContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// トポロギー設定
	DirectX11::Get::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	// カリング削除
	D3D11_RASTERIZER_DESC rasterDesc{};
	rasterDesc.FillMode = D3D11_FILL_SOLID;       // 通常塗りつぶし
	rasterDesc.CullMode = D3D11_CULL_NONE;       // カリング無効
	rasterDesc.FrontCounterClockwise = FALSE;    // 頂点順序: 時計回りが表
	rasterDesc.DepthClipEnable = TRUE;

	ID3D11RasterizerState* noCullRS = nullptr;
	DirectX11::Get::GetDevice()->CreateRasterizerState(&rasterDesc, &noCullRS);

	DirectX11::Get::GetContext()->RSSetState(noCullRS);


	// 3. 定数バッファ更新とバインド
	std::vector<ConstantBufferInfo> cbInfo = vs->GetCBInfo();
	for (auto& cb : cbInfo)
	{
		// Y軸周りに回転させるとします 
		float speed = 3.14159265f * 4; 
		float deltaTime = Timer::GetDeltaTime(); 
		
		// 経過時間に応じて角度を増加 
		angle += speed * deltaTime;

		// ワールド行列（回転のみ）
		Quaternion rotQuat = Quaternion::CreateQuaternionFromAxisAngle(Vector3(1, 0, 0), angle);
		Matrix4x4 world = Matrix4x4::CreateRotationQuaternion_LH(rotQuat);

		// ビュー行列（カメラを少し離す）
		Vector3 eye(0, 0, -5);   // カメラ位置
		Vector3 at(0, 0, 0);     // 注視点
		Vector3 up(0, 1, 0);     // 上方向
		Matrix4x4 view = Matrix4x4::CreateViewMatrix_LH(eye, at, up);

		// プロジェクション行列（透視投影）
		float fov = 3.14159265f / 4.0f;     // 視野角45°
		float aspect = 1280.0f / 720.0f;
		float nearZ = 0.1f;
		float farZ = 100.0f;
		Matrix4x4 proj = Matrix4x4::CreateProjectionMatrix_LH(fov, aspect, nearZ, farZ);

		struct TransformCB
		{
			Matrix4x4 WorldMatrix;
			Matrix4x4 ViewMatrix;
			Matrix4x4 ProjMatrix;
		};

		TransformCB mat = { world.toGPU(), view.toGPU(),proj.toGPU() };

		// 定数バッファ更新
		UpdateShaderConstants(cb.GetName().c_str(), &mat, sizeof(mat));

		// 定数バッファ取得
		ID3D11Buffer * buffer = m_CBManager.GetFindConstantBuffer(cb.GetName());

		if (buffer)
		{
			ID3D11DeviceContext* context = DirectX11::Get::GetContext();

			// VSスロット番号にバインド
			context->VSSetConstantBuffers(cb.GetRegisterNumber(), 1, &buffer);
		}
	}

	// 4. シェーダーセット
	DirectX11::Get::GetContext()->VSSetShader(vs->GetShader(), nullptr, 0);
	DirectX11::Get::GetContext()->PSSetShader(ps->GetShader(), nullptr, 0);

	// 6. 描画
	DirectX11::Get::GetContext()->Draw(3, 0);


	// DirectX11::DebugDraw(Timer::GetElapsedTime()); // デバッグ表示


	DirectX11::EndDraw(); // 描画の終わり処理

	Timer::LastUpdate(); // タイマー更新処理
}