
// ============================================================
// ヘッダー
// ============================================================
// 必須ヘッダー
#include "ModelLoadeModule.h"
// アシンプヘッダー
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#pragma comment(lib, "assimp-vc143-mtd.lib")
// ファイルシステム
#include <filesystem>
// ログ出力
#include "ReportMessage.h"


// ============================================================
// モデル読み込み内部関数
// ============================================================
ModelData ModelLoadeModule::ModelLoad(const char* _modelPath, int _flag, const char* _modelFile)
{
	// String型に変換
	std::string modelPathStr = _modelPath;
	// モデルが入っているパスを作成
	std::filesystem::path modelPath;

	ModelData modelData = {};

	// パスを作成
	if (!modelPathStr.empty())
	{
		modelPath = std::filesystem::path(kModelPath) / modelPathStr
			/ ((_modelPath + std::string(kObjExtension)));
	}
	else
	{
		modelPath = std::filesystem::path(kModelPath) / (_modelPath + std::string(kObjExtension));
	}

	// 区切り文字統一
	modelPath.make_preferred();

	// モデルを読込むための変数
	Assimp::Importer importer;

	// モデルロード
	const aiScene* scene = importer.ReadFile(modelPath.string(), _flag);

	// ロードチェック
	if (!scene) {
		ErrorLog::OutputToConsole("モデルロード失敗");
		return modelData;
	}

	// メッシュ配列のリサイズ
	modelData.meshDataArray.resize(scene->mNumMeshes);

	// データの取得
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		// メッシュ
		MeshData mesh = {};

		// 頂点の作成
		std::vector<Vertex> vtx;
		// 領域確保
		vtx.resize(scene->mMeshes[i]->mNumVertices);

		// 頂点情報の作成
		for (unsigned int j = 0; j < vtx.size(); j++)
		{
			// 位置
			aiVector3D pos = scene->mMeshes[i]->mVertices[j];
			// 法線　情報がなければ0.0f
			aiVector3D normal = scene->mMeshes[i]->HasNormals() ?
				scene->mMeshes[i]->mNormals[j] : aiVector3D(0.0f, 0.0f, 0.0f);
			// UV　情報がなければ0.0f
			aiVector3D uv = scene->mMeshes[i]->HasTextureCoords(0) ?
				scene->mMeshes[i]->mTextureCoords[0][j] : aiVector3D(0.0f, 0.0f, 0.0f);
			// 頂点カラー 情報がなければ白
			aiColor4D color = scene->mMeshes[i]->HasVertexColors(0) ?
				scene->mMeshes[i]->mColors[0][j] : aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);

			// 値を設定
			vtx[j] = {
				Vector3(pos.x, pos.y, pos.z),
				Vector3(normal.x, normal.y, normal.z),
				Vector2(uv.x, uv.y),
				Color(color.r,color.g,color.b,color.a) };
		}

		// メッシュデータに設定
		mesh.vertices = vtx;


		// インデックスの作成
		std::vector<uint32_t> idx;
		// 領域確保
		idx.resize(scene->mMeshes[i]->mNumFaces * 3);

		// インデックス情報の作成
		for (uint32_t j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
		{
			aiFace face = scene->mMeshes[i]->mFaces[j];
			int faceIdx = j * 3;
			idx[faceIdx + 0] = face.mIndices[0];
			idx[faceIdx + 1] = face.mIndices[1];
			idx[faceIdx + 2] = face.mIndices[2];
		}

		// メッシュデータに設定
		mesh.indices = idx;

		// マテリアルIDを設定
		mesh.materialID = scene->mMeshes[i]->mMaterialIndex;

		// メッシュデータをモデルデータに設定
		modelData.meshDataArray[i] = mesh;
	}

	// マテリアル配列リサイズ
	modelData.materialDataArray.resize(scene->mNumMaterials);

	// マテリアル情報取得
	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		// アシンプでテクスチャのパス
		aiString texPath;
		// マテリアル情報を入れる配列
		MeshMaterialData materialData = {};
		// テクスチャのパスを取得
		std::filesystem::path fullPath = "";

		// テクスチャがあればパスを取得
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS)
		{
			// プロジェクトからの相対パスを作成
			fullPath = modelPath.parent_path() / texPath.C_Str();
			// 区切り文字を変換
			fullPath.make_preferred();
		}

		// マテリアルデータを取得
		aiColor4D aiDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
		aiColor4D aiAmbient(0.3f, 0.3f, 0.3f, 1.0f);
		aiColor4D aiSpecular(1.0f, 1.0f, 1.0f, 1.0f);
		float opacity = 1.0f;

		// マテリアルカラー取得（存在しなければデフォルトのまま）
		scene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, aiDiffuse);
		scene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, aiAmbient);
		scene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, aiSpecular);
		scene->mMaterials[i]->Get(AI_MATKEY_OPACITY, opacity);

		// MeshMaterialData にセット
		materialData.diffuse = Color(aiDiffuse.r, aiDiffuse.g, aiDiffuse.b, opacity);
		materialData.ambient = Color(aiAmbient.r, aiAmbient.g, aiAmbient.b, aiAmbient.a);
		materialData.specular = Color(aiSpecular.r, aiSpecular.g, aiSpecular.b, aiSpecular.a);

		// テクスチャ名をセット
		materialData.textureName = fullPath.string();

		// マテリアルデータをモデルデータに設定
		modelData.materialDataArray[i] = materialData;
	}

	return modelData;
}
