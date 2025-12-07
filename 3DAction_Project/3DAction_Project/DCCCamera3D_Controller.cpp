#include "DCCCamera3D_Controller.h"

// ===========================
// カメラをActiveにするか取得
// ===========================
bool DCCCamera3D_Controller::GetControlActive()
{
	// カメラを有効にするか
	if (m_Input->GetMousePress(MouseCode::Mouse_Right))
	{
		return true;
	}

	return false;
}


// ===========================
// 移動ベクトル取得
// ===========================
Vector2 DCCCamera3D_Controller::GetPositionVector()
{
	// 移動ベクトル
	Vector2 moveVec{ 0.0f, 0.0f };

	// キーボード入力
	if (m_Input->GetKeyPress(KeyCode::KeyCode_W))
	{
		moveVec.y += 1.0f;
	}
	if (m_Input->GetKeyPress(KeyCode::KeyCode_S))
	{
		moveVec.y -= 1.0f;
	}
	if (m_Input->GetKeyPress(KeyCode::KeyCode_A))
	{
		moveVec.x -= 1.0f;
	}
	if (m_Input->GetKeyPress(KeyCode::KeyCode_D))
	{
		moveVec.x += 1.0f;
	}

	// 正規化
	if (moveVec.Length() > 1.0f)
	{
		moveVec.Normalize();
	}

	// 反転適応
	moveVec.x *= m_IsInvertX;
	// 感度適応
	moveVec *= m_CameraPositionSensitivity;

	return moveVec;
}

// ===========================
// 視点移動ベクトル取得
// ===========================
Vector2 DCCCamera3D_Controller::GetLookVector()
{
	// 視点移動ベクトル
	Vector2 lookVec{ 0.0f, 0.0f };

	// マウス移動量取得
	lookVec = m_Input->GetMouseMoveAmount();
	
	// 正規化
	if (lookVec.Length() > 1.0f)
	{
		lookVec.Normalize();
	}

	// 感度,反転適用
	lookVec *= m_MouseSensitivity;
	lookVec.x *= m_IsInvertX;
	
	return lookVec;
}
