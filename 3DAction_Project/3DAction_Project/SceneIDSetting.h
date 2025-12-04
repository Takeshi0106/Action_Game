#pragma once

// ===============================
// シーンを設定するヘッダー
// この列挙型を使用してシーンを切り替える
// ===============================
enum SceneEventID
{
	NONE = -1, // イベントなし

	DEBUGSCENE = 0, // デバッグシーン
	TITLESCENE,    // タイトルシーン
	GAMEMAINSCENE, // ゲームメインシーン
	RESULTSCENE,   // リザルトシーン

	MAX_SCENE, // シーンの最大数
};
