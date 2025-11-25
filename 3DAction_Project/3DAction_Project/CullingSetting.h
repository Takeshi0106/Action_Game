#pragma once

// ==============================
// かリング設定用
// ==============================
enum CullingSetting
{
	// かリングしない
	Not_Culling = 0,
	// 表面をかリング
	Front_Culling,
	// 後ろをかリング
	Back_Culling,
	// 設定の最大数
	Culling_Setting_Max
};
