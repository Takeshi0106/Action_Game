#pragma once

// ========================================
// 【クラス概要】
// フェードの基底クラス
// ========================================


// ========================================
// クラス
// ========================================
class BaseFadeState
{
private:
	// フェードに使用する板をここで持つかも？

public:
	// コンストラクタ・デストラクタ
	BaseFadeState() = default;
	virtual ~BaseFadeState() = default;

	// フェードの初期化
	virtual bool Init() = 0;
	// フェードの更新 (フェード進行度を 0~1 で渡してください)
	virtual void Update(float _progress) = 0;
	// フェードの終了処理
	virtual void Uninit() = 0;
};
