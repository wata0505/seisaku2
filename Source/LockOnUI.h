#pragma once
#include "UI.h"

//���b�N�I��UI�N���X
class LockOnUI : public UI
{
public:
	LockOnUI(const wchar_t* filename);
	~LockOnUI() {};

	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	virtual void Render(RenderContext& rc, SpriteShader* shader);

	// �j��
	void Destroy();

	//UIManager* manager = nullptr;
private:
	float lockonAngle;
};