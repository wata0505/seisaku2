#pragma once

#include<vector>
#include<set>
#include"Object.h"

//�e�ۃ}�l�[�W���[
class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	// �X�V����
	virtual void Update(float elapsedTime);

	// �`�揈��
	virtual void Render(ID3D11DeviceContext* dc, ModelShader* shader);

	// �`�揈��
	virtual void TrailRender(RenderContext& rc, SwordTrailShader* shader);

	// �f�o�b�O�v���~�e�B�u�`��
	virtual void DrawDebugPrimitive();

	// �I�u�W�F�N�g�o�^
	void Register(Object* Object);

	//�@�I�u�W�F�N�g�S�폜
	void Clear();

	// �I�u�W�F�N�g���擾
	int GetObjectCount() const { return static_cast<int>(Objects.size()); }

	// �I�u�W�F�N�g�擾
	Object* GetObject(int index) { return Objects.at(index); }

	// �I�u�W�F�N�g�폜
	void Remove(Object* Object);



private:
	std::vector<Object*> Objects;
	std::set<Object*> removes;

};