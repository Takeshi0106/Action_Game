#include "PlayerController.h"

// ===========================
// 移動ベクトル取得
// ===========================
Vector2 PlayerController::GetMoveVector()
{
	// 移動ベクトル
	Vector2 moveVec{ 0.0f, 0.0f };
	
	// キーボード入力
	if (m_Input->GetKeyPress(KeyCode_W))
	{
		moveVec.y += 1.0f;
	}
	if (m_Input->GetKeyPress(KeyCode_S))
	{
		moveVec.y -= 1.0f;
	}
	if (m_Input->GetKeyPress(KeyCode_A))
	{
		moveVec.x -= 1.0f;
	}
	if (m_Input->GetKeyPress(KeyCode_D))
	{
		moveVec.x += 1.0f;
	}

	// 正規化
	if (moveVec.Length() > 1.0f)
	{
		moveVec.Normalize();
	}

	return moveVec;
}
