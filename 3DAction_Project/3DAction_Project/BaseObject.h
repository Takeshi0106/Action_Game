#pragma once

// ========================
// ヘッダー
// ========================
// 描画マネージャー
#include "BaseDrawManager.h"
// SRT情報
#include "SRT.h"
// 文字列
#include <string>


// =========================
// クラス
// =========================
class BaseObject
{
protected:
	// --------------------------------
	// メンバー変数
	// --------------------------------
	// 描画マネージャー
	BaseDrawManager* m_Draw = nullptr;

	// このオブジェクトが更新する定数バッファ名
	const std::string m_TransformCBName = "Transform";

	// SRT情報
	SRT m_SRT = {};

	// --------------------------------
	// 仮想関数
	// --------------------------------
	// 遅延初期化
	virtual void DerivationInit() = 0;

public:
	// --------------------------------
	// コンストラクタ・デストラクタ
	// --------------------------------
	BaseObject() = default;
	virtual ~BaseObject() = default;


	// --------------------------------
	// 仮想関数
	// --------------------------------
	// 描画
	virtual void Draw() = 0;
	// 更新
	virtual void Update() = 0;
	// 後処理
	virtual void Uninit() = 0;


	// ---------------------------------
	// 非仮想関数
	// ---------------------------------
	// 初期化
	void Init(BaseDrawManager* _drawManager)
	{
		// マネージャー保存
		m_Draw = _drawManager;

		// 仮想関数の派生初期化呼び出し
		DerivationInit();
	}

	// ゲッター
	const SRT& GetSRT() const { return m_SRT; }

	// セッター
	void SetSRT(const SRT& srt) { m_SRT = srt; }
	void SetPosition(const Vector3& pos) { m_SRT.position = pos; }
	void SetRotation(const Quaternion& rot) { m_SRT.rotation = rot; }
	void SetScale(const Vector3& scale) { m_SRT.scale = scale; }
};

