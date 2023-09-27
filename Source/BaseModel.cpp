#include "Graphics.h"
#include "BaseModel.h"
#include "ResourceManager.h"

// �R���X�g���N�^
BaseModel::BaseModel(const char* filename,bool triangulate)
{
	// ���\�[�X�ǂݍ���
	resource = std::make_shared<SkinnedMeshResouurce>();
	//resource = ResourceManager::Instance().LoadModelResource(filename, triangulate);
}

// �f�o�b�O���\��
void BaseModel::DrawDebugGUI()
{
	// �f�o�b�O�p�ɃV�F�[�_�[�萔������̃��X�g����
	//constexpr const char* ModelShaderNames[] =
	//{
	//	"Default",
	//	"ShadowmapCaster",
	//	"Phong",
	//	"WaterSurface",
	//	"PBR",
	//};
	////	�V�F�[�_�[�萔�ƃV�F�[�_�[�����X�g�Ƃ̃T�C�Y���Ⴄ�ƃG���[���o��
	//static_assert(ARRAYSIZE(ModelShaderNames) != static_cast<int>(ModelShaderId::Max) - 1, "ModelShaderNames Size Error!");
	//
	//ImGui::Checkbox("static", &staticFlag);
	//if (ImGui::TreeNode("Materials"))
	//{
	//	//	�ގ��ύX
	//	for (const SkinnedMeshResouurce::Material& material : resource->GetMaterials())
	//	{
	//		SkinnedMeshResouurce::Material& mat = const_cast<SkinnedMeshResouurce::Material&>(material);
	//		if (ImGui::TreeNode(mat.name.c_str()))
	//		{
	//			//	�V�F�[�_�[
	//			ImGui::Combo("Shader", &mat.shaderId, ModelShaderNames, static_cast<int>(ModelShaderId::Max), 5);
	//
	//			//	�e�N�X�`����
	//			ImGui::Text("DiffuseTexture Name");
	//			ImGui::Text(mat.textureFilename.c_str());
	//
	//			//	�}�e���A���J���[
	//			ImGui::ColorEdit4("Material Color", &mat.color.x);
	//
	//			//	�}�e���A���p�����[�^�[
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
