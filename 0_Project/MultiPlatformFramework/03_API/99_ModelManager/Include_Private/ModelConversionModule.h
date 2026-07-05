

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
#include "IDrawManager.h"
#include "ModelManager.h"
// モデル情報構造体
#include "ModelData.h"
// 文字列ヘッダー
#include "UTF8_String.h"


// ==============================
// クラス
// ==============================
class ModelConversionModule
{
private:
	// モデルパス
	const String& m_ModelPath;
	const String& m_SelfModelPath;

	// .Objだけをロードする
	const String kObjExtension = u8".obj";

	// モデルをロードする (モデルファイルはm_ModelPathの下にフォルダーを入れていたらフォルダー名を入れる)
	bool ModelLoad(const String& modelPath, int flag, ModelData& modelData, const String& modelFile = u8"");

public:
	// コンストラクタ・デストラクタ
	ModelConversionModule(
		const String& modelPath,
		const String& SelfModelPath)
		: m_ModelPath(modelPath),m_SelfModelPath(SelfModelPath)
	{}
	~ModelConversionModule() = default;

	// 初期化時に変換されているかの確認、変換を行う
	bool ModelConversion();

	// 今はモデルをロードしてモデルマネージャーに入れる関数
	// modelFile はモデルパスの下にファイルがあった場合、ファイル名を入れる
	bool LoadAndRegisterModelResources(
		const Hashed_String& modelName,
		IDrawManager& drawManager,
		ModelManager& modelManager,
		const String& modelFile = u8"");
};
