
// ====================================================
// 【関数概要】
// DirectX用の時間取得変数
// ====================================================


// ====================================================
// ヘッダー
// ====================================================
// 必須ヘッダー
#include "Timer.h" // 自分のヘッダー
// 周波数や時間を取得するヘッダー
#include <Windows.h>
// 配列ヘッダー
#include <unordered_map>
// 名前検索用ヘッダー
#include <string>
// デバッグ出力用ヘッダー
#include "ReportMessage.h"

#if defined(DEBUG) || defined(_DEBUG)
#include <cassert> // プロジェクトを停止するヘッダー

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
		// デバッグ時用のチェック変数
		bool IsInit = false;        // 初期化フラグ
		bool IsStart = false;       // 開始フラグ
		bool IsCheck = false;       // 最終更新フラグ
		unsigned int CollCount = 0; // 呼び出された回数
		unsigned int LastCount = 1; // 最終更新が必ず呼ばれているかのチェック
#endif

	}


	// =====================================================
	// 初期化
	// =====================================================
	void Init()
	{
		// 周波数を取得
		QueryPerformanceFrequency(&frequency);
		// 初期化が呼ばれた時間を取得
		QueryPerformanceCounter(&lastTime);

#if defined(DEBUG) || defined(_DEBUG)
		// 初期化が呼ばれたことを伝えるフラグ
		IsInit = true;
#endif
	}


	// =====================================================
	// 計測開始時間を取得する
	// =====================================================
	void Start()
	{
		QueryPerformanceCounter(&startTime); // 計測開始時間を取得

#if defined(DEBUG) || defined(_DEBUG)
		// 計測を開始したことを伝えるフラグ
		IsStart = true;
#endif
	}


#if defined(DEBUG) || defined(_DEBUG)
	// =====================================================
	// デバック時にのみ有効
	// 毎フレームLastUpdateが呼ばれているかチェックするクラス
	// =====================================================
	void Debug_CheckUpdate()
	{
		// 最終更新が毎フレーム呼ばれているかの確認
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
		// 初期化が呼ばれていないときプロジェクトを停止
		assert(IsInit && "初期化処理が呼ばれていません");

#endif

		// 今の時間を取得
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);
		// 前のカウントと今のカウントを引いて周波数で割り、前のフレームとの時間を取得
		return static_cast<float>(currentTime.QuadPart - lastTime.QuadPart) / frequency.QuadPart;
	}


	// =====================================================
	// 計測開始時間からの経過時間を返す関数
	// =====================================================
	float GetElapsedTime()
	{
#if defined(DEBUG) || defined(_DEBUG)
		// 初期化、計測開始が呼び出されていない場合、プロジェクトを停止
		assert(IsInit && "初期化処理が呼ばれていません");
		assert(IsStart && "計測開始処理がよばれていません");
#endif

		// 今の時間を取得
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);
		// カウントを周波数で割って経過時間を求める
		return static_cast<float>(currentTime.QuadPart - startTime.QuadPart) / frequency.QuadPart;
	}


	// 自分で追加できるタイム関数群
	namespace Label {

		namespace {
			// タイム配列
			std::unordered_map<std::string, LARGE_INTEGER> timers;
		}


		// =======================================
		// タイマー開始
		// =======================================
		void StartTimer(const char* name)
		{
			LARGE_INTEGER time;
			QueryPerformanceCounter(&time);
			timers[name] = time;
		}


		// ======================================
		// 経過時間取得
		// ======================================
		float GetElapsedTimer(const char* name)
		{
			auto it = timers.find(name);
			if (it == timers.end()) {
				ErrorLog::OutputToConsole((std::string(name) + "が見つかりませんでした").c_str());
				return 0.0f;
			}

			LARGE_INTEGER currentTime;
			QueryPerformanceCounter(&currentTime);
			return static_cast<float>(currentTime.QuadPart - it->second.QuadPart) / frequency.QuadPart;
		}


		// ======================================
		// タイマー削除
		// ======================================
		bool RemoveTimer(const char* name)
		{
			return timers.erase(name) > 0;
		}

		
		// ======================================
		// 全削除（リセット）
		// ======================================
		void ClearTimers()
		{
			timers.clear();
		}


	}

}