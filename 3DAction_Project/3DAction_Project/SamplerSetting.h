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

// ==============================
// サンプラーの設定をまとめた構造体
// 
// ==============================
struct SamplerDesc 
{
private:
	// ハッシュ値
	size_t m_Hash = 0;

public:
	// フィルター
	const SamplerFilter filter;

	// アドレス
	const SamplerAddressMode addressU;
	const SamplerAddressMode addressV;
	const SamplerAddressMode addressW; // 2Dの時は Clamp を設定してください。

	// 比較
	const SamplerComparisonFunc comparisonFunc;

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
		// 各メンバーを int にしてハッシュ値を作成する
		size_t h1 = std::hash<int>()(static_cast<int>(filter));
		size_t h2 = std::hash<int>()(static_cast<int>(addressU));
		size_t h3 = std::hash<int>()(static_cast<int>(addressV));
		size_t h4 = std::hash<int>()(static_cast<int>(addressW));
		size_t h5 = std::hash<int>()(static_cast<int>(comparisonFunc));

		// 全てのハッシュ値を混ぜる　(XORと左にビットをずらして簡単なハッシュ値を計算する)
		m_Hash = ((((h1 ^ (h2 << 1)) ^ (h3 << 1)) ^ (h4 << 1)) ^ (h5 << 1));
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

	// ハッシュ値取得
	size_t GetHash() const noexcept
	{
		return m_Hash;
	}
};


// ハッシュ関数
struct SamplerDescHash
{
	size_t operator()(const SamplerDesc& desc) const noexcept
	{
		// SamplerDesc 内に保持しているハッシュ値を返すだけ
		return desc.GetHash();
	}
};
