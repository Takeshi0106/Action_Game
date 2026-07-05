#pragma once

// =======================================================
// 【クラス概要】
// PlatformSystem クラスは、プラットフォームに依存する
// システム起動・実行のための基底クラスです。
// 
// ・派生クラスにより、各プラットフォーム固有の処理を実装する
// ・Execute() により、共通の起動フロー（Init → GameLoop → Uninit）を提供
// ・デバッグビルド時のみ、インスタンスが1つだけ生成されることをアサートで保証
// =======================================================


// =====================================================
// ヘッダー
// =====================================================
// 基底ヘッダー
#include "NonCopyable.h" // コピーを禁止するヘッダー
#include "IGame.h"
#include "IDrawManager.h"


// =====================================================
// プラットフォームの基底クラス
// =====================================================
class IPlatformSystem : public NonCopyable
{
protected:
	// ゲーム本体のポインター
	IGame* m_Game = nullptr;
	// 描画マネージャーのポインター
	IDrawManager* m_DrawManager = nullptr;

	// プラットフォームごとにオーバーライドする関数
	// 初期化・後処理
	virtual bool Init() = 0;	   
	virtual void Uninit() = 0;	
	// ゲーム関連
	virtual void GameLoop() = 0;
	virtual bool GameInit() = 0;   
	virtual bool GameMain() = 0;   
	virtual void GameUninit() = 0; 

public:
	IPlatformSystem() = default; // コンストラクタ
	virtual ~IPlatformSystem() = default; // デストラクタ

	void Execute(IGame* game, IDrawManager* drawManager); // 起動する
};
