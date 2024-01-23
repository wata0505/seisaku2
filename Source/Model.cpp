#include "Graphics.h"
//#include "Graphics/Model.h"
#include "Model.h"
#include "Mathf.h"
#include"ResourceManager.h"

using namespace DirectX;
// コンストラクタ
Model::Model(const char* filename,bool triangulate, bool leftflag)
{
	// リソース読み込み
	//resource = std::make_shared<SkinnedMeshResouurce>();
	//resource->Load(Graphics::Instance().GetDevice(), filename,0);
	resource = ResourceManager::Instance().LoadModelResource(filename,leftflag,triangulate);
	// ノード
	const std::vector<Animation>& animations = resource->GetAnimations();
	//const Animation::Keyframe& keyframe{ resNodes.sequence.at(frame_index)
	


	// 行列計算
	//const DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	UpdateTransform();
}
//描画情報更新
void Model::UpdateBufferData(const DirectX::XMFLOAT4X4& world) {
	data.resize(resource->GetMeshes().size());
	for (int size = 0; size < data.size();size++)
	{
		//SkinnedMeshResouurce::constants data;
		if (keyframe.nodes.size() > 0)
		{
			const Animation::Keyframe::node& mesh_node{ keyframe.nodes.at(resource->GetMeshes().at(size).nodeIndex) };
			XMStoreFloat4x4(&data.at(size).world, XMLoadFloat4x4(&mesh_node.globalTransform) * XMLoadFloat4x4(&world));
	       
			const size_t bone_count{ resource->GetMeshes().at(size).bindPose.bones.size() };
			_ASSERT_EXPR(bone_count < resource->MAX_BONES, L"The value of the 'bone_const' has exceeded MAX_BONES.");
			for (size_t bone_index = 0; bone_index < bone_count; ++bone_index)
			{
				const SkinnedMeshResouurce::Skeleton::bone& bone{ resource->GetMeshes().at(size).bindPose.bones.at(bone_index) };
				const Animation::Keyframe::node& bone_node{ keyframe.nodes.at(bone.nodeIndex) };
				XMStoreFloat4x4(&data.at(size).boneTransforms[bone_index],
					XMLoadFloat4x4(&bone.offsetTransform) *
					XMLoadFloat4x4(&bone_node.globalTransform) *
					XMMatrixInverse(nullptr, XMLoadFloat4x4(&resource->GetMeshes().at(size).defaultGlobalTransform))
				);
			}
		}
		else
		{
			XMStoreFloat4x4(&data.at(size).world,
				XMLoadFloat4x4(&resource->GetMeshes().at(size).defaultGlobalTransform) * XMLoadFloat4x4(&world));
			for (size_t bone_index = 0; bone_index < resource->MAX_BONES; ++bone_index)
			{
				data.at(size).boneTransforms[bone_index] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
			}
		}
		
	}
}
void Model::UpdateSubBufferDara(const DirectX::XMFLOAT4X4& world, std::vector <const char*> Node) {
	data.resize(resource->GetMeshes().size());
	for (int size = 0; size < data.size(); size++)
	{
		//SkinnedMeshResouurce::constants data;
		if (keyframe.nodes.size() > 0)
		{
			const Animation::Keyframe::node& mesh_node{ keyframe.nodes.at(resource->GetMeshes().at(size).nodeIndex) };
			XMStoreFloat4x4(&data.at(size).world, XMLoadFloat4x4(&mesh_node.globalTransform) * XMLoadFloat4x4(&world));
			const size_t bone_count{ resource->GetMeshes().at(size).bindPose.bones.size() };
			_ASSERT_EXPR(bone_count < resource->MAX_BONES, L"The value of the 'bone_const' has exceeded MAX_BONES.");
			for (size_t bone_index = 0; bone_index < bone_count; ++bone_index)
			{
				const SkinnedMeshResouurce::Skeleton::bone& bone{ resource->GetMeshes().at(size).bindPose.bones.at(bone_index) };
				const Animation::Keyframe::node& bone_node{ keyframe.nodes.at(bone.nodeIndex) };
				for (int i = 0; i < Node.size();i++) {
					if(Node.at(i) == bone_node.name)
					XMStoreFloat4x4(&data.at(size).boneTransforms[bone_index],
						XMLoadFloat4x4(&bone.offsetTransform) *
						XMLoadFloat4x4(&bone_node.globalTransform) *
						XMMatrixInverse(nullptr, XMLoadFloat4x4(&resource->GetMeshes().at(size).defaultGlobalTransform))
					);
			    }
			}
		}
		else
		{
			XMStoreFloat4x4(&data.at(size).world,
				XMLoadFloat4x4(&resource->GetMeshes().at(size).defaultGlobalTransform) * XMLoadFloat4x4(&world));
			for (size_t bone_index = 0; bone_index < resource->MAX_BONES; ++bone_index)
			{
				data.at(size).boneTransforms[bone_index] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
			}
		}

	}
}
void Model::UpdateWorldBufferData(const DirectX::XMFLOAT4X4& world) {
	for (int size = 0; size < data.size(); size++)
	{
		//SkinnedMeshResouurce::constants data;
		if (keyframe.nodes.size() > 0)
		{
			const Animation::Keyframe::node& mesh_node{ keyframe.nodes.at(resource->GetMeshes().at(size).nodeIndex) };
			XMStoreFloat4x4(&data.at(size).world, XMLoadFloat4x4(&mesh_node.globalTransform) * XMLoadFloat4x4(&world));
		}
		else
		{
			XMStoreFloat4x4(&data.at(size).world,
				XMLoadFloat4x4(&resource->GetMeshes().at(size).defaultGlobalTransform) * XMLoadFloat4x4(&world));
		}

	}
}


// 変換行列計算
void Model::UpdateTransform()
{


	const size_t node_count{ keyframe.nodes.size() };
	for (size_t node_index = 0; node_index < node_count; ++node_index)
	{
		Animation::Keyframe::node& node{ keyframe.nodes.at(node_index) };
		XMMATRIX S{ XMMatrixScaling(node.scaling.x, node.scaling.y, node.scaling.z) };
		XMMATRIX R{ XMMatrixRotationQuaternion(XMLoadFloat4(&node.rotation)) };
		XMMATRIX T{ XMMatrixTranslation(node.translation.x, node.translation.y, node.translation.z) };

		const int64_t parent_index{ resource->GetNodes().nodes.at(node_index).parent_index };
		XMMATRIX P{ parent_index < 0 ? XMMatrixIdentity() : XMLoadFloat4x4(&keyframe.nodes.at(parent_index).globalTransform) };

		XMStoreFloat4x4(&node.globalTransform, S * R * T * P);
	}
	
}
void Model::PlayAnimation(int index, bool loop, float blendSeconds,bool claerSubAnim,float currentAnimationSecond)
{
	currentAnimationIndex = index;
	currentAnimationSeconds = currentAnimationSecond;
	animationLoopFlag = loop;
	animationEndFlag = false;
	animationBlendTime = 0.0f;
	animationBlendSeconds = blendSeconds;
	//サブアニメーション初期化
	if(claerSubAnim)ClearSubAnimation();
}
void Model::PlaySubAnimation(int index, bool loop, std::vector <const char*> Node, float blendSeconds, float currentAnimationSecond)
{
	currentSubAnimationIndex = index;
	currentSubAnimationSeconds = currentAnimationSecond;
	animationSubLoopFlag = loop;
	animationSubEndFlag = false;
	animationSubBlendTime = 0.0f;
	animationSubBlendSeconds = blendSeconds;
	subNode = Node;
}


void Model::AppendAnimations(const char* Animation_filename, float samplingRate) {
	resource->AppendAnimations(Animation_filename, samplingRate);
}
void Model::ModelSerialize(const char* filename) {
	resource->Serialize(filename);
}
void Model::ModelCreate(const char* filename, const char* texfilename) {
	ID3D11Device* device = Graphics::Instance().GetDevice();
	resource->CreateComObjects(device, filename,texfilename);
}
void Model::ModelRegister(const char* filename, const char* texfilename,bool checkModel) {
	ResourceManager::Instance().SkinnedMeshRegister(filename, resource, texfilename,checkModel);
	const std::vector<Animation>& animations = resource->GetAnimations();
	if (animations.size() > 0) {
		const Animation& animation = animations.at(0);
		const std::vector<Animation::Keyframe>& keyframes = animation.sequence;
		const Animation::Keyframe& reskeyframe = keyframes.at(0);

		keyframe = reskeyframe;
	}
}
void Model::NodeOperation(const char* nodename, DirectX::XMFLOAT4 rotate) {
	for (Animation::Keyframe::node& node : keyframe.nodes)
	{
		// 名前比較
		if (strcmp(node.name.c_str(), nodename) == 0) {
			node.globalTransform._31 = rotate.x;
			node.globalTransform._32 = rotate.y;
			node.globalTransform._33 = rotate.z;
			node.globalTransform._34 = rotate.w;
		}
	}

}
void Model::UpdateSubAnimation(float elapsedTime, const char* rootname, bool renderflag)
{
	// 再生中なら処理しない
	if (!IsSubPlayAnimation())return;

	// ブレンド率の計算
	if (subNode.size() == 0)return;
		float blendRate = 1.0f;
		if (animationSubBlendTime < animationBlendSeconds)
		{
			animationSubBlendTime += elapsedTime;
			if (animationSubBlendTime >= animationBlendSeconds)
			{
				animationSubBlendTime = animationBlendSeconds;
			}
			blendRate = animationSubBlendTime / animationBlendSeconds;
			blendRate *= blendRate;
		}

		// 指定のアニメーションデータを取得
		const std::vector<Animation>& animations = resource->GetAnimations();
		const Animation& animation = animations.at(currentSubAnimationIndex);
		//ModelKyeframe& Modelkeyframe = modelkyeframe.at(currentAnimationIndex);
		// アニメーションデータからキーフレームデータリストを取得
		const std::vector<Animation::Keyframe>& keyframes = animation.sequence;
		int keyCount = static_cast<int>(keyframes.size());
	if (renderflag) {
		for (int KeyIndex = 0; KeyIndex < keyCount - 1; ++KeyIndex)
		{
			//Animation::Keyframe& keyframe = Modelkeyframe.keyframes.at(KeyIndex);
			// 現在の時間がどのキーフレームの間にいるか判断する
			const Animation::Keyframe& keyframe0 = keyframes.at(KeyIndex);
			const Animation::Keyframe& keyframe1 = keyframes.at(KeyIndex + 1);

			if (currentSubAnimationSeconds >= keyframe0.seconds &&
				currentSubAnimationSeconds < keyframe1.seconds)
			{
				//再生時間とキーフレームの時間から補完率を算出する
				float rate = Mathf::Lerp(keyframe0.seconds, keyframe1.seconds, currentSubAnimationSeconds);
				int nodeCount = static_cast<int>(keyframe0.nodes.size());
				Animation::Keyframe::node tentative = {};
				int index = 0;
				for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
				{

					// 2つのキーフレーム間の補完計算
					const Animation::Keyframe::node& key0 = keyframe0.nodes.at(nodeIndex);
					const Animation::Keyframe::node& key1 = keyframe1.nodes.at(nodeIndex);

					//key0.name = 
					Animation::Keyframe::node& node = keyframe.nodes[nodeIndex];
					for (int i = 0; i < subNode.size(); i++) {
						if (subNode.at(i) == node.name) {
							if (node.name == rootname) {
								tentative = keyframe.nodes[nodeIndex];
								index = nodeIndex;
							}
							// ブレンド補完処理
							if (blendRate < 1.0f)
							{
								// 現在の姿勢と次のキーフレームとの姿勢の補完
								DirectX::XMVECTOR Scale0 = DirectX::XMLoadFloat3(&node.scaling);
								DirectX::XMVECTOR Scale1 = DirectX::XMLoadFloat3(&key1.scaling);
								DirectX::XMVECTOR Rotate0 = DirectX::XMLoadFloat4(&node.rotation);
								DirectX::XMVECTOR Rotate1 = DirectX::XMLoadFloat4(&key1.rotation);
								DirectX::XMVECTOR Translate0 = DirectX::XMLoadFloat3(&node.translation);
								DirectX::XMVECTOR Translate1 = DirectX::XMLoadFloat3(&key1.translation);

								DirectX::XMVECTOR Scale = DirectX::XMVectorLerp(Scale0, Scale1, blendRate);
								DirectX::XMVECTOR Rotate = DirectX::XMQuaternionSlerp(Rotate0, Rotate1, blendRate);
								DirectX::XMVECTOR Translate = DirectX::XMVectorLerp(Translate0, Translate1, blendRate);

								DirectX::XMStoreFloat3(&node.scaling, Scale);
								DirectX::XMStoreFloat4(&node.rotation, Rotate);
								DirectX::XMStoreFloat3(&node.translation, Translate);
							}
							// 通常の計算
							else
							{
								// 前のキーフレームと次のキーフレームの姿勢を補完
								DirectX::XMVECTOR Scale0 = DirectX::XMLoadFloat3(&key0.scaling);
								DirectX::XMVECTOR Scale1 = DirectX::XMLoadFloat3(&key1.scaling);
								DirectX::XMVECTOR Rotate0 = DirectX::XMLoadFloat4(&key0.rotation);
								DirectX::XMVECTOR Rotate1 = DirectX::XMLoadFloat4(&key1.rotation);
								DirectX::XMVECTOR Translate0 = DirectX::XMLoadFloat3(&key0.translation);
								DirectX::XMVECTOR Translate1 = DirectX::XMLoadFloat3(&key1.translation);

								DirectX::XMVECTOR Scale = DirectX::XMVectorLerp(Scale0, Scale1, rate);
								DirectX::XMVECTOR Rotate = DirectX::XMQuaternionSlerp(Rotate0, Rotate1, rate);
								DirectX::XMVECTOR Translate = DirectX::XMVectorLerp(Translate0, Translate1, rate);

								DirectX::XMStoreFloat3(&node.scaling, Scale);
								DirectX::XMStoreFloat4(&node.rotation, Rotate);
								DirectX::XMStoreFloat3(&node.translation, Translate);


							}

							if (node.name == rootname) {
								rootPow = key0.translation.y - key1.translation.y;
								node.translation.y = 0;
								node.translation.x = 0;
							}
							XMMATRIX S{ XMMatrixScaling(node.scaling.x, node.scaling.y, node.scaling.z) };
							XMMATRIX R{ XMMatrixRotationQuaternion(XMLoadFloat4(&node.rotation)) };
							XMMATRIX T{ XMMatrixTranslation(node.translation.x, node.translation.y, node.translation.z) };

							const int64_t parent_index{ resource->GetNodes().nodes.at(nodeIndex).parent_index };
							XMMATRIX P{ parent_index < 0 ? XMMatrixIdentity() : XMLoadFloat4x4(&keyframe.nodes.at(parent_index).globalTransform) };

							XMStoreFloat4x4(&node.globalTransform, S * R * T * P);
						}
					}
				}
				//仮ノードを戻す
				if (index) {
					keyframe.nodes[index] = tentative;
				}
				break;
			}
		}
	}
	// 最終フレーム処理
	if (animationSubEndFlag)
	{
		animationSubEndFlag = false;
		currentSubAnimationIndex = -1;
		currentSubAnimationSeconds = 0;
		return;
	}

	
	// 時間経過
	currentSubAnimationSeconds += elapsedTime;
	//キーフレーム番号計算
	int frame_index = static_cast<int>(currentSubAnimationSeconds * animation.samplingRate);
	// 再生時間が終端時間を超えたら
	if (frame_index > animation.sequence.size() - 1)
	{
		// ループフラグがtrueなら再生時間を巻き戻す
		if (animationSubLoopFlag) {
			currentSubAnimationSeconds = 0;
			frame_index = 0;
		}
		else {
			//currentAnimationSeconds = animation.secondsLength;
			subNode.clear();
			animationSubEndFlag = true;
			currentSubAnimationIndex = -1;
		}
	}
}

void Model::ClearSubAnimation() {
	subNode.clear();
	animationSubEndFlag = true;
	currentSubAnimationIndex = -1;
}

void Model::UpdateAnimation(float elapsedTime, const char* rootname, bool renderflag)
{
	// 再生中なら処理しない
	if (!IsPlayAnimation())return;

	// ブレンド率の計算

	float blendRate = 1.0f;
	if (animationBlendTime < animationBlendSeconds)
	{
		animationBlendTime += elapsedTime;
		if (animationBlendTime >= animationBlendSeconds)
		{
			animationBlendTime = animationBlendSeconds;
		}
		blendRate = animationBlendTime / animationBlendSeconds;
		blendRate *= blendRate;
	}

	// 指定のアニメーションデータを取得
	const std::vector<Animation>& animations = resource->GetAnimations();
	const Animation& animation = animations.at(currentAnimationIndex);
	//ModelKyeframe& Modelkeyframe = modelkyeframe.at(currentAnimationIndex);
	// アニメーションデータからキーフレームデータリストを取得
	const std::vector<Animation::Keyframe>& keyframes = animation.sequence;
	int keyCount = static_cast<int>(keyframes.size());
	if (renderflag) {
		for (int KeyIndex = 0; KeyIndex < keyCount - 1; ++KeyIndex)
		{
			//Animation::Keyframe& keyframe = Modelkeyframe.keyframes.at(KeyIndex);
			// 現在の時間がどのキーフレームの間にいるか判断する
			const Animation::Keyframe& keyframe0 = keyframes.at(KeyIndex);
			const Animation::Keyframe& keyframe1 = keyframes.at(KeyIndex + 1);

			if (currentAnimationSeconds >= keyframe0.seconds &&
				currentAnimationSeconds < keyframe1.seconds)
			{
				//再生時間とキーフレームの時間から補完率を算出する
				float rate = Mathf::Lerp(keyframe0.seconds, keyframe1.seconds, currentAnimationSeconds);
				int nodeCount = static_cast<int>(keyframe0.nodes.size());
				for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
				{

					// 2つのキーフレーム間の補完計算
					const Animation::Keyframe::node& key0 = keyframe0.nodes.at(nodeIndex);
					const Animation::Keyframe::node& key1 = keyframe1.nodes.at(nodeIndex);

					//key0.name = 
					Animation::Keyframe::node& node = keyframe.nodes[nodeIndex];
					//for (int i = 0; i < subNode.size(); i++) {
					//	if (subNode.at(i) == node.name) {
					//		SubAnimationFlag = true;
					//	}
					//}
					//if (SubAnimationFlag) {
					//	SubAnimationFlag = false;
					//	continue;
					//}
					// ブレンド補完処理
					if (blendRate < 1.0f)
					{
						// 現在の姿勢と次のキーフレームとの姿勢の補完
						DirectX::XMVECTOR Scale0 = DirectX::XMLoadFloat3(&node.scaling);
						DirectX::XMVECTOR Scale1 = DirectX::XMLoadFloat3(&key1.scaling);
						DirectX::XMVECTOR Rotate0 = DirectX::XMLoadFloat4(&node.rotation);
						DirectX::XMVECTOR Rotate1 = DirectX::XMLoadFloat4(&key1.rotation);
						DirectX::XMVECTOR Translate0 = DirectX::XMLoadFloat3(&node.translation);
						DirectX::XMVECTOR Translate1 = DirectX::XMLoadFloat3(&key1.translation);

						DirectX::XMVECTOR Scale = DirectX::XMVectorLerp(Scale0, Scale1, blendRate);
						DirectX::XMVECTOR Rotate = DirectX::XMQuaternionSlerp(Rotate0, Rotate1, blendRate);
						DirectX::XMVECTOR Translate = DirectX::XMVectorLerp(Translate0, Translate1, blendRate);

						DirectX::XMStoreFloat3(&node.scaling, Scale);
						DirectX::XMStoreFloat4(&node.rotation, Rotate);
						DirectX::XMStoreFloat3(&node.translation, Translate);
					}
					// 通常の計算
					else
					{
						// 前のキーフレームと次のキーフレームの姿勢を補完
						DirectX::XMVECTOR Scale0 = DirectX::XMLoadFloat3(&key0.scaling);
						DirectX::XMVECTOR Scale1 = DirectX::XMLoadFloat3(&key1.scaling);
						DirectX::XMVECTOR Rotate0 = DirectX::XMLoadFloat4(&key0.rotation);
						DirectX::XMVECTOR Rotate1 = DirectX::XMLoadFloat4(&key1.rotation);
						DirectX::XMVECTOR Translate0 = DirectX::XMLoadFloat3(&key0.translation);
						DirectX::XMVECTOR Translate1 = DirectX::XMLoadFloat3(&key1.translation);

						DirectX::XMVECTOR Scale = DirectX::XMVectorLerp(Scale0, Scale1, rate);
						DirectX::XMVECTOR Rotate = DirectX::XMQuaternionSlerp(Rotate0, Rotate1, rate);
						DirectX::XMVECTOR Translate = DirectX::XMVectorLerp(Translate0, Translate1, rate);

						DirectX::XMStoreFloat3(&node.scaling, Scale);
						DirectX::XMStoreFloat4(&node.rotation, Rotate);
						DirectX::XMStoreFloat3(&node.translation, Translate);


					}

					if (node.name == rootname) {
						rootPow = key1.translation.z - key0.translation.z;
						node.translation.z = 0;
						//node.translation.y = 0;
						node.translation.x = 0;
					}
					XMMATRIX S{ XMMatrixScaling(node.scaling.x, node.scaling.y, node.scaling.z) };
					XMMATRIX R{ XMMatrixRotationQuaternion(XMLoadFloat4(&node.rotation)) };
					XMMATRIX T{ XMMatrixTranslation(node.translation.x, node.translation.y, node.translation.z) };

					const int64_t parent_index{ resource->GetNodes().nodes.at(nodeIndex).parent_index };
					XMMATRIX P{ parent_index < 0 ? XMMatrixIdentity() : XMLoadFloat4x4(&keyframe.nodes.at(parent_index).globalTransform) };

					XMStoreFloat4x4(&node.globalTransform, S * R * T * P);
				}
				break;
			}
		}
	}
	// 最終フレーム処理
	if (animationEndFlag)
	{
		animationEndFlag = false;
		currentAnimationIndex = -1;
		currentAnimationSeconds = 0;
		return;
	}


	// 時間経過
	currentAnimationSeconds += elapsedTime;
	//キーフレーム番号計算
	int frame_index = static_cast<int>(currentAnimationSeconds * animation.samplingRate);
	// 再生時間が終端時間を超えたら
	if (frame_index > animation.sequence.size() - 1)
	{
		// ループフラグがtrueなら再生時間を巻き戻す
		if (animationLoopFlag) {
			currentAnimationSeconds = 0;
			frame_index = 0;
		}
		else {
			//currentAnimationSeconds = animation.secondsLength;
			animationEndFlag = true;
		}
	}
}

void Model::TrimmingAnimation(float elapsedTime)
{
	// 再生中なら処理しない
	if (!IsPlayAnimation())return;

	// ブレンド率の計算
	//float blendRate = 1.0f;
	//if (animationBlendTime < animationBlendSeconds)
	//{
	//	animationBlendTime += elapsedTime;
	//	if (animationBlendTime >= animationBlendSeconds)
	//	{
	//		animationBlendTime = animationBlendSeconds;
	//	}
	//	blendRate = animationBlendTime / animationBlendSeconds;
	//	blendRate *= blendRate;
	//}

	// 指定のアニメーションデータを取得
	const std::vector<Animation>& animations = resource->GetAnimations();
	const Animation& animation = animations.at(currentAnimationIndex);
	//ModelKyeframe& Modelkeyframe = modelkyeframe.at(currentAnimationIndex);
	int frame_index = static_cast<int>(currentAnimationSeconds * animation.samplingRate);
	// 再生時間が終端時間を超えたら
	if (frame_index > animation.sequence.size() - 2)
	{
		currentAnimationSeconds = 0;
		frame_index = 0;
	}
	// アニメーションデータからキーフレームデータリストを取得
	const std::vector<Animation::Keyframe>& keyframes = animation.sequence;
	int keyCount = static_cast<int>(keyframes.size());
	for (int KeyIndex = 0; KeyIndex < keyCount - 1; ++KeyIndex)
	{
		//Animation::Keyframe& keyframe = Modelkeyframe.keyframes.at(KeyIndex);
		// 現在の時間がどのキーフレームの間にいるか判断する
		const Animation::Keyframe& keyframe0 = keyframes.at(KeyIndex);
		const Animation::Keyframe& keyframe1 = keyframes.at(KeyIndex + 1);
		if (currentAnimationSeconds == 0) { currentAnimationSeconds += elapsedTime * 2; };
		if (currentAnimationSeconds >= keyframe0.seconds &&
			currentAnimationSeconds < keyframe1.seconds)
		{
			//再生時間とキーフレームの時間から補完率を算出する
			float rate = Mathf::Lerp(keyframe0.seconds, keyframe1.seconds, currentAnimationSeconds);
			int nodeCount = static_cast<int>(keyframe0.nodes.size());
			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				// 2つのキーフレーム間の補完計算
				const Animation::Keyframe::node& key0 = keyframe0.nodes.at(nodeIndex);
				const Animation::Keyframe::node& key1 = keyframe1.nodes.at(nodeIndex);

				Animation::Keyframe::node& node = keyframe.nodes[nodeIndex];

				
				
				{
					
					node.scaling = key0.scaling;
					node.rotation = key0.rotation;
					node.translation = key0.translation;
				}
			}
			break;
		}
	}

	


	// 時間経過
	//currentAnimationSeconds += elapsedTime;
	//キーフレーム番号計算
    frame_index = static_cast<int>(currentAnimationSeconds * animation.samplingRate);
	// 再生時間が終端時間を超えたら
	if (frame_index > animation.sequence.size() - 1)
	{
		// ループフラグがtrueなら再生時間を巻き戻す
		if (animationLoopFlag) {
			currentAnimationSeconds = 0;
			frame_index = 0;
		}
		else {
			//currentAnimationSeconds = animation.secondsLength;
			animationEndFlag = true;
		}
	}
}
//
//
//// アニメーション再生中か
bool Model::IsPlayAnimation() const
{
	if (currentAnimationIndex < 0)return false;
	if (currentAnimationIndex >= resource->GetAnimations().size())return false;
	return true;
}
bool Model::IsSubPlayAnimation() const
{
	if (currentSubAnimationIndex < 0)return false;
	if (currentSubAnimationIndex >= resource->GetAnimations().size())return false;
	return true;
}
bool Model::EndPlayAnimation() const
{
	
	return animationEndFlag;
}
//
Animation::Keyframe::node* Model::FindNode(const char* name)
{
	// 全てのノードの総当たりで名前比較する
	for (Animation::Keyframe::node& node : keyframe.nodes)
	{
		// 名前比較
		if (strcmp(node.name.c_str(), name) == 0)
		{
			return &node;
		}
	}
	// 見つからなかった
	return nullptr;
}

// シェーダー情報調整
void Model::ShaderAdjustment(float glitchScale, float timer, float maxHeight, DirectX::XMFLOAT3 hologramColor)
{
	for (const SkinnedMeshResouurce::Mesh& mesh : resource->GetMeshes())
	{
		for (const SkinnedMeshResouurce::Mesh::subset& subset : mesh.subsets)
		{
			// マテリアルの識別ID からマテリアルを取得し参照として設定
			const SkinnedMeshResouurce::Material& material{ resource->materials.find({subset.materialUniqueId}).operator*() };			
			SkinnedMeshResouurce::Material& mat = const_cast<SkinnedMeshResouurce::Material&>(material);
			mat.pbr.glitchScale = glitchScale;				// 振れ幅
			mat.pbr.timer = timer;							// 更新時間
			mat.pbr.hologramColor = hologramColor;			// ホログラム色
			mat.pbr.maxHeight = maxHeight;				// 最高点
		}
	}
}
