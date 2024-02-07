#pragma once
#include "AsoUtility.h"
#include "Vector2F.h"
class Player;
class Blast;

class Shot
{

public:

	// �e�̃T�C�Y
	static constexpr int IMG_SIZE_X = 16;
	static constexpr int IMG_SIZE_Y = 16;

	// �e�̐�������
	static constexpr float SHOT_ALIVE = 1.5f;

	// �R���X�g���N�^(�e�摜�̃n���h��ID)
	Shot(int img);

	// �f�X�g���N�^
	~Shot(void);

	// �e�̐���(�e�̐i�s�����A�\���J�n���W)
	void CreateShot(Vector2 pos, Vector2F dir);

	// �X�V�X�e�b�v
	void Update(void);

	// �`��
	void Draw(void);

	// �J������
	void Release(void);

	// ��������
	bool IsAlive(void);

	// �������Ԃ̐ݒ�
	void SetAlive(float time);

	// �e�̍��W�̎擾
	Vector2F GetPos(void);

	// �e�𔚔�������
	void ShotBlast(void);

private:

	// �����G�t�F�N�g
	Blast* blast_;

	// �V���b�g�摜
	int img_;

	// ����
	Vector2F dir_;

	// �e�̍��W
	Vector2F pos_;

	// �e�̈ړ����x
	float speed_;

	// �e�̐������Ԍv�Z�p
	float stepAlive_;

};

