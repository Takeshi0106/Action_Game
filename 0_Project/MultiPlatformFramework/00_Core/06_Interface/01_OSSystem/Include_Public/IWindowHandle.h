#pragma once

// ===========================================
// 【クラス概要】
// ウィンドウハンドルの抽象化クラス（基底クラス）
// このクラスを派生させてプラットフォーム別の
// ウィンドウハンドルを作成する
// ===========================================

class IWindowHandle
{
public:
	// コンストラクタ・デストラクタ
	IWindowHandle() = default;
	virtual ~IWindowHandle() = default;

	// ウィンドウハンドルの取得
	virtual void* GetWindowHandle() const = 0;
};
