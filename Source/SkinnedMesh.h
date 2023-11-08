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
        // 識別 ID
        uint64_t uniqueId{ 0 };
        // 名商
        std::string name;
        // 種類
        FbxNodeAttribute::EType attribute{ FbxNodeAttribute::EType::eUnknown };
        // 親ノードの識別 ID
        int64_t parent_index{ -1 };
        template<class T>
        void serialize(T& archive)
        {
            archive(uniqueId, name, attribute, parent_index);
        }
    };
    std::vector<node> nodes;
    /// <summary>
    /// 上記のノード内にある指定のユニーク ID のインデックス番号を調べる
    /// </summary>
    /// <param name="unique_id">指定のユニーク ID</param>
    /// <returns>インデックス番号</returns>
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
/// スキンメッシュ
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
    // 頂点フォーマット
    struct Vertex
    {
        DirectX::XMFLOAT3 position;	// 頂点座標
        DirectX::XMFLOAT3 normal;	// 法線
        DirectX::XMFLOAT4 tangent;
        DirectX::XMFLOAT2 texcoord;	// テクスチャ座標
        float boneWeights[MAX_BONE_INFLUENCES]{ 1,0,0,0 };
        INT boneIndices[MAX_BONE_INFLUENCES]{};
        DirectX::XMFLOAT3 binormal;	// 従法線
        template<class T>
        void serialize(T& archive)
        {
            archive(position, normal, tangent, texcoord,binormal, boneWeights, boneIndices);
        }
        
    };

    static const int MAX_BONES{ 256 };
    // 定数バッファフォーマット
    struct constants
    {
        DirectX::XMFLOAT4X4 world;			// ワールド行列
        DirectX::XMFLOAT4 materialColor;	// マテリアルカラー
        DirectX::XMFLOAT4X4 boneTransforms[MAX_BONES]{ {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1} };

    };

  

    /// <summary>
    /// メッシュ情報
    /// </summary>
     struct Mesh
      {
          uint64_t uniqueId{ 0 };        // 識別 ID
          std::string name;               // メッシュ名
          int64_t nodeIndex{ 0 };        // ノード ID
          std::vector<Vertex> vertices;   // 頂点座標
          std::vector<uint32_t> indices;  // 頂点インデックス
          DirectX::XMFLOAT4X4 defaultGlobalTransform{ 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
          Skeleton bindPose;
          DirectX::XMFLOAT3 boundingBox[2]
          {
              { +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX},
              { -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX}
          };
          
          /// <summary>
          /// サブセット情報
          /// </summary>
          struct subset
          {
              uint64_t materialUniqueId{ 0 };   // 識別 ID
              std::string materialName;          // マテリアル名
              uint32_t startIndexLocation{ 0 }; // インデックスの開始位置
              uint32_t indexCount{ 0 };          // インデックスの数(頂点数)
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
           Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;     // 頂点バッファ
           Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;      // インデックスバッファ
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
    // 頂点シェーダー
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    // ピクセルシェーダー
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    // 入力レイアウト
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
    // 定数バッファ
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;

    
public:
    /// <summary>
    /// constructor
    /// </summary>
    /// <param name="device">デバイス</param>
    /// <param name="fbx_filename">fbx ファイル名</param>
    /// <param name="triangulate">true...多角形で作られたポリゴンを三角形化	false...三角形化しない</param>
    SkinnedMeshResouurce() {};
    virtual ~SkinnedMeshResouurce() {};
       /// <summary>
       /// 描画処理
       /// </summary>
       /// <param name="immediate_context">デバイスコンテキスト</param>
       /// <param name="world">ワールド行列</param>
       /// <param name="materialColor">マテリアルカラー</param>
   

    void Serialize(const char* filename);
    bool AppendAnimations(const char* Animation_filename, float samplingRate);
    /// <summary>
    /// バッファの生成
    /// </summary>
    /// <param name="device">デバイス</param>
    /// <param name="fbx_filename">fbx ファイル名</param>
    void CreateComObjects(ID3D11Device* device, const char* fbx_filename, const char* texfilename);

    void Load(ID3D11Device* device, const char* fbx_filename, float samplingRate, bool leftflag , bool triangulate = false);
    ID3D11VertexShader* GetVsShader() { return vertexShader.Get(); }
    // ピクセルシェーダー
    ID3D11PixelShader* GetPixelShader() { return pixelShader.Get(); }
    // 入力レイアウト
    ID3D11InputLayout* GetInputLayout() { return inputLayout.Get(); }
    // 定数バッファ
    ID3D11Buffer* GetConstantBuffer() { return constantBuffer.Get(); }

    //ID3D11Buffer* GetAndconstant_buffer() { return constant_buffer.GetAddressOf(); }

    const std::vector<Mesh>& GetMeshes() const { return meshes; }
    const scene GetNodes() const { return scene_view; }
    const std::vector<Animation>& GetAnimations() const { return AnimationClips;}
    const std::set<Material>& GetMaterials() const { return materials; }

protected:
    // この fbx の親シーン
    scene scene_view;
    /// <summary>
    /// メッシュ情報の取り出し
    /// </summary>
    /// <param name="fbx_scene">メッシュ情報の元データがあるノードの情報を持っているシーン</param>
    /// <param name="meshes">取り出し後にメッシュ情報を設定するための出力用メッシュ</param>
    void FetchMeshes(fbxsdk::FbxScene* fbx_scene, std::vector<Mesh>& meshes);

    void FetchMaterials(fbxsdk::FbxScene* fbx_scene, std::set<Material>& materials);

    void FetchSkeleton(FbxMesh* fbx_mesh, Skeleton& bindPose);

    void FetchAnimations(fbxsdk::FbxScene* fbx_scene, std::vector<Animation>& Animation_clips, float samplingRate);
private:
    bool serializeFlag = false;

    bool leftFlag = false;
private:
    // 移動値を右手座標系から左手座標系へ変換する
    void ConvertTranslationFromRHtoLH(DirectX::XMFLOAT3& translate);
    // 移動値を右手座標系から左手座標系へ変換する
    void ConvertTranslationFromRHtoLH(DirectX::XMFLOAT4& translate);

    // 回転値を右手座標系から左手座標系へ変換する
    void ConvertRotationFromRHtoLH(DirectX::XMFLOAT4& rotate);

    // 行列値を右手座標系から左手座標系へ変換する
    void ConvertMatrixFromRHtoLH(DirectX::XMFLOAT4X4& matrix);

    // インデックスバッファを右手座標系から左手座標系へ変換する
    void ConvertIndexBufferFromRHtoLH(std::vector<UINT>& indices);
    

};