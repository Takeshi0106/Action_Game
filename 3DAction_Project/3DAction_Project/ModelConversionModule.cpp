
// =====================================
// ヘッダー
// =====================================
// 必須ヘッダー
#include "ModelConversionModule.h"
// アシンプヘッダー
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#pragma comment(lib, "assimp-vc143-mtd.lib")
// ファイルシステム
#include <filesystem>
// メッシュ情報
#include "MeshData.h"
// ログ出力
#include "ReportMessage.h"


// =====================================
// 初期化
// =====================================
bool ModelConversionModule::Init()
{
	// モデルが入っているパス内を捜査
	for (const auto& entry : std::filesystem::directory_iterator(m_ModelPath))
	{
		// モデルかチェック
		if (!entry.is_regular_file() || entry.path().extension() != ".obj") { continue; }

		// モデル読込
		Assimp::Importer importer;

		// フラグ　３角形・法線生成・重複頂点削除
		const aiScene* scene = importer.ReadFile(entry.path().string(),
			aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);

		// エラーチェック
		if (!scene) {
			ErrorLog::OutputToConsole("モデルロード失敗");
			return false;
		}

		// データの取得
		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			// メッシュ
			MeshData mesh = {};

			// 頂点の作成
			std::vector<Vertex> vtx;
			// 領域確保
			vtx.resize(scene->mMeshes[i]->mNumVertices);

			for (unsigned int j = 0; j < vtx.size(); j++)
			{
				// 値の吸出し
				aiVector3D pos = scene->mMeshes[i]->mVertices[j];
				aiVector3D uv = scene->mMeshes[i]->HasTextureCoords(0) ?
					scene->mMeshes[i]->mTextureCoords[0][j] : aiVector3D(0.0f, 0.0f, 0.0f);
				aiVector3D normal = scene->mMeshes[i]->HasNormals() ?
					scene->mMeshes[i]->mNormals[j] : aiVector3D(0.0f, 0.0f, 0.0f);

				// 値を設定
				vtx[j] = {
					Vector3(pos.x, pos.y, pos.z),
					Vector3(normal.x, normal.y, normal.z),
					Vector2(uv.x, uv.y) };
			}

			// インデックスの作成
			std::vector<int> idx;
			// 領域確保
			idx.resize(scene->mMeshes[i]->mNumFaces * 3);

			for (unsigned int j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
			{
				aiFace face = scene->mMeshes[i]->mFaces[j];
				int faceIdx = j * 3;
				idx[faceIdx + 0] = face.mIndices[0];
				idx[faceIdx + 1] = face.mIndices[1];
				idx[faceIdx + 2] = face.mIndices[2];
			}

			// マテリアル情報作成
			aiMaterial* material = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];
			aiString texPath;

			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS)
			{
				std::string texturePath = texPath.C_Str();
				// 相対パス補正
				std::filesystem::path fullPath = std::filesystem::path(m_ModelPath) / texturePath;
			}
		}
	}

	return true;
}


// =====================================
// 
// =====================================
