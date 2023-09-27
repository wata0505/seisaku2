#pragma once

#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "SkinnedMesh.h"
#include "Shader.h"

class BaseModel
{
public:
	BaseModel(const char* filename, bool triangulate);
	virtual	~BaseModel() {}

	// �f�o�b�O���\��
	void DrawDebugGUI();

	// ���\�[�X�擾
	const SkinnedMeshResouurce* GetResource() const { return resource.get(); }

	// �ÓI���f��������
	bool IsStaticModel() const { return staticFlag; }
	void SetStaticModelFlag(bool staticFlag) { this->staticFlag = staticFlag; }

public:
	// �V�F�[�_�[�ւ̐ݒ�p�o�b�t�@���擾
	virtual BufferData GetBufferData(const SkinnedMeshResouurce::Mesh& mesh) const = 0;

protected:
	std::shared_ptr<SkinnedMeshResouurce>	resource;
	bool							staticFlag = false;	//	�ÓI���f���t���O
};
