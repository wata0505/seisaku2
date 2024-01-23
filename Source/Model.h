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

	// 行列計算
	void UpdateTransform();

	// キーフレーム取得
	const Animation::Keyframe& GetKeyframe() const { return keyframe; }
	//std::vector<Node>& GetNodes() { return nodes; }

	// リソース取得
	const SkinnedMeshResouurce* GetResource() const { return resource.get(); }

	void SetResource(const std::shared_ptr<SkinnedMeshResouurce> r) { resource = r; }

	// アニメーション更新処理
	void UpdateAnimation(float elapsedTime, const char* rootname,bool renderflag = true);
	// サブアニメーション更新処理
	void UpdateSubAnimation(float elapsedTime, const char* rootname,bool renderflag = true);

	void TrimmingAnimation(float elapsedTime);

	void ClearSubAnimation();

	// アニメーション再生
	void PlayAnimation(int index, bool loop, float blendSeconds = 0.4f,bool clerSubAnim = true ,float currentAnimationSecond = 0.0f);
	// サブアニメーション再生
	void PlaySubAnimation(int subindex, bool loop, std::vector <const char*> Node,float blendSeconds = 0.2f, float currentAnimationSecond = 0.0f);
	//アニメーション追加
	void AppendAnimations(const char* Animation_filename, float samplingRate);
	//シリアライズ
	void ModelSerialize(const char*filename);
	//オブジェクト生成
	void ModelCreate(const char* filename, const char* texfilename = 0);
	//モデル登録
	void ModelRegister(const char* filename, const char* texfilename = 0, bool checkModel = true);

	//ノード操作
	void NodeOperation(const char* nodename,DirectX::XMFLOAT4 rotate);
	// アニメーション再生中か
	bool IsPlayAnimation() const;

	bool IsSubPlayAnimation() const;
	//アニメーションが終わったか
	bool EndPlayAnimation() const;

	// 現在のアニメーション再生時間取得
	float GetCurrentAnimationSeconds()const { return currentAnimationSeconds; }
	// 現在のサブアニメーション再生時間取得
	float GetCurrentSubAnimationSeconds()const { return currentSubAnimationSeconds; }
	//ルートモーション用
	float GetRootPow()const { return rootPow; }
	//現在何のアニメーションか取得
	int GetCurrentAnimationIndex()const { return currentAnimationIndex; }
	//現在何のサブアニメーションか取得
	int GetCurrentSubAnimationIndex()const { return currentSubAnimationIndex; }
	//描画情報取得
	void UpdateBufferData(const DirectX::XMFLOAT4X4& world);
	//サブノードの描画情報取得
	void UpdateSubBufferDara(const DirectX::XMFLOAT4X4& world, std::vector <const char*> Node);
	
	std::vector <SkinnedMeshResouurce::constants>& GetBufferData() { return data; };
	//描画情報の行列取得
	DirectX::XMFLOAT4X4 GetBufferTransform() { return data.at(0).world; };

	//描画情報のワールド行列更新
	void UpdateWorldBufferData(const DirectX::XMFLOAT4X4& world);
	//ループフラグ取得
	bool GetLoopflag() { return animationLoopFlag; }
	//描画情報設定
	void SetBufferData(std::vector <SkinnedMeshResouurce::constants> data) { this->data = data; };

	// ノード検索
	Animation::Keyframe::node* FindNode(const char* name);

	Animation::Keyframe GetKeyframe() { return keyframe; }

	// シェーダー情報調整
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