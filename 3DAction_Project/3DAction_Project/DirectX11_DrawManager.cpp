
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

// シェーダーヘッダー
#include "ShaderData.h"
// 頂点バッファヘッダー
#include "VertexBufferData.h"

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

	// シェーダー作成
	if (!m_ShaderManager.Init(DirectX11::Get::GetDevice())) {
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
	m_ShaderManager.Uninit();
	// 定数バッファ解放
	m_CBManager.ReleaseAllConstantBuffers();
	// 頂点バッファ解放
	m_VBManager.ReleaseAllVertexBuffers();
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
	if (!m_VBManager.CreateVertexBuffer(
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
	if (!m_CBManager.CreateConstantBuffer(
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
	else
	{
		ErrorLog::OutputToConsole((std::string("定数バッファが更新できませんでした　:") + constantName).c_str());
	}
}


// =============================================
// 頂点バッファ更新
// =============================================
void DirectX_DrawManager::UpdateVertexBuffer(const char* drawID, const void* data, int size)
{
	// 描画IDから頂点シェーダー検索 // 

	// 頂点シェーダー検索
	VertexShaderData* vs = m_ShaderManager.GetFindVertexShader(drawID);

	// 頂点バッファデータ取得
	VertexBufferData* vbData = m_VBManager.GetFindVertexData(drawID);

	// 頂点バッファ検索
	ID3D11Buffer* buffer = vbData->GetVertexBuffer();

	// 頂点バッファ更新
	ID3D11DeviceContext* context = DirectX11::Get::GetContext();

	D3D11_MAPPED_SUBRESOURCE mapped{};
	HRESULT hr = context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	if (SUCCEEDED(hr))
	{
		memcpy(mapped.pData, data, size);
		DirectX11::Get::GetContext()->Unmap(buffer, 0);

		vbData->SetIsUpdate(true);
	}
	else
	{
		ErrorLog::OutputToConsole((std::string("頂点バッファが更新できませんでした　:") + drawID).c_str());
	}
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
	VertexShaderData* vs = m_ShaderManager.GetFindVertexShader(vsName);
	PixelShaderData* ps = m_ShaderManager.GetFindPixelShader(psName);

	// 入力レイアウト設定
	DirectX11::Get::GetContext()->IASetInputLayout(vs->GetILInfo()); // 入力レイアウト情報

	// 頂点バッファ取得
	VertexBufferData* vertexBufferData= m_VBManager.GetFindVertexData(vsName);

	// 更新されている場合だけセット
	if (vertexBufferData->GetIsUpdate()) 
	{
		// 入力アセンブラ
		ID3D11Buffer* buffers = vertexBufferData->GetVertexBuffer();
		UINT stride = UINT(vertexBufferData->GetStride());
		UINT offset = 0;

		DirectX11::Get::GetContext()->IASetVertexBuffers(0, 1, &buffers, &stride, &offset);
		vertexBufferData->SetIsUpdate(false);
	}

	// トポロギー設定
	DirectX11::Get::GetContext()->IASetPrimitiveTopology(vertexBufferData->GetPrimitiveType());

	// 3. 定数バッファ更新とバインド
	// 頂点
	std::vector<ConstantBufferInfo> cbInfo = vs->GetCBInfo();
	for (auto& cb : cbInfo)
	{
		// 定数バッファ取得
		ID3D11Buffer* buffer = m_CBManager.GetFindConstantBuffer(cb.GetName());
		if (buffer) {
			// VSスロット番号にバインド
			DirectX11::Get::GetContext()->VSSetConstantBuffers(cb.GetRegisterNumber(), 1, &buffer);
		}
	}
	// ピクセル
	cbInfo = ps->GetCBInfo();
	for (auto& cb : cbInfo)
	{
		// 定数バッファ取得
		ID3D11Buffer* buffer = m_CBManager.GetFindConstantBuffer(cb.GetName());
		if (buffer) {
			// VSスロット番号にバインド
			DirectX11::Get::GetContext()->PSSetConstantBuffers(cb.GetRegisterNumber(), 1, &buffer);
		}
	}

	// 4. シェーダーセット
	DirectX11::Get::GetContext()->VSSetShader(vs->GetShader(), nullptr, 0);
	DirectX11::Get::GetContext()->PSSetShader(ps->GetShader(), nullptr, 0);

	// 6. 描画
	DirectX11::Get::GetContext()->Draw(vertexBufferData->GetVertexCount(), 0);



	DirectX11::EndDraw(); // 描画の終わり処理
}