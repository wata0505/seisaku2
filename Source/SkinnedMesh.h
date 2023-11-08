#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <directxmath.h>
#include <vector>
#include <string>
#include <fbxsdk.h>
#include <unordered_map>
#include "StaticMesh.h"
#include <set>
#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/unordered_map.hpp>

//using namespace static_mesh;
namespace DirectX
{
    template<class T>
    void serialize(T& archive, XMFLOAT2& v)
    {
        archive(
            cereal::make_nvp("x", v.x),
            cereal::make_nvp("y", v.y)
        );
    }

    template<class T>
    void serialize(T& archive, XMFLOAT3& v)
    {
        archive(
            cereal::make_nvp("x", v.x),
            cereal::make_nvp("y", v.y),
            cereal::make_nvp("z", v.z)
        );
    }

    template<class T>
    void serialize(T& archive, XMFLOAT4& v)
    {
        archive(
            cereal::make_nvp("x", v.x),
            cereal::make_nvp("y", v.y),
            cereal::make_nvp("z", v.z),
            cereal::make_nvp("w", v.w)
        );
    }

    template<class T>
    void serialize(T& archive, XMFLOAT4X4& m)
    {
        archive(
            cereal::make_nvp("_11", m._11), cereal::make_nvp("_12", m._12),
            cereal::make_nvp("_13", m._13), cereal::make_nvp("_14", m._14),
            cereal::make_nvp("_21", m._21), cereal::make_nvp("_22", m._22),
            cereal::make_nvp("_23", m._23), cereal::make_nvp("_24", m._24),
            cereal::make_nvp("_31", m._31), cereal::make_nvp("_32", m._32),
            cereal::make_nvp("_33", m._33), cereal::make_nvp("_34", m._34),
            cereal::make_nvp("_41", m._41), cereal::make_nvp("_42", m._42),
            cereal::make_nvp("_43", m._43), cereal::make_nvp("_44", m._44)
            );
    }

}
struct Animation
{
    std::string name;
    float samplingRate{ 0 };

    struct Keyframe
    {
        float						seconds;
        struct node
        {
            std::string			name;
            // 'globalTransform' is used to convert from local space of node to global space of scene.
            DirectX::XMFLOAT4X4 globalTransform{ 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };

            //The transformation data of a node includes its translation, rotation and scaling vectors
            //with respect to its parent. 
            DirectX::XMFLOAT3 scaling{ 1,1,1 };
            DirectX::XMFLOAT4 rotation{ 0,0,0,1 }; //Rotation quaternion
            DirectX::XMFLOAT3 translation{ 0,0,0 };
            template<class T>
            void serialize(T& archive)
            {
                archive(name,globalTransform, scaling, rotation, translation);
            }

        };
        std::vector<node> nodes;

        template<class T>
        void serialize(T& archive)
        {
            archive(nodes,seconds);
        }
    };
    std::vector<Keyframe> sequence;
    template<class T>
    void serialize(T& archive)
    {
        archive(name, samplingRate, sequence);
    }
};
struct scene
{

    struct node
    {
        // ���� ID
        uint64_t uniqueId{ 0 };
        // ����
        std::string name;
        // ���
        FbxNodeAttribute::EType attribute{ FbxNodeAttribute::EType::eUnknown };
        // �e�m�[�h�̎��� ID
        int64_t parent_index{ -1 };
        template<class T>
        void serialize(T& archive)
        {
            archive(uniqueId, name, attribute, parent_index);
        }
    };
    std::vector<node> nodes;
    /// <summary>
    /// ��L�̃m�[�h���ɂ���w��̃��j�[�N ID �̃C���f�b�N�X�ԍ��𒲂ׂ�
    /// </summary>
    /// <param name="unique_id">�w��̃��j�[�N ID</param>
    /// <returns>�C���f�b�N�X�ԍ�</returns>
    int64_t indexof(uint64_t unique_id) const
    {
        int64_t index{ 0 };
        for (const node& node : nodes)
        {
            if (node.uniqueId == unique_id)
            {
                return index;
            }
            ++index;
        }
        return -1;
    }
    template<class T>
    void serialize(T& archive)
    {
        archive(nodes);
    }

};
/// <summary>
/// �X�L�����b�V��
/// </summary>
class SkinnedMeshResouurce
{
public:

    struct Skeleton
    {
        struct bone
        {
            uint64_t uniqueId{ 0 };
            std::string name;
            // 'parent_index'is index that refers to the parent bone's position in the array that contains itself.
            int64_t parentIndex{ -1 }; // -1 : the bone is orphan
            // 'node_index' is an index that refers to the node array of the scene.
            int64_t nodeIndex{ 0 };

            // 'offsetTransform' is used to convert from model(mesh) space to bone(node) scene.
            DirectX::XMFLOAT4X4 offsetTransform{ 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };

            bool is_orphan() const { return parentIndex < 0; };
            template<class T>
            void serialize(T& archive)
            {
                archive(uniqueId, name, parentIndex, nodeIndex, offsetTransform);
            }

        };
        std::vector<bone> bones;
        int64_t indexof(uint64_t unique_id)const
        {
            int64_t index{ 0 };
            for (const bone& bone : bones)
            {
                if (bone.uniqueId == unique_id)
                {
                    return index;
                }
                ++index;
            }
            return -1;
        }
        template<class T>
        void serialize(T& archive)
        {
            archive(bones);
        }

    };

   
    
    //std::vector<scene> scenes;
    static const int MAX_BONE_INFLUENCES{ 4 };
    // ���_�t�H�[�}�b�g
    struct Vertex
    {
        DirectX::XMFLOAT3 position;	// ���_���W
        DirectX::XMFLOAT3 normal;	// �@��
        DirectX::XMFLOAT4 tangent;
        DirectX::XMFLOAT2 texcoord;	// �e�N�X�`�����W
        float boneWeights[MAX_BONE_INFLUENCES]{ 1,0,0,0 };
        INT boneIndices[MAX_BONE_INFLUENCES]{};
        DirectX::XMFLOAT3 binormal;	// �]�@��
        template<class T>
        void serialize(T& archive)
        {
            archive(position, normal, tangent, texcoord,binormal, boneWeights, boneIndices);
        }
        
    };

    static const int MAX_BONES{ 256 };
    // �萔�o�b�t�@�t�H�[�}�b�g
    struct constants
    {
        DirectX::XMFLOAT4X4 world;			// ���[���h�s��
        DirectX::XMFLOAT4 materialColor;	// �}�e���A���J���[
        DirectX::XMFLOAT4X4 boneTransforms[MAX_BONES]{ {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1} };

    };

  

    /// <summary>
    /// ���b�V�����
    /// </summary>
     struct Mesh
      {
          uint64_t uniqueId{ 0 };        // ���� ID
          std::string name;               // ���b�V����
          int64_t nodeIndex{ 0 };        // �m�[�h ID
          std::vector<Vertex> vertices;   // ���_���W
          std::vector<uint32_t> indices;  // ���_�C���f�b�N�X
          DirectX::XMFLOAT4X4 defaultGlobalTransform{ 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
          Skeleton bindPose;
          DirectX::XMFLOAT3 boundingBox[2]
          {
              { +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX},
              { -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX}
          };
          
          /// <summary>
          /// �T�u�Z�b�g���
          /// </summary>
          struct subset
          {
              uint64_t materialUniqueId{ 0 };   // ���� ID
              std::string materialName;          // �}�e���A����
              uint32_t startIndexLocation{ 0 }; // �C���f�b�N�X�̊J�n�ʒu
              uint32_t indexCount{ 0 };          // �C���f�b�N�X�̐�(���_��)
              template<class T>
              void serialize(T& archive)
              {
                  archive(materialUniqueId, materialName, startIndexLocation, indexCount);
              }
             

             
          };
          std::vector<subset> subsets;
          template<class T>
          void serialize(T& archive)
          {
              archive(uniqueId, name, nodeIndex, subsets, defaultGlobalTransform,
                  bindPose, boundingBox, vertices, indices);
          }

      public:
           Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;     // ���_�o�b�t�@
           Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;      // �C���f�b�N�X�o�b�t�@
           friend class SkinnedMeshResouurce;
          
    };
     std::vector<Mesh> meshes;
  
   
     struct Material
     {
         uint64_t uniqueId{ 0 };
         std::string name;
     
         DirectX::XMFLOAT4 Ka{ 0.2f, 0.2f, 0.2f, 1.0f };
         DirectX::XMFLOAT4 Kd{ 0.8f, 0.8f, 0.8f, 1.0f };
         DirectX::XMFLOAT4 Ks{ 1.0f, 1.0f, 1.0f, 1.0f };
     
         std::string textureFilenames[5];
         bool operator<(const Material& rhs) const { return uniqueId < rhs.uniqueId; }
         template<class T>
         void serialize(T& archive)
         {
             archive(uniqueId, name, Ka, Kd, Ks, textureFilenames);
         }
     };
     std::set<Material> materials;
     std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> shaderResourceViews;

     //std::vector<material> materials;
     std::vector<Animation> AnimationClips;


private:
    // ���_�V�F�[�_�[
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    // �s�N�Z���V�F�[�_�[
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    // ���̓��C�A�E�g
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
    // �萔�o�b�t�@
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;

    
public:
    /// <summary>
    /// constructor
    /// </summary>
    /// <param name="device">�f�o�C�X</param>
    /// <param name="fbx_filename">fbx �t�@�C����</param>
    /// <param name="triangulate">true...���p�`�ō��ꂽ�|���S�����O�p�`��	false...�O�p�`�����Ȃ�</param>
    SkinnedMeshResouurce() {};
    virtual ~SkinnedMeshResouurce() {};
       /// <summary>
       /// �`�揈��
       /// </summary>
       /// <param name="immediate_context">�f�o�C�X�R���e�L�X�g</param>
       /// <param name="world">���[���h�s��</param>
       /// <param name="materialColor">�}�e���A���J���[</param>
   

    void Serialize(const char* filename);
    bool AppendAnimations(const char* Animation_filename, float samplingRate);
    /// <summary>
    /// �o�b�t�@�̐���
    /// </summary>
    /// <param name="device">�f�o�C�X</param>
    /// <param name="fbx_filename">fbx �t�@�C����</param>
    void CreateComObjects(ID3D11Device* device, const char* fbx_filename, const char* texfilename);

    void Load(ID3D11Device* device, const char* fbx_filename, float samplingRate, bool leftflag , bool triangulate = false);
    ID3D11VertexShader* GetVsShader() { return vertexShader.Get(); }
    // �s�N�Z���V�F�[�_�[
    ID3D11PixelShader* GetPixelShader() { return pixelShader.Get(); }
    // ���̓��C�A�E�g
    ID3D11InputLayout* GetInputLayout() { return inputLayout.Get(); }
    // �萔�o�b�t�@
    ID3D11Buffer* GetConstantBuffer() { return constantBuffer.Get(); }

    //ID3D11Buffer* GetAndconstant_buffer() { return constant_buffer.GetAddressOf(); }

    const std::vector<Mesh>& GetMeshes() const { return meshes; }
    const scene GetNodes() const { return scene_view; }
    const std::vector<Animation>& GetAnimations() const { return AnimationClips;}
    const std::set<Material>& GetMaterials() const { return materials; }

protected:
    // ���� fbx �̐e�V�[��
    scene scene_view;
    /// <summary>
    /// ���b�V�����̎��o��
    /// </summary>
    /// <param name="fbx_scene">���b�V�����̌��f�[�^������m�[�h�̏��������Ă���V�[��</param>
    /// <param name="meshes">���o����Ƀ��b�V������ݒ肷�邽�߂̏o�͗p���b�V��</param>
    void FetchMeshes(fbxsdk::FbxScene* fbx_scene, std::vector<Mesh>& meshes);

    void FetchMaterials(fbxsdk::FbxScene* fbx_scene, std::set<Material>& materials);

    void FetchSkeleton(FbxMesh* fbx_mesh, Skeleton& bindPose);

    void FetchAnimations(fbxsdk::FbxScene* fbx_scene, std::vector<Animation>& Animation_clips, float samplingRate);
private:
    bool serializeFlag = false;

    bool leftFlag = false;
private:
    // �ړ��l���E����W�n���獶����W�n�֕ϊ�����
    void ConvertTranslationFromRHtoLH(DirectX::XMFLOAT3& translate);
    // �ړ��l���E����W�n���獶����W�n�֕ϊ�����
    void ConvertTranslationFromRHtoLH(DirectX::XMFLOAT4& translate);

    // ��]�l���E����W�n���獶����W�n�֕ϊ�����
    void ConvertRotationFromRHtoLH(DirectX::XMFLOAT4& rotate);

    // �s��l���E����W�n���獶����W�n�֕ϊ�����
    void ConvertMatrixFromRHtoLH(DirectX::XMFLOAT4X4& matrix);

    // �C���f�b�N�X�o�b�t�@���E����W�n���獶����W�n�֕ϊ�����
    void ConvertIndexBufferFromRHtoLH(std::vector<UINT>& indices);
    

};