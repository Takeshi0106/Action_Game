

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
// 描画マネージャー
#include "BaseDrawManager.h"
#include "ModelManager.h"
// モデル情報構造体
#include "ModelData.h"


// ==============================
// クラス
// ==============================
class ModelConversionModule
{
private:
	// モデルパス
	const char* m_ModelPath;
	const char* m_SelfModelPath;

	// .Objだけをロードする
	const char* kObjExtension = ".obj";

	// モデルをロードする (モデルファイルはm_ModelPathの下にフォルダーを入れていたらフォルダー名を入れる)
	bool ModelLoad(const char* modelPath, int flag, ModelData& modelData, const char* modelFile = "");

public:
	// コンストラクタ・デストラクタ
	ModelConversionModule(
		const char* modelPath,
		const char* SelfModelPath)
		: m_ModelPath(modelPath),m_SelfModelPath(SelfModelPath)
	{}
	~ModelConversionModule() = default;

	// 初期化時に変換されているかの確認、変換を行う
	bool ModelConversion();

	// 今はモデルをロードしてモデルマネージャーに入れる関数
	// modelFile はモデルパスの下にファイルがあった場合、ファイル名を入れる
	bool LoadAndRegisterModelResources(
		const char* modelName,
		BaseDrawManager& drawManager,
		ModelManager& modelManager,
		const char* modelFile = "");
};
