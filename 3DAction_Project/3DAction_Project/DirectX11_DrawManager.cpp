
// =========================================
// ヘッダー
// =========================================
// 必須ヘッダー
#include "DirectX11_DrawManager.h" // 自分のヘッダー

// DirectXヘッダー
#include "DirectX.h" // DirectXの関数群ヘッダー

// マネージャーヘッダー
#include "ShaderManager.h" // シェーダーマネージャー
#include "ConstantBufferManager.h" // 定数バッファマネージャー
#include "VertexBufferManager.h"   // 頂点バッファマネージャー
#include "TextureManager.h"

// シェーダーヘッダー
#include "ShaderData.h"
// 頂点バッファヘッダー
#include "VertexBufferData.h"
// 定数バッファヘッダー
#include "ConstantBufferData.h"

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
struct TransformCB
{
	Matrix4x4 WorldMatrix;
	Matrix4x4 ViewMatrix;
	Matrix4x4 ProjMatrix;
};
// 三角形の頂点
Vertex vertices[3] =
{
{ { 0.0f, 0.57735f, 0.0f }, {1, 0, 0, 1} },   // 上頂点 (y = √3/2 * 0.5)
{ { 0.5f, -0.288675f, 0.0f }, {0, 1, 0, 1} }, // 右下頂点
{ { -0.5f, -0.288675f, 0.0f }, {0, 0, 1, 1} } // 左下頂点
};
float angle;


// コンストラクタ・デストラクタ
DirectX_DrawManager::DirectX_DrawManager()
{
	// 実態作成
#if defined(DEBUG) || defined(_DEBUG)
	m_ShaderManager = std::make_unique<ShaderManager>(
		"Asset/Debug/Shader",
		"",
		"Asset/Info/ShaderReflection.txt"
	);
#else
	m_ShaderManager = std::make_unique<ShaderManager>(
		"Asset/Shader/Compile",
		"Asset/Shader/Hlsl",
		"Asset/Info/ShaderReflection.txt"
	);
#endif

	m_CBManager = std::make_unique<ConstantBufferManager>();
	m_VBManager = std::make_unique<VertexBufferManager>();
	m_TextureManager = std::make_unique<TextureManager>();
}


DirectX_DrawManager::~DirectX_DrawManager() = default;

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

	// シェーダー作成
	if (!m_ShaderManager->Init(DirectX11::Get::GetDevice())) {
		ErrorLog::OutputToMessageBox("ShaderManagerの初期化に失敗しました");
		return false; // 失敗したら戻る
	}


	// 定数バッファ初期化
	// ワールド行列
	Matrix4x4 world = Matrix4x4::CreateIdentityMatrix();

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

	TransformCB mat = { world.toGPU(), view.toGPU(),proj.toGPU() };


	// 頂点バッファ作成
	CreateVertexBuffer(
		"VS_TriangleDebug",
		vertices,
		sizeof(Vertex),
		PrimitiveType::TriangleStrip,
		BufferUsage::Dynamic,
		CPUAccess::Write);

	// 定数バッファ作成
	CreateConstantBuffer(
		"Transform1",
		&mat,
		sizeof(mat),
		BufferUsage::Dynamic,
		CPUAccess::Write);

	// テクスチャ作成
	if (!m_TextureManager->CreateTexture(
		"DebugTexture",
		DirectX11::Get::GetDevice(),
		1920,
		1080,
		Format::R8G8B8A8_UNorm,
		BindFlag::ShaderResource | BindFlag::RenderTarget,
		BufferUsage::Default,
		CPUAccess::None))
	{
		ErrorLog::OutputToConsole("テクスチャ作成に失敗");
		return false;
	}


	// カリング削除
	D3D11_RASTERIZER_DESC rasterDesc{};
	rasterDesc.FillMode = D3D11_FILL_SOLID;       // 通常塗りつぶし
	rasterDesc.CullMode = D3D11_CULL_NONE;       // カリング無効
	rasterDesc.FrontCounterClockwise = FALSE;    // 頂点順序: 時計回りが表
	rasterDesc.DepthClipEnable = TRUE;

	ID3D11RasterizerState* noCullRS = nullptr;
	DirectX11::Get::GetDevice()->CreateRasterizerState(&rasterDesc, &noCullRS);

	DirectX11::Get::GetContext()->RSSetState(noCullRS);


	Timer::Init(); // タイマー初期化
	Timer::Start(); // タイマー開始

	return true;
}


// ============================================
// 後処理
// ============================================
void DirectX_DrawManager::Uninit()
{
	// Directの後処理
	DirectX11::Uninit();
	// シェーダ―マネージャーの後処理
	m_ShaderManager->Uninit();
	// 定数バッファ解放
	m_CBManager->ReleaseAllConstantBuffers();
	// 頂点バッファ解放
	m_VBManager->ReleaseAllVertexBuffers();
}


// ===========================================
// 描画
// ===========================================
void DirectX_DrawManager::Draw(const char* drawID, const void* data, const int size)
{

}



// ===========================================
// 頂点バッファ作成
// ===========================================
void DirectX_DrawManager::CreateVertexBuffer(
	const char* drawID,
	const void* data,
	size_t stride,
	PrimitiveType type,
	BufferUsage usage,
	CPUAccess access)
{
	// 描画IDを使用して頂点数を取得


	// 頂点バッファ作成
	if (!m_VBManager->CreateVertexBuffer(
		drawID,
		DirectX11::Get::GetDevice(),
		data, // 頂点データ
		3,   // 頂点数
		10,   // 最大頂点数
		stride,
		type,
		usage,
		access))
	{
		ErrorLog::OutputToConsole("頂点バッファ作製失敗");
	}
}


// ===========================================
// 定数バッファ作成
// ===========================================
void DirectX_DrawManager::CreateConstantBuffer(
	const char* constantName,
	const void* data,
	size_t size,
	BufferUsage usage,
	CPUAccess access)
{
	// 定数バッファ作成
	if (!m_CBManager->CreateConstantBuffer(
		constantName,
		DirectX11::Get::GetDevice(),
		data, // データ
		size,
		usage,
		access))
	{
		ErrorLog::OutputToConsole("定数バッファ作製失敗");
	}
}


// ===========================================
// 定数バッファ更新
// ===========================================
void DirectX_DrawManager::UpdateShaderConstants(const char* constantName, const void* data, const int size)
{
	// 定数バッファ取得
	ConstantBufferData* buffer = m_CBManager->GetFindConstantBuffer(constantName);

	// 定数バッファ更新
	buffer->UpdateConstantBuffer(DirectX11::Get::GetContext(), data, size);
}


// =============================================
// 頂点バッファ更新
// =============================================
void DirectX_DrawManager::UpdateVertexBuffer(const char* drawID, const void* data, int size)
{
	// 描画IDから頂点シェーダー検索 // 


	// 頂点バッファデータ取得
	VertexBufferData* vbData = m_VBManager->GetFindVertexData(drawID);

	// 頂点バッファ更新
	vbData->UpdateBuffer(DirectX11::Get::GetContext(), data, size);
}


// ==============================================
// デバッグ用更新
// ==============================================
void DirectX_DrawManager::DebugUpdate()
{
	// タイマーデバッグ
	Timer::Debug_CheckUpdate();

	// デルタタイム取得
	float deltaTime = Timer::GetDeltaTime();

	// 頂点バッファ更新 ------------------------------
	static float offset = 0.0f;
	offset += 0.01f * deltaTime; // 時間経過で移動

	for (int i = 0; i < sizeof(vertices) / sizeof(Vertex); i++)
	{
		vertices[i].pos.x += offset;
	}

	// 頂点バッファ更新
	UpdateVertexBuffer("VS_TriangleDebug", vertices, sizeof(vertices));

	
	// 定数バッファ更新 ------------------------------
	// Y軸周りに回転させるとします 
	float speed = 3.14159265f * 4;

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

	TransformCB mat = { world.toGPU(), view.toGPU(),proj.toGPU() };

	// 定数バッファ更新
	// UpdateShaderConstants("Transform1", &mat, sizeof(mat));


	// タイマー更新処理
	Timer::LastUpdate();
}


// ===========================================
// デバッグ用描画
// ===========================================
void DirectX_DrawManager::DebugDraw()
{
	// 三角形デバッグ描画	

	// デバッグ時 
	DirectX11::BeginDraw(); // 描画の開始処理

	// DirectX11::DebugDraw(Timer::GetElapsedTime()); // デバッグ表示

	std::string vsName = "VS_TriangleDebug";
	std::string psName = "PS_TriangleDebug";

	// シェーダー取得
	VertexShaderData* vs = m_ShaderManager->GetFindVertexShader(vsName);
	PixelShaderData* ps = m_ShaderManager->GetFindPixelShader(psName);

	// 入力レイアウト設定
	DirectX11::Get::GetContext()->IASetInputLayout(vs->GetInputLayout()); // 入力レイアウト情報

	// 頂点バッファ取得
	VertexBufferData* vertexBufferData = m_VBManager->GetFindVertexData(vsName);

	// 更新されている場合だけセット
	// 入力アセンブラ
	ID3D11Buffer* vbuffers = vertexBufferData->GetVertexBuffer();
	UINT stride = UINT(vertexBufferData->GetStride());
	UINT offset = 0;

	DirectX11::Get::GetContext()->IASetVertexBuffers(0, 1, &vbuffers, &stride, &offset);
	vertexBufferData->SetIsUpdate(false);

	// トポロギー設定
	DirectX11::Get::GetContext()->IASetPrimitiveTopology(vertexBufferData->GetPrimitiveType());

	// 3. 定数バッファ更新とバインド
	// 頂点
	std::vector<ConstantBufferInfo> cbInfo = vs->GetCBInfo();
	std::vector<ID3D11Buffer*> buffers(cbInfo.size(), nullptr);

	for (size_t i = 0; i < cbInfo.size(); i++)
	{
		// 定数バッファ取得
		ConstantBufferData* buffer = m_CBManager->GetFindConstantBuffer(cbInfo[i].GetName());
		if (buffer) {
			// VSスロット番号にバインド
			buffers[i] = buffer->GetBuffer(); // バッファポインタをセット
		}
	}

	// まとめてバインド
	DirectX11::Get::GetContext()->VSSetConstantBuffers(
		0,                         // 先頭スロット
		static_cast<UINT>(buffers.size()),
		buffers.data()             // 配列を渡す
	);

	// ピクセル
	cbInfo = ps->GetCBInfo();
	buffers.resize(cbInfo.size(), nullptr);

	for (size_t i = 0; i < cbInfo.size(); i++)
	{
		// 定数バッファ取得
		ConstantBufferData* buffer = m_CBManager->GetFindConstantBuffer(cbInfo[i].GetName());
		if (buffer) {
			// VSスロット番号にバインド
			buffers[i] = buffer->GetBuffer(); // バッファポインタをセット
		}
	}

	// まとめてバインド
	DirectX11::Get::GetContext()->PSSetConstantBuffers(
		0,                         // 先頭スロット
		static_cast<UINT>(buffers.size()),
		buffers.data()             // 配列を渡す
	);


	// 4. シェーダーセット
	DirectX11::Get::GetContext()->VSSetShader(vs->GetVertexShader(), nullptr, 0);
	DirectX11::Get::GetContext()->PSSetShader(ps->GetPixelShader(), nullptr, 0);


	// 6. 描画
	DirectX11::Get::GetContext()->Draw(vertexBufferData->GetVertexCount(), 0);


	DirectX11::EndDraw(); // 描画の終わり処理
}