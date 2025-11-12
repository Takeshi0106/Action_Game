

// ==============================
// 【クラス概要】
// コンストラクタで 自作に変換するモデルのパスを入れる
// Assimpで読み込んでマネージャーに格納する
// ==============================

// ==============================
// ヘッダー
// ==============================
#include <string>


// ==============================
// クラス
// ==============================
class ModelConversionModule
{
private:
	// モデルパス
	const std::string m_ModelPath;
	const std::string m_SelfModelPath;

public:
	// コンストラクタ・デストラクタ
	ModelConversionModule(const char* modelPath,
		const char* SelfModelPath)
		: m_ModelPath(modelPath),m_SelfModelPath(SelfModelPath)
	{}
	~ModelConversionModule() = default;

	// 初期化時に変換されているかの確認、変換を行う
	bool Init();
};
