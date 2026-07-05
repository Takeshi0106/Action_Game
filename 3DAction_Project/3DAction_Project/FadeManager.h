#pragma once

// ========================================
// 【クラス概要】
// フェードマネージャー
// ========================================


// ========================================
// ヘッダー
// ========================================
// フェード基底クラス
#include "BaseFadeState.h"
// フェードID設定
#include "FadeIDSetting.h"
// フェード描画用
#include "BaseDrawManager.h"
// 動的確保ヘッダー
#include <memory>


// ========================================
// クラス
// ========================================
class FadeManager final
{
private:
	// フェード時間
	float m_FadeTime = 0.0f;
	// フェード経過時間
	float m_FadeElapsedTime = 0.0f;

	// フェードState
	std::unique_ptr<BaseFadeState> m_FadeState = nullptr;

	// フェードで使用する板

	// フェード描画用
	BaseDrawManager* m_DrawManager = nullptr;

public:
	// コンストラクタ・デストラクタ
	FadeManager() = default;
	~FadeManager() = default;

	// 初期化
	bool Init(BaseDrawManager* _draw);
	// フェード更新
	bool FadeUpdate(float deltaTime);
	// 終了処理
	void Uninit();

	// フェード変更
	void ChangeFadeState(FadeID _fade);

	// フェードタイムをセット
	void SetFadeTime(float _time) { m_FadeTime = _time; }
	// 
};

