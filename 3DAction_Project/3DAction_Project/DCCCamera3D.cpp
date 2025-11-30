
// =================================
// ヘッダー
// =================================
// 必須ヘッダー
#include "DCCCamera3D.h"
// 計算ヘッダー
#include "Quaternionh.h"
// 文字列
#include <string>
// ログ出力ヘッダー
#include "ReportMessage.h"


// =================================
// 派生初期化
// =================================
bool DCCCamera3D::DerivativInit()
{
	if (!m_Controller.IsInit()) {
		ErrorLog::OutputToConsole("DCCCamera3Dのコントローラーが初期化されていません");
		return false;
	}

	return true;
}


// =================================
// コントローラーのセット
// =================================
void DCCCamera3D::SetInput(Input* input)
{
	// コントローラー初期化
	m_Controller.Init(input);
}


// =================================
// 更新
// =================================
void DCCCamera3D::Update()
{
	// カメラ操作が有効出なければ実行しない
	if (m_Controller.GetControlActive() == false) { return; }

	// カメラを動かす
	MoveCamera();
}


// =================================
// カメラを動かす
// =================================
void DCCCamera3D::MoveCamera()
{
	// 位置移動ベクトル取得
	Vector2 movePosition = m_Controller.GetPositionVector();
	// 視点移動ベクトル取得
	Vector2 moveLook = m_Controller.GetLookVector();

	// 前のフレームのカメラの前方向ベクトル計算
	Vector3 forward = (m_ViewInfo.LookPoint - m_ViewInfo.CameraPos).Normalize();
	// 前のフレームのカメラの上方向ベクトル計算
	Vector3 up = m_ViewInfo.UpDir;
	// 前のフレームのカメラの右方向ベクトル計算
	Vector3 right = forward.Cross(up).Normalize();

	// 軸計算 (視線移動クォータニオンを計算)
	Quaternion pitchQ = Quaternion::CreateQuaternionFromAxisAngle(right, moveLook.y);
	Quaternion yawQ = Quaternion::CreateQuaternionFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), moveLook.x);
	Quaternion rotateQ = (yawQ * pitchQ).Normalize();

	// 新しい方向ベクトルを計算
	forward = rotateQ.RotateVector(forward).Normalize();
	up = rotateQ.RotateVector(up).Normalize();
	right = forward.Cross(up).Normalize();

	// 移動ベクトル計算
	Vector3 localMove = forward * movePosition.y + right * movePosition.x;

	// カメラ位置・視点更新
	m_ViewInfo.CameraPos += localMove;
	m_ViewInfo.LookPoint = m_ViewInfo.CameraPos + forward;
	m_ViewInfo.UpDir = up;
}
