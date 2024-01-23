#pragma once


#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "SkinnedMesh.h"
class Model
{
public:
	Model(const char* filename,bool triangulate,bool leftflag = true);
	~Model() {}

	//struct Keyframe
	//{
	//	struct node
	//	{
	//		// 'globalTransform' is used to convert from local space of node to global space of scene.
	//		DirectX::XMFLOAT4X4 globalTransform{ 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	//
	//		//The transformation data of a node includes its translation, rotation and scaling vectors
	//		//with respect to its parent. 
	//		DirectX::XMFLOAT3 scaling{ 1,1,1 };
	//		DirectX::XMFLOAT4 rotation{ 0,0,0,1 }; //Rotation quaternion
	//		DirectX::XMFLOAT3 translation{ 0,0,0 };
	//		template<class T>
	//		void serialize(T& archive)
	//		{
	//			archive(globalTransform, scaling, rotation, translation);
	//		}
	//
	//	};
	//	std::vector<node> nodes;
	//
	//	
	//};

	// �s��v�Z
	void UpdateTransform();

	// �L�[�t���[���擾
	const Animation::Keyframe& GetKeyframe() const { return keyframe; }
	//std::vector<Node>& GetNodes() { return nodes; }

	// ���\�[�X�擾
	const SkinnedMeshResouurce* GetResource() const { return resource.get(); }

	void SetResource(const std::shared_ptr<SkinnedMeshResouurce> r) { resource = r; }

	// �A�j���[�V�����X�V����
	void UpdateAnimation(float elapsedTime, const char* rootname,bool renderflag = true);
	// �T�u�A�j���[�V�����X�V����
	void UpdateSubAnimation(float elapsedTime, const char* rootname,bool renderflag = true);

	void TrimmingAnimation(float elapsedTime);

	void ClearSubAnimation();

	// �A�j���[�V�����Đ�
	void PlayAnimation(int index, bool loop, float blendSeconds = 0.4f,bool clerSubAnim = true ,float currentAnimationSecond = 0.0f);
	// �T�u�A�j���[�V�����Đ�
	void PlaySubAnimation(int subindex, bool loop, std::vector <const char*> Node,float blendSeconds = 0.2f, float currentAnimationSecond = 0.0f);
	//�A�j���[�V�����ǉ�
	void AppendAnimations(const char* Animation_filename, float samplingRate);
	//�V���A���C�Y
	void ModelSerialize(const char*filename);
	//�I�u�W�F�N�g����
	void ModelCreate(const char* filename, const char* texfilename = 0);
	//���f���o�^
	void ModelRegister(const char* filename, const char* texfilename = 0, bool checkModel = true);

	//�m�[�h����
	void NodeOperation(const char* nodename,DirectX::XMFLOAT4 rotate);
	// �A�j���[�V�����Đ�����
	bool IsPlayAnimation() const;

	bool IsSubPlayAnimation() const;
	//�A�j���[�V�������I�������
	bool EndPlayAnimation() const;

	// ���݂̃A�j���[�V�����Đ����Ԏ擾
	float GetCurrentAnimationSeconds()const { return currentAnimationSeconds; }
	// ���݂̃T�u�A�j���[�V�����Đ����Ԏ擾
	float GetCurrentSubAnimationSeconds()const { return currentSubAnimationSeconds; }
	//���[�g���[�V�����p
	float GetRootPow()const { return rootPow; }
	//���݉��̃A�j���[�V�������擾
	int GetCurrentAnimationIndex()const { return currentAnimationIndex; }
	//���݉��̃T�u�A�j���[�V�������擾
	int GetCurrentSubAnimationIndex()const { return currentSubAnimationIndex; }
	//�`����擾
	void UpdateBufferData(const DirectX::XMFLOAT4X4& world);
	//�T�u�m�[�h�̕`����擾
	void UpdateSubBufferDara(const DirectX::XMFLOAT4X4& world, std::vector <const char*> Node);
	
	std::vector <SkinnedMeshResouurce::constants>& GetBufferData() { return data; };
	//�`����̍s��擾
	DirectX::XMFLOAT4X4 GetBufferTransform() { return data.at(0).world; };

	//�`����̃��[���h�s��X�V
	void UpdateWorldBufferData(const DirectX::XMFLOAT4X4& world);
	//���[�v�t���O�擾
	bool GetLoopflag() { return animationLoopFlag; }
	//�`����ݒ�
	void SetBufferData(std::vector <SkinnedMeshResouurce::constants> data) { this->data = data; };

	// �m�[�h����
	Animation::Keyframe::node* FindNode(const char* name);

	Animation::Keyframe GetKeyframe() { return keyframe; }

	// �V�F�[�_�[��񒲐�
	void ShaderAdjustment(float glitchScale, float timer, float maxHeight, DirectX::XMFLOAT3 hologramColor = { 1.0f, 0.0f, 0.0f });

private:
	std::shared_ptr<SkinnedMeshResouurce>	resource;
    Animation::Keyframe keyframe = {  };
	int		currentAnimationIndex = -1;
	float	currentAnimationSeconds = 0.0f;
	bool animationLoopFlag = false;
	bool animationEndFlag = false;
	float animationBlendTime = 0.0f;
	float animationBlendSeconds = 0.0f;
	int		currentSubAnimationIndex = -1;
	float	currentSubAnimationSeconds = 0.0f;
	bool animationSubLoopFlag = false;
	bool animationSubEndFlag = false;
	bool SubAnimationFlag = false;
	float animationSubBlendTime = 0.0f;
	float animationSubBlendSeconds = 0.0f;
	float rootPow = 0;
	std::vector<SkinnedMeshResouurce::constants> data;
	std::vector <const char*> subNode;
};