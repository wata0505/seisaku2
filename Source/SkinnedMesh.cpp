#include "misc.h"
#include "SkinnedMesh.h"
#include <sstream>
#include <functional>
#include "Shader.h"
#include <filesystem>
#include "texture.h"
#include <fstream>

using namespace DirectX;


inline XMFLOAT4X4 to_xmfloat4x4(const FbxAMatrix& fbxamatrix)
{
    XMFLOAT4X4 xmfloat4x4;
    for (int row = 0; row < 4; row++)
    {
        for (int column = 0; column < 4; column++)
        {
            xmfloat4x4.m[row][column] = static_cast<float>(fbxamatrix[row][column]);
        }
    }
    return xmfloat4x4;
}
inline XMFLOAT3 to_xmfloat3(const FbxDouble3& fbxdouble3)
{
    XMFLOAT3 xmfloat3;
    xmfloat3.x = static_cast<float>(fbxdouble3[0]);
    xmfloat3.y = static_cast<float>(fbxdouble3[1]);
    xmfloat3.z = static_cast<float>(fbxdouble3[2]);
    return xmfloat3;
}
inline XMFLOAT4 to_xmfloat4(const FbxDouble4& fbxdouble4)
{
    XMFLOAT4 xmfloat4;
    xmfloat4.x = static_cast<float>(fbxdouble4[0]);
    xmfloat4.y = static_cast<float>(fbxdouble4[1]);
    xmfloat4.z = static_cast<float>(fbxdouble4[2]);
    xmfloat4.w = static_cast<float>(fbxdouble4[3]);
    return xmfloat4;
}
struct bone_influence
{
    uint32_t bone_index;
    float bone_weight;
};
using bone_influences_per_control_point = std::vector<bone_influence>;

/// <summary>
/// ボーン影響度を FBX データから取得
/// </summary>
/// <param name="fbx_mesh">ボーン影響度の情報を持っている取り出し元の FBX メッシュ</param>
/// <param name="bone_influences">取り出したボーン影響度</param>
void fetch_bone_influences(const FbxMesh* fbx_mesh, std::vector<bone_influences_per_control_point>& bone_influences)
{
    // ボーン影響度の数 = FBX メッシュにあるコントロールポイントの数に設定
    const int control_points_count{ fbx_mesh->GetControlPointsCount() };
    bone_influences.resize(control_points_count);

    // メッシュにあるスキンの数を取得
    const int skin_count{ fbx_mesh->GetDeformerCount(FbxDeformer::eSkin) };
    // メッシュにあるすべてのスキンの情報をチェック
    for (int skin_index = 0; skin_index < skin_count; ++skin_index)
    {
        // 現在のスキンを取得
        const FbxSkin* fbx_skin
        { static_cast<FbxSkin*>(fbx_mesh->GetDeformer(skin_index, FbxDeformer::eSkin)) };

        // スキンにあるクラスターの数を取得
        const int cluster_count{ fbx_skin->GetClusterCount() };
        // スキンにあるすべてのクラスターの情報をチェック
        for (int cluster_index = 0; cluster_index < cluster_count; ++cluster_index)
        {
            // 現在のクラスターの情報をチェック
            const FbxCluster* fbx_cluster{ fbx_skin->GetCluster(cluster_index) };

            // クラスターにあるコントロールポイントの数を取得
            const int control_point_indices_count{ fbx_cluster->GetControlPointIndicesCount() };
            // クラスターにあるすべてのコントロールポイントのウェイトの値を取得
            for (int control_point_indices_index = 0; control_point_indices_index < control_point_indices_count;
                ++control_point_indices_index)
            {
                // 現在のコントロールポイントの番号
                int control_point_index{ fbx_cluster->GetControlPointIndices()[control_point_indices_index] };
                // コントロールポイントのウェイト値
                double control_point_weight
                { fbx_cluster->GetControlPointWeights()[control_point_indices_index] };
                // 新規にボーン影響度を生成して追加
                bone_influence& bone_influence{ bone_influences.at(control_point_index).emplace_back() };
                // 現在のクラスターの番号をボーン番号として設定
                bone_influence.bone_index = static_cast<uint32_t>(cluster_index);
                // コントロールのウェイト値をボーンのウェイト値として設定
                bone_influence.bone_weight = static_cast<float>(control_point_weight);
            }
        }
    }
}

/// <summary>
/// constructor
/// </summary>
/// <param name="device">デバイス</param>
/// <param name="fbx_filename">fbx ファイル名</param>
/// <param name="triangulate">true...多角形で作られたポリゴンを三角形化	false...三角形化しない</param>
void SkinnedMeshResouurce::Load(ID3D11Device* device, const char* fbx_filename, float samplingRate, bool leftflag, bool triangulate)
{
    //左手系座標変換フラグ
    leftFlag = leftflag;
    std::filesystem::path cereal_filename(fbx_filename);
    cereal_filename.replace_extension("cereal");
    if (std::filesystem::exists(cereal_filename.c_str()))
    {
        std::ifstream ifs(cereal_filename.c_str(), std::ios::binary);
        cereal::BinaryInputArchive deserialization(ifs);
        deserialization(scene_view, meshes, materials, AnimationClips);
        serializeFlag = true;
    }
    else
    {

        // fbx マネージャーを生成
        FbxManager* fbx_manager{ FbxManager::Create() };

        // fbx シーンを生成
        FbxScene* fbx_scene{ FbxScene::Create(fbx_manager, "") };

        // fbx インポーターの生成
        FbxImporter* fbx_importer{ FbxImporter::Create(fbx_manager, "") };
        bool import_status{ false };

        // fbx ファイルを読み込む
        import_status = fbx_importer->Initialize(fbx_filename);
        _ASSERT_EXPR_A(import_status, fbx_importer->GetStatus().GetErrorString());

        // 読み込んだファイルの情報を fbx シーンに流し込む
        import_status = fbx_importer->Import(fbx_scene);
        _ASSERT_EXPR_A(import_status, fbx_importer->GetStatus().GetErrorString());

        FbxGeometryConverter fbx_converter(fbx_manager);
        if (triangulate)
        {
            // 多角形で作られたポリゴンをすべて三角形にする
            fbx_converter.Triangulate(fbx_scene, true, false);
            fbx_converter.RemoveBadPolygonsFromMeshes(fbx_scene);
        }
        FbxAxisSystem fbx_scene_axis_system = fbx_scene->GetGlobalSettings().GetAxisSystem();
        if (fbx_scene_axis_system == FbxAxisSystem::eMayaZUp)
        {
            FbxAxisSystem::MayaYUp.ConvertScene(fbx_scene);
        }

        // ノードの情報を解析する関数
        std::function<void(FbxNode*)> traverse{
            [&](FbxNode* fbx_node) {
                // scene_view.nodes に新しくノードを取り付け、取り付けたノードをローカルの node に代入し値を設定していく
                scene::node& node{scene_view.nodes.emplace_back()};
                node.attribute = fbx_node->GetNodeAttribute() ? fbx_node->GetNodeAttribute()->GetAttributeType() : FbxNodeAttribute::EType::eUnknown;
                node.name = fbx_node->GetName();
                node.uniqueId = fbx_node->GetUniqueID();
                node.parent_index = scene_view.indexof(fbx_node->GetParent() ? fbx_node->GetParent()->GetUniqueID() : 0);
                // 現在解析しているノードに取り付けられているの子ノードを解析していく
                for (int child_index = 0; child_index < fbx_node->GetChildCount(); ++child_index)
                {
                    traverse(fbx_node->GetChild(child_index));
                }
            }
        };

        // ルートノードから解析開始
        traverse(fbx_scene->GetRootNode());

#if 0
        // シーンに取り付けたすべてのノードの情報を表示
        for (const scene::node& node : scene_view.nodes)
        {
            // ノード名でノードを検索し取得
            FbxNode* fbx_node{ fbx_scene->FindNodeByName(node.name.c_str()) };
            std::string node_name = fbx_node->GetName();
            uint64_t uid = fbx_node->GetUniqueID();
            uint64_t parent_uid = fbx_node->GetParent() ? fbx_node->GetParent()->GetUniqueID() : 0;
            int32_t type = fbx_node->GetNodeAttribute() ? fbx_node->GetNodeAttribute()->GetAttributeType() : 0;

            std::stringstream debug_string;
            debug_string << node_name << ":" << uid << ":" << parent_uid << ":" << type << "\n";
            OutputDebugStringA(debug_string.str().c_str());
        }
#endif


        FetchAnimations(fbx_scene, AnimationClips, samplingRate);

        FetchMeshes(fbx_scene, meshes);

        FetchMaterials(fbx_scene, materials);
       
        fbx_manager->Destroy();

        //if (Animation_filename != "") {
        //    AppendAnimations(Animation_filename, samplingRate);
        //}
        
        //std::ofstream ofs(cereal_filename.c_str(),std::ios::binary);
        //cereal::BinaryOutputArchive serializetion(ofs);
        //serializetion(scene_view, meshes, materials, Animation_clips);

    }

    //CreateComObjects(device, fbx_filename);

}
/// <summary>
/// メッシュ情報の取り出し
/// </summary>
/// <param name="fbx_scene">メッシュ情報の元データがあるノードの情報を持っているシーン</param>
/// <param name="meshes">取り出し後にメッシュ情報を設定するための出力用メッシュ</param>
void SkinnedMeshResouurce::FetchMeshes(FbxScene* fbx_scene, std::vector<Mesh>& meshes)
{
    //float node_index = 0;
    //for (const scene::node& node : scene_view.nodes)
    //{
    const size_t node_count{ scene_view.nodes.size() };
    for (size_t node_index = 0; node_index < node_count; ++node_index)
    {
        const scene::node& node{ scene_view.nodes.at(node_index) };
        // シーンの中にあるノードのからメッシュの属性を持っているノードをチェック
        if (node.attribute != FbxNodeAttribute::EType::eMesh)
        {
            continue;
        }
        
        FbxNode* fbx_node{ fbx_scene->GetNode(node_index) };
        FbxMesh* fbx_mesh{ fbx_node->GetMesh() };               // メッシュ情報を取得

        Mesh& mesh{ meshes.emplace_back() };                    // メッシュに取り付け
        mesh.uniqueId = fbx_mesh->GetNode()->GetUniqueID();
        mesh.name = fbx_mesh->GetNode()->GetName();
        mesh.nodeIndex = scene_view.indexof(mesh.uniqueId);   // このメッシュに対するノード ID の割り振り
        mesh.defaultGlobalTransform = to_xmfloat4x4(fbx_mesh->GetNode()->EvaluateGlobalTransform());

        for (const Vertex& v : mesh.vertices)
        {
            mesh.boundingBox[0].x = std::min<float>(mesh.boundingBox[0].x, v.position.x);
            mesh.boundingBox[0].y = std::min<float>(mesh.boundingBox[0].y, v.position.y);
            mesh.boundingBox[0].z = std::min<float>(mesh.boundingBox[0].z, v.position.z);
            mesh.boundingBox[1].x = std::max<float>(mesh.boundingBox[1].x, v.position.x);
            mesh.boundingBox[1].y = std::max<float>(mesh.boundingBox[1].y, v.position.y);
            mesh.boundingBox[1].z = std::max<float>(mesh.boundingBox[1].z, v.position.z);

        }

        std::vector<bone_influences_per_control_point> bone_influences;
        fetch_bone_influences(fbx_mesh, bone_influences);
        FetchSkeleton(fbx_mesh, mesh.bindPose);

        std::vector<Mesh::subset>& subsets{ mesh.subsets };
        

        const int material_count{ fbx_mesh->GetNode()->GetMaterialCount() };
        subsets.resize(material_count > 0 ? material_count : 1);
        for (int material_index = 0; material_index < material_count; ++material_index)
            
        {
            
            const FbxSurfaceMaterial* fbx_material{ fbx_mesh->GetNode()->GetMaterial(material_index) };
            subsets.at(material_index).materialName = fbx_material->GetName();
            subsets.at(material_index).materialUniqueId = fbx_material->GetUniqueID();
            
        }
        if (material_count > 0)
        {
            const int polygo_count{ fbx_mesh->GetPolygonCount() };
            for (int polygon_index = 0; polygon_index < polygo_count; ++polygon_index)
            {
                const int material_index{ fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(polygon_index) };
                subsets.at(material_index).indexCount += 3;
            }
            uint32_t offset{ 0 };
            for (Mesh::subset& subset : subsets)
            {
                subset.startIndexLocation = offset;
                offset += subset.indexCount;
                // This will be used as counter in the following procedures, reset to zero
                subset.indexCount = 0;
            }
        }

        const int polygon_count{ fbx_mesh->GetPolygonCount() };
        mesh.vertices.resize(polygon_count * 3LL);
        mesh.indices.resize(polygon_count * 3LL);

        FbxStringList uv_names;
        fbx_mesh->GetUVSetNames(uv_names);
        const FbxVector4* control_points{ fbx_mesh->GetControlPoints() };
        for (int polygon_index = 0; polygon_index < polygon_count; ++polygon_index)
        {
            const int material_index{ material_count > 0 ?
            fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(polygon_index) : 0 };
            Mesh::subset& subset{ subsets.at(material_index) };
            const uint32_t offset{ subset.startIndexLocation + subset.indexCount };
            for (int position_in_polygon = 0; position_in_polygon < 3; ++position_in_polygon)
            {
                const int vertex_index{ polygon_index * 3 + position_in_polygon };

                // 頂点座標の取得
                Vertex vertex;
                const int polygon_vertex{ fbx_mesh->GetPolygonVertex(polygon_index, position_in_polygon) };
                vertex.position.x = static_cast<float>(control_points[polygon_vertex][0]);
                vertex.position.y = static_cast<float>(control_points[polygon_vertex][1]);
                vertex.position.z = static_cast<float>(control_points[polygon_vertex][2]);

                const bone_influences_per_control_point& influences_per_control_point{ bone_influences.at(polygon_vertex) };
                for (size_t influences_index = 0; influences_index < influences_per_control_point.size();
                    ++influences_index)
                {
                    if (influences_index < MAX_BONE_INFLUENCES)
                    {
                        vertex.boneWeights[influences_index] = influences_per_control_point.at(influences_index).bone_weight;
                        vertex.boneIndices[influences_index] = influences_per_control_point.at(influences_index).bone_index;
                    }
                }
                // 法線の取得
                if (fbx_mesh->GetElementNormalCount() > 0)
                {
                    FbxVector4 normal;
                    fbx_mesh->GetPolygonVertexNormal(polygon_index, position_in_polygon, normal);
                    vertex.normal.x = static_cast<float>(normal[0]);
                    vertex.normal.y = static_cast<float>(normal[1]);
                    vertex.normal.z = static_cast<float>(normal[2]);
                }
                //ConvertTranslationFromRHtoLH(vertex.position);
                //ConvertTranslationFromRHtoLH(vertex.normal);
                // テクスチャ座標の取得
                if (fbx_mesh->GetElementUVCount() > 0)
                {
                    FbxVector2 uv;
                    bool unmapped_uv;
                    fbx_mesh->GetPolygonVertexUV(polygon_index, position_in_polygon,
                        uv_names[0], uv, unmapped_uv);
                    vertex.texcoord.x = static_cast<float>(uv[0]);
                    vertex.texcoord.y = 1.0f - static_cast<float>(uv[1]);
                }
                if (fbx_mesh->GenerateTangentsData(0, false))
                {
                    const FbxGeometryElementTangent* tangent = fbx_mesh->GetElementTangent(0);
                    vertex.tangent.x = static_cast<float>(tangent->GetDirectArray().GetAt(vertex_index)[0]);
                    vertex.tangent.y = static_cast<float>(tangent->GetDirectArray().GetAt(vertex_index)[1]);
                    vertex.tangent.z = static_cast<float>(tangent->GetDirectArray().GetAt(vertex_index)[2]);
                    vertex.tangent.w = static_cast<float>(tangent->GetDirectArray().GetAt(vertex_index)[3]);
                    //ConvertTranslationFromRHtoLH(vertex.tangent);
                }
                if (fbx_mesh->GetElementBinormalCount() > 0) {
                    FbxLayerElementArrayTemplate<FbxVector4>* binormals;
                    fbx_mesh->GetBinormals(&binormals);
                    vertex.binormal.x = static_cast<float>(binormals->GetAt(vertex_index)[0]);
                    vertex.binormal.y = static_cast<float>(binormals->GetAt(vertex_index)[1]);
                    vertex.binormal.z = static_cast<float>(binormals->GetAt(vertex_index)[2]);
                    // 座標系変換
                    //ConvertTranslationFromRHtoLH(vertex.binormal);
                
                }
                if (leftFlag) {
                    ConvertTranslationFromRHtoLH(vertex.position);
                    ConvertTranslationFromRHtoLH(vertex.normal);
                    ConvertTranslationFromRHtoLH(vertex.tangent);
                    ConvertTranslationFromRHtoLH(vertex.binormal);
                }
                mesh.vertices.at(vertex_index) = std::move(vertex);
                //mesh.indices.at(vertex_index) = vertex_index;
                mesh.indices.at(static_cast<size_t>(offset) + position_in_polygon) = vertex_index;
                subset.indexCount++;
            }
        }
        // 座標系変換
        if (leftFlag) {
           // ConvertIndexBufferFromRHtoLH(mesh.indices);
        }
        //node_index++;
    }
}

/// <summary>
/// バッファの生成
/// </summary>
/// <param name="device">デバイス</param>
/// <param name="fbx_filename">fbx ファイル名</param>
void SkinnedMeshResouurce::CreateComObjects(ID3D11Device* device, const char* fbx_filename, const char* texfilename)
{
    char drive[32], dir[256], dirname[256];
    ::_splitpath_s(fbx_filename, drive, sizeof(drive), dir, sizeof(dir), nullptr, 0, nullptr, 0);
    ::_makepath_s(dirname, sizeof(dirname), drive, dir, nullptr, nullptr);
    if (texfilename) {
        ::_splitpath_s(texfilename, nullptr, 0, dir, sizeof(dir), nullptr, 0, nullptr, 0);
    }
    for (Mesh& mesh : meshes)
    {
        HRESULT hr{ S_OK };
        D3D11_BUFFER_DESC buffer_desc{};
        D3D11_SUBRESOURCE_DATA subresource_data{};
        buffer_desc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * mesh.vertices.size());
        buffer_desc.Usage = D3D11_USAGE_DEFAULT;
        buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        buffer_desc.CPUAccessFlags = 0;
        buffer_desc.MiscFlags = 0;
        buffer_desc.StructureByteStride = 0;
        subresource_data.pSysMem = mesh.vertices.data();
        subresource_data.SysMemPitch = 0;
        subresource_data.SysMemSlicePitch = 0;
        hr = device->CreateBuffer(&buffer_desc, &subresource_data,
            mesh.vertexBuffer.ReleaseAndGetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

        buffer_desc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * mesh.indices.size());
        buffer_desc.Usage = D3D11_USAGE_DEFAULT;
        buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        subresource_data.pSysMem = mesh.indices.data();
        hr = device->CreateBuffer(&buffer_desc, &subresource_data,
            mesh.indexBuffer.ReleaseAndGetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
#if 1
       // mesh.vertices.clear();
       // mesh.indices.clear();
#endif
    }

    HRESULT hr = S_OK;
    D3D11_INPUT_ELEMENT_DESC input_element_desc[]
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
       { "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BINORMAL", 0,DXGI_FORMAT_R32G32B32A32_UINT, 0,
             D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        { "WEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        { "BONES", 0,DXGI_FORMAT_R32G32B32A32_UINT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        
    };
    //create_vs_from_cso(device, "Shader\\SkinnedMeshResouurce_vs.cso", vertex_shader.ReleaseAndGetAddressOf(),
    //    input_layout.ReleaseAndGetAddressOf(), input_element_desc, ARRAYSIZE(input_element_desc));
    //create_ps_from_cso(device, "Shader\\SkinnedMeshResouurce_ps.cso", pixel_shader.ReleaseAndGetAddressOf());
    

    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth = sizeof(constants);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = device->CreateBuffer(&buffer_desc, nullptr, constantBuffer.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

    for (const Material& material : materials)    
     {
        for (size_t texture_index = 0; texture_index < 5; ++texture_index)
        {
            Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view;
            D3D11_TEXTURE2D_DESC texture2d_desc;
            if (!texfilename && material.textureFilenames[texture_index].size() > 0)
            {
                std::filesystem::path path(fbx_filename);
                path.replace_filename(material.textureFilenames[texture_index]);
                D3D11_TEXTURE2D_DESC texture2d_desc;

                if(material.textureFilenames[texture_index] == "dummy_normal_map" || material.textureFilenames[texture_index] == "dummy_diffuse_map")
                {
                    MakeDummyTexture(device, shader_resource_view.GetAddressOf(), texture_index == 1 ? 0xFFFF7F7F : 0xFFFFFFFF, 16);
                    break;
                }
                LoadTextureFromFile(device, path.c_str(),
                    shader_resource_view.GetAddressOf(), &texture2d_desc);
                // 相対パスの解決
                //char filename[256];
                //::_makepath_s(filename, 256, nullptr, dirname, material.textureFilenames[texture_index].c_str(), nullptr);
              
                
                // テクスチャ読み込み
                //load_texture_from_file(device, filename,
                //        shader_resource_view.GetAddressOf(), &texture2d_desc);
            }
            else if (texfilename && texture_index < 5) {
                bool dummyflag = false;
                if (texture_index == 0) {
                    std::filesystem::path path(dirname);
                    path.replace_filename(texfilename);  
                    //char filename[256];
                    //::_makepath_s(filename, 256, nullptr, dirname, texfilename, nullptr);
                    // テクスチャ読み込み

                    LoadTextureFromFile(device, path.c_str(),
                        shader_resource_view.GetAddressOf(), &texture2d_desc);
                    const_cast<std::string*>(&material.textureFilenames[texture_index])->assign
                    ("Albedo");
                }
                if (texture_index == 1)
                {
                    std::filesystem::path path(dirname);
                    const char* searchName = "Normal";
                    path.replace_filename(dir);
                    path.replace_filename(searchName);
                    // テクスチャ読み込み
                    std::filesystem::path dds_filename(path);
                    dds_filename.replace_extension("dds");
                    if (std::filesystem::exists(dds_filename.c_str())) {
                        LoadTextureFromFile(device, path.c_str(),
                            shader_resource_view.GetAddressOf(), &texture2d_desc);
                    }
                    else {
                        dummyflag = true;
                    }
                }
                if (texture_index == 2)
                {
                    std::filesystem::path path(dirname);
                    const char* searchName = "MetalSmoothness";
                    path.replace_filename(dir);
                    path.replace_filename(searchName);
                    // テクスチャ読み込み
                    std::filesystem::path dds_filename(path);
                    dds_filename.replace_extension("dds");
                    if (std::filesystem::exists(dds_filename.c_str())) {
                        LoadTextureFromFile(device, path.c_str(),
                            shader_resource_view.GetAddressOf(), &texture2d_desc);
                        const_cast<std::string*>(&material.textureFilenames[texture_index])->assign
                        ("MetalSmoothness");
                    }
                    else {
                        dummyflag = true;
                    }
                }
                if (texture_index == 3)
                {
                    std::filesystem::path path(dirname);
                    const char* searchName = "AO";
                    path.replace_filename(dir);
                    path.replace_filename(searchName);
                    // テクスチャ読み込み
                    std::filesystem::path dds_filename(path);
                    dds_filename.replace_extension("dds");
                    if (std::filesystem::exists(dds_filename.c_str())) {
                        LoadTextureFromFile(device, path.c_str(),
                            shader_resource_view.GetAddressOf(), &texture2d_desc);
                        const_cast<std::string*>(&material.textureFilenames[texture_index])->assign
                        ("AO");
                    }
                    else {
                        dummyflag = true;
                    }
                }
                if (texture_index == 4)
                {
                    std::filesystem::path path(dirname);
                    const char* searchName = "Emission";
                    path.replace_filename(dir);
                    path.replace_filename(searchName);
                    // テクスチャ読み込み
                    std::filesystem::path dds_filename(path);
                    dds_filename.replace_extension("dds");
                    if (std::filesystem::exists(dds_filename.c_str())) {
                        LoadTextureFromFile(device, path.c_str(),
                            shader_resource_view.GetAddressOf(), &texture2d_desc);
                        const_cast<std::string*>(&material.textureFilenames[texture_index])->assign
                        ("Emission");
                    }
                    else {
                        dummyflag = true;
                    }
                }
                if (dummyflag) 
                {
                    const_cast<std::string*>(&material.textureFilenames[texture_index])->assign
                    (texture_index == 1 ? "dummy_normal_map" : "dummy_diffuse_map");
                    MakeDummyTexture(device, shader_resource_view.GetAddressOf(), texture_index == 1 ? 0xFFFF7F7F : 0x00000000, 16);
                }
            }
            else
            {
                const_cast<std::string*>(&material.textureFilenames[texture_index])->assign
                (texture_index == 1 ? "dummy_normal_map" : "dummy_diffuse_map");
                MakeDummyTexture(device, shader_resource_view.GetAddressOf(),texture_index == 1 ? 0xFFFF7F7F : 0x00000000, 16);
            }
            shaderResourceViews.insert(std::make_pair(material.textureFilenames[texture_index], shader_resource_view));
        }
     }


}





void SkinnedMeshResouurce::FetchMaterials(fbxsdk::FbxScene* fbx_scene, std::set<Material>& materials)
{
    const size_t node_count{ scene_view.nodes.size() };
    for (size_t node_index = 0; node_index < node_count; ++node_index)
    {
        // ノードの情報１つ１つをチェック
        const scene::node& node{ scene_view.nodes.at(node_index) };
        // シーンから FbxNode を取得
        const FbxNode* fbx_node{ fbx_scene->GetNode(node_index) };

        // ノードの中にあるマテリアルの情報をすべて検索
        const int material_count{ fbx_node->GetMaterialCount() };
        for (int material_index = 0; material_index < material_count; ++material_index)
        {
            // マテリアルの情報１つ１つをチェック
            const FbxSurfaceMaterial* fbx_material{ fbx_node->GetMaterial(material_index) };

            Material material;
            // マテリアル名の設定
            material.name = fbx_material->GetName();
            // マテリアルの識別 ID の設定
            material.uniqueId = fbx_material->GetUniqueID();

            
            

            FbxProperty fbx_property;
            // ディフューズ(拡散反射光)の情報を取得
            fbx_property = fbx_material->FindProperty(FbxSurfaceMaterial::sDiffuse);
            // ディフューズがあったらディフューズの情報を設定していく
            if (fbx_property.IsValid())
            {
                const FbxDouble3 color{ fbx_property.Get<FbxDouble3>() };
                material.Kd.x = static_cast<float>(color[0]);
                material.Kd.y = static_cast<float>(color[1]);
                material.Kd.z = static_cast<float>(color[2]);
                material.Kd.w = 1.0f;

                // テクスチャのファイル名を取得
                const FbxFileTexture* fbx_texture{ fbx_property.GetSrcObject<FbxFileTexture>() };
                // 相対パス込みでのファイル名を設定する
                material.textureFilenames[0] =
                    fbx_texture ? fbx_texture->GetRelativeFileName() : "";
            }

            // アンビエント(環境光)の情報を取得
            fbx_property = fbx_material->FindProperty(FbxSurfaceMaterial::sAmbient);
            // ディフューズがあったらディフューズの情報を設定していく
            if (fbx_property.IsValid())
            {
                const FbxDouble3 color{ fbx_property.Get<FbxDouble3>() };
                material.Ka.x = static_cast<float>(color[0]);
                material.Ka.y = static_cast<float>(color[1]);
                material.Ka.z = static_cast<float>(color[2]);
                material.Ka.w = 1.0f;
            }
            // スペキュラ(鏡面反射光)の情報を取得
            fbx_property = fbx_material->FindProperty(FbxSurfaceMaterial::sSpecular);
            // ディフューズがあったらディフューズの情報を設定していく
            if (fbx_property.IsValid())
            {
                const FbxDouble3 color{ fbx_property.Get<FbxDouble3>() };
                material.Ks.x = static_cast<float>(color[0]);
                material.Ks.y = static_cast<float>(color[1]);
                material.Ks.z = static_cast<float>(color[2]);
                material.Ks.w = 1.0f;
            }
            fbx_property = fbx_material->FindProperty(FbxSurfaceMaterial::sNormalMap);
            if (fbx_property.IsValid())
            {
                const FbxFileTexture* file_texture{ fbx_property.GetSrcObject<FbxFileTexture>() };
                material.textureFilenames[1] = file_texture ? file_texture->GetRelativeFileName(): "";
            }

            // 取得したマテリアルの情報を設定する
            materials.insert(std::move(material));
        }
    }
#if 1
    // ダミーのマテリアルを挿入
    Material material;
    materials.insert(material);
#endif
}

void SkinnedMeshResouurce::FetchSkeleton(FbxMesh* fbx_mesh, Skeleton& bindPose)
{
    // メッシュにあるスキンの数を取得
    const int deformer_count = fbx_mesh->GetDeformerCount(FbxDeformer::eSkin);
    // メッシュにあるすべてのスキンの情報をチェック
    for (int deformer_index = 0; deformer_index < deformer_count; ++deformer_index)
    {
        // 現在のスキンを取得
        FbxSkin* skin = static_cast<FbxSkin*>(fbx_mesh->GetDeformer(deformer_index, FbxDeformer::eSkin));
        // スキンにあるクラスターの数を取得
        const int cluster_count = skin->GetClusterCount();
        // メッシュのボーンの数 = メッシュのクラスターの数
        bindPose.bones.resize(cluster_count);
        // スキンにあるすべてのクラスターの情報をチェック
        for (int cluster_index = 0; cluster_index < cluster_count; ++cluster_index)
        {
            // 現在のクラスターの情報をチェック
            FbxCluster* cluster = skin->GetCluster(cluster_index);

            Skeleton::bone& bone{ bindPose.bones.at(cluster_index) };
            bone.name = cluster->GetLink()->GetName();
            bone.uniqueId = cluster->GetLink()->GetUniqueID();
            bone.parentIndex = bindPose.indexof(cluster->GetLink()->GetParent()->GetUniqueID());
            bone.nodeIndex = scene_view.indexof(bone.uniqueId);

            // リンクを含むノードに関連付けられた行列を取得
            // モデル(メッシュ)のローカル空間からシーンのグローバル空間に変換するために使用
            FbxAMatrix reference_global_init_position;
            cluster->GetTransformMatrix(reference_global_init_position);

            // リンクノードに関連付けられた行列を取得
            // ボーンのローカル空間からシーンのグローバル空間に変換するために使用
            // ローカル座標空間でのボーンの絶対位置
            FbxAMatrix cluster_global_init_position;
            cluster->GetTransformLinkMatrix(cluster_global_init_position);

            bone.offsetTransform =
                to_xmfloat4x4(cluster_global_init_position.Inverse() * reference_global_init_position);

            ConvertMatrixFromRHtoLH(bone.offsetTransform);
        }
    }
}

void SkinnedMeshResouurce::FetchAnimations(fbxsdk::FbxScene* fbx_scene, std::vector<Animation>& Animation_clips, float samplingRate /*If this value is 0, the Animation data will be sampled at the default frame rate.*/)
{
    FbxArray<FbxString*> Animation_stack_names;
    fbx_scene->FillAnimStackNameArray(Animation_stack_names);
    const int Animation_stack_count{ Animation_stack_names.GetCount() };
    for (int Animation_stack_index = 0; Animation_stack_index < Animation_stack_count; ++Animation_stack_index)
    {
        Animation& Animation_clip{ Animation_clips.emplace_back() };
        Animation_clip.name = Animation_stack_names[Animation_stack_index]->Buffer();

        FbxAnimStack* Animation_stack{ fbx_scene->FindMember<FbxAnimStack>(Animation_clip.name.c_str()) };
        fbx_scene->SetCurrentAnimationStack(Animation_stack);

        const FbxTime::EMode time_mode{ fbx_scene->GetGlobalSettings().GetTimeMode() };
        FbxTime one_second;
        one_second.SetTime(0, 0, 1, 0, 0, time_mode);
        float samplingRate = static_cast<float>(one_second.GetFrameRate(time_mode));
        float samplingTime = 1.0f / samplingRate;
        Animation_clip.samplingRate = samplingRate > 0 ?
            samplingRate : static_cast<float>(one_second.GetFrameRate(time_mode));
        const FbxTime sampling_interval
        { static_cast<FbxLongLong>(one_second.Get() / Animation_clip.samplingRate) };
        const FbxTakeInfo* take_info{ fbx_scene->GetTakeInfo(Animation_clip.name.c_str()) };
        const FbxTime start_time{ take_info->mLocalTimeSpan.GetStart() };
        const FbxTime stop_time{ take_info->mLocalTimeSpan.GetStop() };
        float seconds = 0.0f;
        for (FbxTime time = start_time; time < stop_time; time += sampling_interval)
        {
            Animation::Keyframe& keyframe{ Animation_clip.sequence.emplace_back() };
            keyframe.seconds = seconds;
            const size_t node_const{ scene_view.nodes.size() };
            keyframe.nodes.resize(node_const);
            for (size_t node_index = 0; node_index < node_const; ++node_index)
            {
                FbxNode* fbx_node{ fbx_scene->FindNodeByName(scene_view.nodes.at(node_index).name.c_str()) };
                if (fbx_node)
                {
                    Animation::Keyframe::node& node{ keyframe.nodes.at(node_index) };
                    node.name = fbx_node->GetName();
                    // 'globalTransform' is a transformation matrix of a node with respect to
                    //  the scene's global coordinate system.
                    node.globalTransform = to_xmfloat4x4(fbx_node->EvaluateGlobalTransform(time));
                    // 'local_transform' is a transformation matrix of a node with respect to
                    // its parent's local coordinate system.
                    const FbxAMatrix& local_transform{ fbx_node->EvaluateLocalTransform(time) };
                    node.scaling = to_xmfloat3(local_transform.GetS());
                    node.rotation = to_xmfloat4(local_transform.GetQ());
                    node.translation = to_xmfloat3(local_transform.GetT());
                    if (leftFlag) {
                        ConvertTranslationFromRHtoLH(node.translation);
                        ConvertRotationFromRHtoLH(node.rotation);
                    }
                }
            }
            seconds += samplingTime;
        }
    }
    for (int Animation_stack_index = 0; Animation_stack_index < Animation_stack_count; ++Animation_stack_index)
    {
        delete Animation_stack_names[Animation_stack_index];
    }
}


bool SkinnedMeshResouurce::AppendAnimations(const char* Animation_filename, float samplingRate)
{
    if (serializeFlag == false) {
        FbxManager* fbx_manager{ FbxManager::Create() };
        FbxScene* fbx_scene{ FbxScene::Create(fbx_manager,"") };

        FbxImporter* fbx_importer{ FbxImporter::Create(fbx_manager,"") };
        bool import_status{ false };
        import_status = fbx_importer->Initialize(Animation_filename);
        _ASSERT_EXPR_A(import_status, fbx_importer->GetStatus().GetErrorString());
        import_status = fbx_importer->Import(fbx_scene);
        _ASSERT_EXPR_A(import_status, fbx_importer->GetStatus().GetErrorString());

        FbxAxisSystem fbx_scene_axis_system = fbx_scene->GetGlobalSettings().GetAxisSystem();
        if (fbx_scene_axis_system == FbxAxisSystem::eMayaZUp)
        {
            FbxAxisSystem::MayaYUp.ConvertScene(fbx_scene);
        }

        FetchAnimations(fbx_scene, AnimationClips, samplingRate);

        fbx_manager->Destroy();
    }
    return true;
}
void  SkinnedMeshResouurce::Serialize(const char* filename) {
    if (serializeFlag == false) {
        std::filesystem::path cereal_filename(filename);
        cereal_filename.replace_extension("cereal");
        std::ofstream ofs(cereal_filename.c_str(), std::ios::binary);
        cereal::BinaryOutputArchive serializetion(ofs);
        serializetion(scene_view, meshes, materials, AnimationClips);
    }
}
//
//void SkinnedMeshResouurce::blend_Animations(const Animation::keyframe* keyframes[2], float factor, Animation::keyframe& keyframe)
//{
//    size_t node_const{ keyframes[0]->nodes.size() };
//    keyframe.nodes.resize(node_const);
//    for (size_t node_index = 0; node_index < node_const; ++node_index)
//    {
//        XMVECTOR S[2]{
//            XMLoadFloat3(&keyframes[0]->nodes.at(node_index).scaling),
//            XMLoadFloat3(&keyframes[1]->nodes.at(node_index).scaling) };
//        XMStoreFloat3(&keyframe.nodes.at(node_index).scaling, XMVectorLerp(S[0], S[1], factor));
//
//        XMVECTOR R[2]{
//            XMLoadFloat4(&keyframes[0]->nodes.at(node_index).rotation),
//            XMLoadFloat4(&keyframes[1]->nodes.at(node_index).rotation) };
//        XMStoreFloat4(&keyframe.nodes.at(node_index).rotation, XMQuaternionSlerp(R[0], R[1], factor));
//
//        XMVECTOR T[2]{
//            XMLoadFloat3(&keyframes[0]->nodes.at(node_index).translation),
//            XMLoadFloat3(&keyframes[1]->nodes.at(node_index).translation) };
//        XMStoreFloat3(&keyframe.nodes.at(node_index).translation, XMVectorLerp(T[0], T[1], factor));
//    }
//}

// 移動値を右手座標系から左手座標系へ変換する
void SkinnedMeshResouurce::ConvertTranslationFromRHtoLH(DirectX::XMFLOAT3& translate)
{
    translate.x = -translate.x;
}
// 移動値を右手座標系から左手座標系へ変換する
void SkinnedMeshResouurce::ConvertTranslationFromRHtoLH(DirectX::XMFLOAT4& translate)
{
    translate.x = -translate.x;
}
// 回転値を右手座標系から左手座標系へ変換する
void SkinnedMeshResouurce::ConvertRotationFromRHtoLH(DirectX::XMFLOAT4& rotate)
{
    rotate.x = -rotate.x;
    rotate.w = -rotate.w;
}

// 行列値を右手座標系から左手座標系へ変換する
void SkinnedMeshResouurce::ConvertMatrixFromRHtoLH(DirectX::XMFLOAT4X4& matrix)
{
#if 0
    DirectX::XMFLOAT4X4 temp = matrix;

    DirectX::XMMATRIX M = DirectX::XMLoadFloat4x4(&matrix);

    // スケール値を取り出す
    DirectX::XMVECTOR SX = DirectX::XMVector3Length(M.r[0]);
    DirectX::XMVECTOR SY = DirectX::XMVector3Length(M.r[1]);
    DirectX::XMVECTOR SZ = DirectX::XMVector3Length(M.r[2]);
    // スケール値のない行列に変換
    M.r[0] = DirectX::XMVector3Normalize(M.r[0]);
    M.r[1] = DirectX::XMVector3Normalize(M.r[1]);
    M.r[2] = DirectX::XMVector3Normalize(M.r[2]);
    // 回転行列をクォータニオンへ変換
    DirectX::XMVECTOR Q = DirectX::XMQuaternionRotationMatrix(M);
    DirectX::XMFLOAT4 q;
    DirectX::XMStoreFloat4(&q, Q);
    // クォータニオンを左手座標系へ変換
    q.x = -q.x;
    q.w = -q.w;
    // クォータニオンを回転行列へ変換
    Q = DirectX::XMLoadFloat4(&q);
    M = DirectX::XMMatrixRotationQuaternion(Q);
    // スケール値を戻す
    M.r[0] = DirectX::XMVectorMultiply(M.r[0], SX);
    M.r[1] = DirectX::XMVectorMultiply(M.r[1], SY);
    M.r[2] = DirectX::XMVectorMultiply(M.r[2], SZ);
    // 回転＆スケール行列を取り出す
    DirectX::XMStoreFloat4x4(&matrix, M);
    // 移動値を左手座標系へ変換しつつ設定
    matrix._41 = -temp._41;
    matrix._42 = temp._42;
    matrix._43 = temp._43;
#else
    matrix._12 = -matrix._12;
    matrix._13 = -matrix._13;
    matrix._21 = -matrix._21;
    matrix._31 = -matrix._31;
    matrix._41 = -matrix._41;
#endif
}

// インデックスバッファを右手座標系から左手座標系へ変換する
void SkinnedMeshResouurce::ConvertIndexBufferFromRHtoLH(std::vector<UINT>& indices)
{
    size_t size = indices.size();
    UINT* p = indices.data();
    for (size_t i = 0; i < size; i += 3)
    {
        UINT temp = p[1];
        p[1] = p[2];
        p[2] = temp;

        p += 3;
    }
}
