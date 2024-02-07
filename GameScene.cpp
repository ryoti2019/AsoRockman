#include <DxLib.h>
#include "GameScene.h"
#include "Stage.h"
#include"Player.h"
#include "Shot.h"

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	// �X�e�[�W�̏�����
	stage_ = new Stage();
	stage_->Init();

	// �v���C���[1�̏�����
	player_[0] = new Player();
	Player::KEY_CONFIG keyP1 = {
		KEY_INPUT_UP, KEY_INPUT_DOWN, KEY_INPUT_LEFT,
		KEY_INPUT_RIGHT, KEY_INPUT_N, KEY_INPUT_M
	};
	player_[0]->Init(this, Player::TYPE::PLAYER_1, keyP1);

	// �v���C���[2�̏�����
	player_[1] = new Player();
	Player::KEY_CONFIG keyP2 = {
		KEY_INPUT_W, KEY_INPUT_S, KEY_INPUT_A,
		KEY_INPUT_D, KEY_INPUT_LCONTROL, KEY_INPUT_LSHIFT
	};
	player_[1]->Init(this, Player::TYPE::PLAYER_2, keyP2);

}

void GameScene::Update(void)
{

	// �X�e�[�W�̍X�V
	stage_->Update();

	// �v���C���[�̍X�V
	player_[0]->Update();
	player_[1]->Update();

	// �e�̍X�V
	auto shots = player_[0]->GetShots();

	for (auto shot : shots)	// �͈�for��
	{
		shot->Update();
	}

	// �e�̍X�V
	auto shots2 = player_[1]->GetShots();

	for (auto shot : shots2)	// �͈�for��
	{
		shot->Update();
	}

	//std::vector<Shot*> shots = player_->GetShots();

	//for (Shot* shot : shots)
	//{
	//	shot->Update();
	//}

	// �Փ˔���
	Collision();


}

void GameScene::Draw(void)
{
	// �X�e�[�W�̕`��
	stage_->Draw();

	// �v���C���[�̕`��
	player_[0]->Draw();
	player_[1]->Draw();

	// �e�̕`��
	auto shots = player_[0]->GetShots();

	for (auto shot : shots)	// �͈�for��
	{
		shot->Draw();
	}

	// �e�̕`��
	auto shots2 = player_[1]->GetShots();

	for (auto shot : shots2)	// �͈�for��
	{
		shot->Draw();
	}

}

void GameScene::Release(void)
{
	// �X�e�[�W�J��
	stage_->Release();
	delete stage_;

	// �v���C���[�J��
	player_[0]->Release();
	delete player_[0];

	// �v���C���[�J��
	player_[1]->Release();
	delete player_[1];
}

Vector2 GameScene::World2MapPos(Vector2 worldPos)
{

	Vector2 ret;

	int  mapX = worldPos.x / Stage::CHIP_SIZE_X;
	int  mapY = worldPos.y / Stage::CHIP_SIZE_Y;

	ret.x = mapX;
	ret.y = mapY;

	return ret;

}

bool GameScene::IsCollisionStage(Vector2 worldPos)
{

	// ���[���h���W����}�b�v���W�ɕϊ�����
	Vector2 mapPos = World2MapPos(worldPos);

	// �}�b�v���W����}�b�v�`�b�v�ԍ����擾����
	int chipNo = stage_->GetChipNo(mapPos);

	// �}�b�v�`�b�v��8�ȏ�̏ꍇ�́A�Փ˂���
	if (chipNo >= 8)
	{
		return true;
	}

	return false;

}

void GameScene::Collision(void)
{

	// �e�����擾����
	std::vector<Shot*> shots = player_[0]->GetShots();
	for (Shot* shot : shots)
	{

		// �e���������Ă��Ȃ���΁A�������΂�
		if (!shot->IsAlive())
		{
			continue;
		}

		// �e�ƃX�e�[�W�̏Փ˔���
		Vector2 shotPos = shot->GetPos().ToVector2();

		if (IsCollisionStage(shotPos))
		{
			// �����G�t�F�N�g�𔭐�������
			shot->ShotBlast();
		}
	}

	// �e�����擾����
	std::vector<Shot*> shots2 = player_[1]->GetShots();
	for (Shot* shot : shots2)
	{

		// �e���������Ă��Ȃ���΁A�������΂�
		if (!shot->IsAlive())
		{
			continue;
		}

		// �e�ƃX�e�[�W�̏Փ˔���
		Vector2 shotPos = shot->GetPos().ToVector2();

		if (IsCollisionStage(shotPos))
		{
			// �����G�t�F�N�g�𔭐�������
			shot->ShotBlast();
		}
	}

}
