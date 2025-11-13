

// ==============================
// 【クラス概要】
// コンストラクタで 自作に変換するモデルのパスを入れる
// Assimpで読み込んでマネージャーに格納する
// のちに、自作モデルに変換したい
// 
// 【注意】
// 今はボーンを使用しない +
// 使用したいモデルが.objなので
// .objしか読み込めないようにしています。
// ==============================

// ==============================
// ヘッダー
// ==============================
// 文字列
#include <string>
// 描画マネージャー
#include "DirectX11_DrawManager.h"


// ==============================
// クラス
// ==============================
class ModelConversionModule
{
private:
	// モデルパス
	const std::string m_ModelPath;
	const std::string m_SelfModelPath;

	// .Objだけをロードする
	const std::string kObjExtension = ".obj";

	// モデルをロードする
	bool ModelLoad(const std::string& modelPath);

public:
	// コンストラクタ・デストラクタ
	ModelConversionModule(const char* modelPath,
		const char* SelfModelPath)
		: m_ModelPath(modelPath),m_SelfModelPath(SelfModelPath)
	{}
	~ModelConversionModule() = default;

	// 初期化時に変換されているかの確認、変換を行う
	bool Init();

	// 今はモデルをロードしてモデルマネージャーに入れる関数
	bool LoadAndRegisterModelResources(const std::string& modelName,
		DirectX_DrawManager& drawManager);
};
