#pragma once

// ==============================
// ヘッダー
// ==============================
#include <functional>


// ==============================
// サンプラー設定用
// ==============================
// フィルター
enum class SamplerFilter
{
	// そのまま
	Point,

	// 補間
	Linear,
	Anisotropic
};

// 範囲を超えた時の処理
enum class SamplerAddressMode
{
	Wrap, // 繰り返す
	Mirror, // 折り返し
	Clamp,  // 端の色を伸ばす
	Border  // 境界線を使用
};

// 比較
enum class SamplerComparisonFunc
{
	Never,
	Less,
	Equal,
	LessEqual,
	Greater,
	NotEqual,
	GreaterEqual,
	Always
};

// サンプラーの設定をまとめた構造体
struct SamplerDesc {
	// フィルター
	SamplerFilter filter;

	// アドレス
	SamplerAddressMode addressU;
	SamplerAddressMode addressV;
	SamplerAddressMode addressW; // 2Dの時は Clamp を設定してください。

	// 比較
	SamplerComparisonFunc comparisonFunc;

	// コンストラクタ
	SamplerDesc(
		SamplerFilter _filter,
		SamplerAddressMode _addressU,
		SamplerAddressMode _addressV,
		SamplerAddressMode _addressW = SamplerAddressMode::Clamp,
		SamplerComparisonFunc _comparisonFunc = SamplerComparisonFunc::Never) :
		filter(_filter),
		addressU(_addressU),
		addressV(_addressV),
		addressW(_addressW),
		comparisonFunc(_comparisonFunc)
	{
	}

	// 基本サンプラー
	static SamplerDesc NormalSampler()
	{
		// 基本サンプラー
		SamplerDesc normalDesc = {
				SamplerFilter::Linear,
				SamplerAddressMode::Wrap,
				SamplerAddressMode::Wrap,
				SamplerAddressMode::Clamp,
				SamplerComparisonFunc::Never };

		return normalDesc;
	}

	// 比較演算子
	inline bool operator==(const SamplerDesc& other) const noexcept
	{
		return filter == other.filter &&
			addressU == other.addressU &&
			addressV == other.addressV &&
			addressW == other.addressW &&
			comparisonFunc == other.comparisonFunc;
	}

};

// ハッシュ関数を作成する
struct SamplerDescHash
{
	size_t operator()(const SamplerDesc& _desc) const noexcept
	{
		// 各メンバーを int にしてハッシュ値を作成する
		size_t h1 = std::hash<int>()(static_cast<int>(_desc.filter));
		size_t h2 = std::hash<int>()(static_cast<int>(_desc.addressU));
		size_t h3 = std::hash<int>()(static_cast<int>(_desc.addressV));
		size_t h4 = std::hash<int>()(static_cast<int>(_desc.addressW));
		size_t h5 = std::hash<int>()(static_cast<int>(_desc.comparisonFunc));

		// 全てのハッシュ値を混ぜる　(XORと左にビットをずらして簡単なハッシュ値を計算する)
		return (((((h1 ^ (h2 << 1)) ^ (h3 << 1)) ^ (h4 << 1)) ^ (h5 << 1)));	
	}
};