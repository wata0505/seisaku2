#include "Graphics.h"
#include "BaseModel.h"
#include "ResourceManager.h"

// コンストラクタ
BaseModel::BaseModel(const char* filename,bool triangulate)
{
	// リソース読み込み
	resource = std::make_shared<SkinnedMeshResouurce>();
	//resource = ResourceManager::Instance().LoadModelResource(filename, triangulate);
}

// デバッグ情報表示
void BaseModel::DrawDebugGUI()
{
	// デバッグ用にシェーダー定数文字列のリスト生成
	//constexpr const char* ModelShaderNames[] =
	//{
	//	"Default",
	//	"ShadowmapCaster",
	//	"Phong",
	//	"WaterSurface",
	//	"PBR",
	//};
	////	シェーダー定数とシェーダー名リストとのサイズが違うとエラーを出す
	//static_assert(ARRAYSIZE(ModelShaderNames) != static_cast<int>(ModelShaderId::Max) - 1, "ModelShaderNames Size Error!");
	//
	//ImGui::Checkbox("static", &staticFlag);
	//if (ImGui::TreeNode("Materials"))
	//{
	//	//	材質変更
	//	for (const SkinnedMeshResouurce::Material& material : resource->GetMaterials())
	//	{
	//		SkinnedMeshResouurce::Material& mat = const_cast<SkinnedMeshResouurce::Material&>(material);
	//		if (ImGui::TreeNode(mat.name.c_str()))
	//		{
	//			//	シェーダー
	//			ImGui::Combo("Shader", &mat.shaderId, ModelShaderNames, static_cast<int>(ModelShaderId::Max), 5);
	//
	//			//	テクスチャ名
	//			ImGui::Text("DiffuseTexture Name");
	//			ImGui::Text(mat.textureFilename.c_str());
	//
	//			//	マテリアルカラー
	//			ImGui::ColorEdit4("Material Color", &mat.color.x);
	//
	//			//	マテリアルパラメーター
	//			switch (static_cast<ModelShaderId>(mat.shaderId))
	//			{
	//			case ModelShaderId::Phong:
	//			{
	//				ImGui::SliderFloat3("ka", &mat.Ka.x, 0.0f, 1.0f);
	//				ImGui::SliderFloat3("kd", &mat.Kd.x, 0.0f, 1.0f);
	//				ImGui::SliderFloat3("ks", &mat.Ks.x, 0.0f, 1.0f);
	//				//ImGui::SliderFloat("shiness", &mat.phong.shiness, 0.0001f, 128.0f);
	//				break;
	//			}
	//			case ModelShaderId::WaterSurface:
	//			{
	//				ImGui::SliderFloat3("ka", &mat.waterSurface.phong.ka.x, 0.0f, 1.0f);
	//				ImGui::SliderFloat3("kd", &mat.waterSurface.phong.kd.x, 0.0f, 1.0f);
	//				ImGui::SliderFloat3("ks", &mat.waterSurface.phong.ks.x, 0.0f, 1.0f);
	//				ImGui::SliderFloat("shiness", &mat.waterSurface.phong.shiness, 0.0001f, 128.0f);
	//				ImGui::SliderFloat2("waveScroll1", &mat.waterSurface.waveScroll1.x, -0.1f, 0.1f);
	//				ImGui::SliderFloat2("waveScroll2", &mat.waterSurface.waveScroll2.y, -0.1f, 0.1f);
	//				break;
	//			}
	//			case ModelShaderId::PBR:
	//			{
	//				ImGui::SliderFloat("AdjustMetalness", &mat.pbr.adjustMetalness, -1.0f, 1.0f);
	//				ImGui::SliderFloat("AdjustSmoothness", &mat.pbr.adjustSmoothness, -1.0f, 1.0f);
	//				break;
	//			}
	//			}
	//			ImGui::TreePop();
	//		}
	//	}
	//	ImGui::TreePop();
	//}
}
