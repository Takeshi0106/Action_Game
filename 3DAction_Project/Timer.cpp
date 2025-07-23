#include "Timer.h"
#include <Windows.h>

#if defined(DEBUG) || defined(_DEBUG)
// デバッグ時にヘッダーをコンパイル
#include <cassert>
#endif

// 静的メンバー変数
namespace Timer {
	namespace {
		// =====================================================
		// 変数
		// =====================================================
		LARGE_INTEGER frequency; // 一秒間の経過時間
		LARGE_INTEGER lastTime;  // 前回の計測時間を保存する
		LARGE_INTEGER startTime; // 計測開始時間

#if defined(DEBUG) || defined(_DEBUG)
		// デバッグ時以外ではコンパイルしない
		bool IsInit = false;
		bool IsStart = false;
		bool IsCheck = false;
		unsigned int CollCount = 0;
		unsigned int LastCount = 1;
#endif
	}


	// =====================================================
    // 前方宣言を構造体として定義
    // =====================================================
	struct TimePoint {
		LARGE_INTEGER time;

		// コンストラクタ
		TimePoint(const LARGE_INTEGER& largeInteger) : time(largeInteger) {}
	};


	// =====================================================
	// 初期化
	// =====================================================
	void Init()
	{
		QueryPerformanceFrequency(&frequency); // 周波数を取得
		QueryPerformanceCounter(&lastTime);    // 初期化が呼ばれた時間を取得
#if defined(DEBUG) || defined(_DEBUG)
		IsInit = true; // デバッグ時以外ではコンパイルされない
#endif
	}


	// =====================================================
	// 計測開始時間を取得する
	// =====================================================
	void Start()
	{
		QueryPerformanceCounter(&startTime); // 計測開始時間を取得
#if defined(DEBUG) || defined(_DEBUG)
		IsStart = true; // デバッグ時以外ではコンパイルされない
#endif
	}


#if defined(DEBUG) || defined(_DEBUG)
	// =====================================================
	// デバック時にのみ有効
	// 毎フレームLastUpdateが呼ばれているかチェックするクラス
	// =====================================================
	void Debug_CheckUpdate()
	{
		if (LastCount == CollCount)
		{
			assert(false && "LastUpdate()が前のフレームで呼ばれていません");
		}
		assert(!IsCheck && "Debug_CheckeUpdate()が複数回呼ばれています");
		LastCount = CollCount;
		IsCheck = true;
	}
#endif


	// =====================================================
	// 最後に呼び出す
	// =====================================================
	void LastUpdate()
	{
#if defined(DEBUG) || defined(_DEBUG)
		if (LastCount < CollCount)
		{
			assert(false && "LastUpdate()が複数回呼ばれています");
		}
		CollCount++;
		IsCheck = false;
#endif
		QueryPerformanceCounter(&lastTime);    // 初期化が呼ばれた時間を取得
	}


	// =====================================================
	// 前のフレームからの経過時間を返す
	// =====================================================
	float GetDeltaTime()
	{
#if defined(DEBUG) || defined(_DEBUG)
		assert(IsInit && "初期化処理が呼ばれていません"); // 初期化フラグがfalseのときプログラムを強制終了
#endif
		// 今の時間を取得
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);
		// 前のカウントと今のカウントを引いて周波数で割り、前のフレームとの時間を取得
		return static_cast<float>(currentTime.QuadPart - lastTime.QuadPart) / frequency.QuadPart;
	}


	// =====================================================
	// 引き数で渡された時間からの計測時間を返す
	// =====================================================
	float GetElapsedTime(const TimePoint& _startTime)
	{
#if defined(DEBUG) || defined(_DEBUG)
		assert(IsInit && "初期化処理が呼ばれていません");    // 初期化フラグがfalseのときプログラムを強制終了
#endif
		// 今の時間を取得
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);
		// カウントを周波数で割って経過時間を求める
		return static_cast<float>(currentTime.QuadPart - _startTime.time.QuadPart) / frequency.QuadPart;
	}


	// =====================================================
	// 計測開始時間からの経過時間を返す関数
	// =====================================================
	float GetElapsedTime()
	{
#if defined(DEBUG) || defined(_DEBUG)
		assert(IsInit && "初期化処理が呼ばれていません");    // 初期化フラグがfalseのときプログラムを強制終了
		assert(IsStart && "計測開始処理がよばれていません"); // 計測開始フラグがfalseのときプログラムを強制終了
#endif
		// 今の時間を取得
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);
		// カウントを周波数で割って経過時間を求める
		return static_cast<float>(currentTime.QuadPart - startTime.QuadPart) / frequency.QuadPart;
	}
}