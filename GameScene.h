#pragma once
#include "SceneBase.h"
#include "Vector2.h"

class Stage;
class Player;

class GameScene : public SceneBase
{

public:

	// �R���X�g���N�^
	GameScene(void);

	// �f�X�g���N�^
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	// ���[���h(�X�N���[��)���W���}�b�v���W�ɕϊ�����
	Vector2 World2MapPos(Vector2 worldPos);

	// �}�b�v�`�b�v���g�p�����Փ˔���
	bool IsCollisionStage(Vector2 worldPos);

private:

	// �X�e�[�W
	Stage* stage_;

	// �v���C���[
	Player* player_[2];

	// �Փ˔���
	void Collision(void);

};

