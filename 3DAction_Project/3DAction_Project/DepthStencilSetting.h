#pragma once

// ==============================
// 深度ステンシル設定用
// ステンシルは未対応
// ==============================
enum DepthStencilSetting
{
	// 深度テスト・書き込み ON
	DepthEnableON_DepthWriteON = 0,
	// 深度テストON 書き込み OFF
	DepthEnableON_DepthWriteOFF,
	// 深度テスト・書き込み OFF
	DepthEnableOFF_DepthWriteOFF,
	// 設定の最大数
	Depth_Setting_Max
};
