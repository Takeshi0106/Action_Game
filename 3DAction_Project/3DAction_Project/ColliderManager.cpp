
// =====================================
// ヘッダー
// =====================================
// 必須ヘッダー
#include "ColliderManager.h"
// 固定整数ヘッダー
#include <cstdint>


// =====================================
// コライダー追加
// =====================================
ColliderInfo ColliderManager::ColliderAdd(const std::string& _name, const AABBCollider _collider)
{
	// 戻り値
	ColliderInfo info = {};

	// AABBコライダー追加
	uint32_t ID = m_AABBManager.AddData(_name, _collider);

	// 情報設定
	info.type = ColliderType::AABB;
	info.ID = ID;

	return info;
}


// =====================================
// コライダーの当たり判定
// =====================================
bool ColliderManager::CheckCollision(
	const ColliderInfo& _aColInfo, const Vector3& _aPos, const Vector3& _aSize,
	const ColliderInfo& _bColInfo, const Vector3& _bPos, const Vector3& _bSize)
{
	switch (_aColInfo.type)
	{
	case ColliderType::AABB:

		switch (_bColInfo.type)
		{
		case ColliderType::AABB:

			// 当たり判定
			return m_CollisionModule.CheckCollision(
				*m_AABBManager.GetData(_aColInfo.ID), _aPos, _aSize,
				*m_AABBManager.GetData(_bColInfo.ID), _bPos, _bSize);
			break;

		case ColliderType::OBB:
			break;

		case ColliderType::SPHERE:
			break;

		}
		break;

	case ColliderType::OBB:

		switch (_bColInfo.type)
		{
		case ColliderType::AABB:
			break;

		case ColliderType::OBB:
			break;

		case ColliderType::SPHERE:
			break;

		}
		break;

	case ColliderType::SPHERE:

		switch (_bColInfo.type)
		{
		case ColliderType::AABB:
			break;

		case ColliderType::OBB:
			break;

		case ColliderType::SPHERE:
			break;

		}
		break;
	}

	return true;
}
