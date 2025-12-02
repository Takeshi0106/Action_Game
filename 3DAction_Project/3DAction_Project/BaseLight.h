#pragma once

// ===============================
// 基底ライトクラス
// ===============================


// ===============================
// ヘッダー
// ===============================
// 描画マネージャー
#include "BaseDrawManager.h"
// 計算ヘッダー
#include "Color.h"


// ===============================
// クラス
// ===============================
class BaseLight
{
protected:
	// 描画マネージャー
	BaseDrawManager* m_Draw = nullptr;

	// 反射光
	Color m_Diffuse;
	// 環境光
	Color m_Ambient;
	// 鏡面反射光
	Color m_Specular;

	// 派生初期化
	virtual bool DerivativeInit() = 0;

public:
	// コンストラクタ / デストラクタ
	BaseLight()
		: m_Diffuse(1.0f, 1.0f, 1.0f, 1.0f)
		, m_Ambient(0.2f, 0.2f, 0.2f, 1.0f)
		, m_Specular(1.0f, 1.0f, 1.0f, 1.0f)
	{
	}
	virtual ~BaseLight() = default;

	// GPU転送用仮想関数
	virtual bool Init(BaseDrawManager* _Draw) {
		m_Draw = _Draw;
		return DerivativeInit();
	}
	virtual void Update() = 0;
	virtual void UpdateToGPU() = 0;
	virtual void Uninit() = 0;

	// セッター
	void SetDiffuse(const Color& diffuse) { m_Diffuse = diffuse; }
	void SetAmbient(const Color& ambient) { m_Ambient = ambient; }
	void SetSpecular(const Color& specular) { m_Specular = specular; }
};

