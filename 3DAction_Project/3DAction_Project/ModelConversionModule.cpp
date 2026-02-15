
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
// モデル情報
#include "ModelData.h"
// ログ出力
#include "ReportMessage.h"


// =====================================
// 初期化
// =====================================
bool ModelConversionModule::ModelConversion()
{
	// モデルが入っているパス内を捜査
	for (const auto& entry : std::filesystem::directory_iterator(m_ModelPath.GetU8String()))
	{
		// モデルかチェック
		if (!entry.is_regular_file() || entry.path().extension() != kObjExtension.GetU8String()) { continue; }

		// モデルデータ
		ModelData modelData;

		// モデル読込み 重たいので注意
		if (!ModelLoad(entry.path().u8string().c_str(), aiProcessPreset_TargetRealtime_MaxQuality, modelData)) {
			ErrorLog::OutputToConsole(u8"モデルを読み込めませんでした");
			return false;
		}
	}

	return true;
}


// =====================================
// モデルを読み込んで、各マネージャーに登録する
// =====================================
bool ModelConversionModule::LoadAndRegisterModelResources(
	const Hashed_String& modelName, 
	BaseDrawManager& drawManager,ModelManager& modelManager,
	const String& modelFile)
{
	// 登録済みチェック
	if (modelManager.CheckModelData(modelName)) {
		return true;
	}

	// モデルデータ
	ModelData modelData;

	// モデルを読み込む
	if (!ModelLoad(modelName.GetString(),
		aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded,
		modelData,
		modelFile)) {
		ErrorLog::OutputToConsole(u8"モデルを読み込めませんでした");
		return false;
	}

	// 各バッファ作成
	for (size_t i = 0; i < modelData.meshDataArray.size(); i++)
	{
		// メッシュデータ取得
		MeshData& mesh = modelData.meshDataArray[i];
		// 登録名
		Hashed_String keyName = Hashed_String(modelName.GetString() + 
			String::to_u8string(i));

		// 頂点バッファ作成
		if (!drawManager.CreateVertexBuffer(
			keyName,
			mesh.vertices.data(),
			sizeof(Vertex),
			static_cast<uint32_t>(mesh.vertices.size()),
			static_cast<uint32_t>(mesh.vertices.size()),
			PrimitiveType::TriangleList))
		{
			ErrorLog::OutputToConsole(u8" 頂点バッファの作成に失敗しました");
			return false;
		}

		// インデックスバッファ作成
		if (!drawManager.CreateIndexBuffer(
			keyName,
			static_cast<const uint32_t*>(mesh.indices.data()),
			static_cast<uint32_t>(mesh.indices.size())))
		{
			ErrorLog::OutputToConsole(u8" インデックスバッファの作成に失敗しました");
			return false;
		}
	}

	// テクスチャ読込み
	for (size_t i = 0; i < modelData.materialDataArray.size(); i++)
	{
		// メッシュマテリアル情報取得
		MeshMaterialData materialData = modelData.materialDataArray[i];

		// テクスチャパスがあるか確認
		if (!materialData.textureName.GetString().GetU8String().empty())
		{
			// テクスチャロード
			if (!drawManager.LoadTexture(materialData.textureName)) {
				ErrorLog::OutputToConsole(u8" テクスチャの作成に失敗しました");
				return false;
			}

			// パスではなくファイル名に変換
			modelData.materialDataArray[i].textureName = 
				Hashed_String(std::filesystem::path(materialData.textureName.GetString().GetU8String()).filename().u8string());

			DebugLog::OutputToConsole(u8" テクスチャのロードに成功しました : ");
		}
	}

	// モデルマネージャーに登録
	modelManager.RegisterModel(modelName, modelData);

	return true;
}


// =====================================
// モデルを読み込む関数
// =====================================
bool ModelConversionModule::ModelLoad(
	const String& _modelPath, 
	int flag,
	ModelData& modelData, 
	const String& modelFile)
{
	// モデルが入っているパスを作成
	std::filesystem::path modelPath;

	// パスを作成
	if (!modelFile.IsEmpty())
	{
		modelPath = std::filesystem::path(m_ModelPath.GetU8String()) / 
			modelFile.GetU8String() /
			(_modelPath + kObjExtension).GetU8String();
	}
	else
	{
		modelPath = std::filesystem::path(m_ModelPath.GetU8String()) /
			(_modelPath + kObjExtension).GetU8String();
	}

	// 区切り文字統一
	modelPath.make_preferred();

	// モデルを読込むための変数
	Assimp::Importer importer;

	// モデルロード
	const aiScene* scene = importer.ReadFile(modelPath.string(), flag);

	// ロードチェック
	if (!scene) {
		ErrorLog::OutputToConsole(u8"モデルロード失敗");
		return false;
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
		idx.resize(static_cast<size_t>(scene->mMeshes[i]->mNumFaces * 3));

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
		materialData.materialCBData.diffuse = Color(aiDiffuse.r, aiDiffuse.g, aiDiffuse.b, opacity);
		materialData.materialCBData.ambient = Color(aiAmbient.r, aiAmbient.g, aiAmbient.b, aiAmbient.a);
		materialData.materialCBData.specular = Color(aiSpecular.r, aiSpecular.g, aiSpecular.b, aiSpecular.a);

		// テクスチャ名をセット
		materialData.textureName = Hashed_String(fullPath.u8string());

		// マテリアルデータをモデルデータに設定
		modelData.materialDataArray[i] = materialData;
	}

	return true;
}
