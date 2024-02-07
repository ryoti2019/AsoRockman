#pragma once
#include "SceneBase.h"
#include "Vector2.h"

class Stage
{

public:

	// �X�e�[�W�摜�̉��̕�����
	static constexpr int NUM_MAP_CHIPS_X = 13;

	// �X�e�[�W�摜�̏c�̕�����
	static constexpr int NUM_MAP_CHIPS_Y = 1;

	// �X�e�[�W�摜�̕�������
	static constexpr int NUM_MAP_CHIPS = NUM_MAP_CHIPS_X * NUM_MAP_CHIPS_Y;

	// �}�b�v�`�b�v�T�C�Y
	static constexpr int CHIP_SIZE_X = 32;
	static constexpr int CHIP_SIZE_Y = 32;

	// �}�b�v�̃T�C�Y
	static constexpr int MAP_SIZE_X = 48;
	static constexpr int MAP_SIZE_Y = 15;

	// �R���X�g���N�^
	Stage(void);

	// �f�X�g���N�^
	~Stage(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// �O���t�@�C������n��̃X�e�[�W�f�[�^��ǂݍ���
	void LoadGroundCsvData(void);

	// �}�b�v�`�b�v�ԍ����擾
	int GetChipNo(Vector2 mapPos);

private:

	// �}�b�v�摜
	int imgMapChips_[NUM_MAP_CHIPS];

	// �}�b�v�f�[�^
	int map_[MAP_SIZE_Y][MAP_SIZE_X];

};

