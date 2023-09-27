#pragma once
#include<d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <wrl.h>
#include <string>

using namespace DirectX;

//���̈ʒu��ۑ�����\����
struct PosBuffer
{
    XMFLOAT3 headPos; //���̐�[�̈ʒu
    XMFLOAT3 tailPos; //���̖��[�̈ʒu
};

//���_�o�b�t�@
struct SwordTrailVertex
{
    XMFLOAT3 position = { 0, 0, 0 };
    XMFLOAT2 texcoord = { 0, 0 };
};

class SwordTrail
{
private:
    
    PosBuffer tempPos;                           //���݃t���[���ł̌��̈ʒu
    Microsoft::WRL::ComPtr<ID3D11Buffer>	vertexBuffer;
public:
    SwordTrail(int bufferSize, const wchar_t* filename);                   //�R���X�g���N�^��posArray�̃T�C�Y�����߂�
    void Update();                                //�������X�V���āA���_�o�b�t�@���X�V����
    void SetPos(XMFLOAT3& head, XMFLOAT3& tail);  //���݃t���[���̌��̈ʒu��ۑ�����
    void CreateCurveVertex(std::vector<PosBuffer>& usedPosArray);
    int split = 5;
     //���t���[���O�O�Ղ̈ʒu
     DirectX::XMFLOAT3 GetTrail(int i) { return posArray[i].tailPos; };

     DirectX::XMFLOAT3 GetTrailVertex(int i) { return vertex[i].position; };

    std::vector<PosBuffer> posArray;             //���̈ʒu��ۑ�����o�b�t�@
    std::vector<SwordTrailVertex> vertex;        //���_�o�b�t�@

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shaderResourceView;
    const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() const { return shaderResourceView; }
    const std::vector<SwordTrailVertex>& GetVertex() { return vertex; };
    // ���_�o�b�t�@�̎擾
    const Microsoft::WRL::ComPtr<ID3D11Buffer>& GetVertexBuffer() const { return vertexBuffer; }

    

};