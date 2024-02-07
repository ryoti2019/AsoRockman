#include <string>
#include <DxLib.h>
#include "Application.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "GameScene.h"
#include "AsoUtility.h"
#include "Vector2.h"
#include "Stage.h"
#include "Shot.h"
#include "Player.h"

Player::Player(void)
{
}

Player::~Player(void)
{
}

void Player::Init(GameScene* scene, TYPE type, KEY_CONFIG config)
{


	// �Q�[���V�[���̋@�\���g����悤��
	gameScene_ = scene;

	// �v���C���[���
	type_ = type;

	// �L�[�ݒ�
	keyConfig_ = config;

	// �摜�̓ǂݍ���
	LoadImages();

	// �A�j���[�V�������
	animState_ = ANIM_STATE::IDLE;

	// �U�����
	atkState_ = ATK_STATE::NONE;

	// �A�j���[�V�����J�E���^
	stepAnim_ = 0.0f;

	// �������W
	pos_ = { 300.0f, 180.0f };

	// �v���C���[�̌���
	dir_ = AsoUtility::DIR::LEFT;

	// �ړ����x�̏�����
	moveSpeed_ = 0.0f;

	// �W�����v��
	jumpPow_ = 0.0f;

	// �W�����v����
	isJump_ = true;

	// �W�����v�L�[���̓J�E���^
	// �X�^�[�g����́A1�񒅒n���Ȃ��ƃW�����v�����Ȃ�
	cntJumpInput_ = INPUT_JUMP_FRAME;

	// �W�����v�L�[��������
	isPutJumpKey_ = false;

	// �Փ˔���p�F���S���W(pos_����̑��΍��W)
	hitPos_ = { 0, 8 };

	// �Փ˔���p�F�͈�
	hitBox_ = { 16, 24 };

	// �V���b�g�̍d������
	stepShotDelay_ = 0.0f;

}


void Player::Update(void)
{

	// ���[�V�����̏�����
	animState_ = ANIM_STATE::IDLE;

	// �v���C���[�̈ړ�����
	ProcessMove();

	// ����
	Decelerate(MOVE_DEC);

	// �ړ�(���ۂ̍��W�ړ�)
	Move();

	// ���E�̏Փ˔���
	CollisionSide();

	// �v���C���[�̃W�����v����
	ProcessJump();

	// �d�͂���ɂ�����
	AddGravity();

	// �W�����v
	Jump();

	// �����̏Փ˔���
	CollisionFoot();

	// ���̏Փˍ��W
	CollisionHead();

	// �U��
	Attack();

}

void Player::Draw(void)
{

	// �W�����v�A�j���[�V�����ݒ�
	if (isJump_)
	{
		animState_ = ANIM_STATE::JUMP;
	}

	// �A�j���[�V�������
	int animState = static_cast<int>(animState_);

	// �U�����
	int atkState = static_cast<int>(atkState_);

	switch (animState_)
	{
	case ANIM_STATE::IDLE:
	case ANIM_STATE::JUMP:
		DrawPlayer(images_[animState][atkState][0]);
		break;
	case ANIM_STATE::RUN:
	{
		stepAnim_ += ANIM_SPEED;
		int animIdx = AsoUtility::Round(stepAnim_) % MAX_NUM_ANIM;
		DrawPlayer(images_[animState][atkState][animIdx]);
	}
	break;
	case ANIM_STATE::DAMAGED:
		DrawPlayer(images_[animState][0][0]);
		break;
	}

	//DrawDebug();
}

void Player::Release(void)
{

	int animMax = static_cast<int>(ANIM_STATE::MAX);
	int atkMax = static_cast<int>(ATK_STATE::MAX);

	// �A�j���[�V������ʂ̃��[�v
	for (int m = 0; m < animMax; m++)
	{
		// �U����ʂ̃��[�v
		for (int k = 0; k < atkMax; k++)
		{
			// �A�j���[�V�����R�}�̃��[�v
			for (int n = 0; n < MAX_NUM_ANIM; n++)
			{
				DeleteGraph(images_[m][k][n]);
			}
		}
	}

	// �e�摜�̊J��
	DeleteGraph(imgShot_);

	// �e�̊J��
	size_t size = shots_.size();
	for (int i = 0; i < size; i++)
	{
		shots_[i]->Release();
		delete shots_[i];
	}

}

std::vector<Shot*> Player::GetShots(void)
{
	return shots_;
}

void Player::LoadImages(void)
{

	std::string basePath = Application::PATH_IMG_PLAYER1;

	int anim = 0;
	int atkNone = static_cast<int>(ATK_STATE::NONE);
	int atkShot = static_cast<int>(ATK_STATE::SHOT);

	// �ҋ@ ���P�����Ȃ��摜�͌ʂɓǂݍ���
	anim = static_cast<int>(ANIM_STATE::IDLE);
	images_[anim][atkNone][0] = LoadGraph((basePath + "Idle.png").c_str());
	images_[anim][atkShot][0] = LoadGraph((basePath + "IdleShot.png").c_str());

	// �W�����v
	anim = static_cast<int>(ANIM_STATE::JUMP);
	images_[anim][atkNone][0] = LoadGraph((basePath + "Jump.png").c_str());
	images_[anim][atkShot][0] = LoadGraph((basePath + "JumpShot.png").c_str());

	// ��_���[�W�F�U������
	anim = static_cast<int>(ANIM_STATE::DAMAGED);
	images_[anim][atkNone][0] = LoadGraph((basePath + "Damaged.png").c_str());

	// ����
	anim = static_cast<int>(ANIM_STATE::RUN);
	LoadDivGraph(
		(basePath + "Run.png").c_str(),
		MAX_NUM_ANIM,
		MAX_NUM_ANIM, 1,
		SIZE_X, SIZE_Y,
		&images_[anim][atkNone][0],
		false
	);

	LoadDivGraph(
		(basePath + "RunShot.png").c_str(),
		MAX_NUM_ANIM,
		MAX_NUM_ANIM, 1,
		SIZE_X, SIZE_Y,
		&images_[anim][atkShot][0],
		false
	);

	// �e�摜
	imgShot_ = LoadGraph((basePath + "Shot.png").c_str());

}

void Player::DrawPlayer(int handleId)
{

	// ���W�ϊ�
	Vector2 pos = pos_.ToVector2();

	// �v���C���[�̌���
	bool isLeftDir = true;
	if (dir_ == AsoUtility::DIR::RIGHT)
	{
		isLeftDir = false;
	}

	// �`��(���W�A�g�嗦�A��]�A�n���h��ID�A���߃t���O�A�����]�A�c���])
	DrawRotaGraph(
		pos.x, pos.y,
		1.0, 0.0,
		handleId, true,
		isLeftDir);

}

void Player::DrawDebug(void)
{

	DrawFormatString(0, 0, 0x000000, "�v���C���[���W(%.2f, %.2f)", pos_.x, pos_.y);

	DrawFormatString(0, 20, 0x000000, "�ړ����x : %.2f", moveSpeed_);

	DrawFormatString(0, 20, 0x000000, "�W�����v�� : %.2f", jumpPow_);

	Vector2 pos = pos_.ToVector2();

	// �v���C���[�̉摜�g
	DrawBox(pos.x - SIZE_X / 2, pos.y - SIZE_Y / 2,
		pos.x + SIZE_X / 2, pos.y + SIZE_Y / 2, 0x000000, false);

	DrawBox(pos.x - 3, pos.y - 3, pos.x + 3, pos.y + 3, 0xff0000, true);

	// �I�����W
	int color = 0xff8c00;

	// �f�o�b�O�p�F�����Փ˔���
	Vector2 footPos = pos;
	footPos.y += (8 + 24);

	// �����F����
	DrawBox(footPos.x - 3, footPos.y - 3, // �������W
			footPos.x + 3, footPos.y + 3, // �E�����W
			color, true);

	// �����F��
	DrawBox(footPos.x - (32 / 2) - 3, footPos.y - 3,
		footPos.x - (32 / 2) + 3, footPos.y + 3, color, true);

	// �����F�E
	DrawBox(footPos.x + (32 / 2) - 3, footPos.y - 3,
		footPos.x + (32 / 2) + 3, footPos.y + 3, color, true);

}

void Player::ProcessMove(void)
{

	// ������
	if (InputManager::GetInstance().IsNew(keyConfig_.LEFT))
	{
		dir_ = AsoUtility::DIR::LEFT;

		// ����
		Accele(-MOVE_ACC);

		animState_ = ANIM_STATE::RUN;
	}

	// �E����
	if (InputManager::GetInstance().IsNew(keyConfig_.RIGHT))
	{
		dir_ = AsoUtility::DIR::RIGHT;

		// ����
		Accele(MOVE_ACC);

		animState_ = ANIM_STATE::RUN;
	}

}

void Player::Move(void)
{
	pos_.x += moveSpeed_;
}

void Player::Accele(float speed)
{

	moveSpeed_ += speed;

	// ���x����(�E����)
	if (moveSpeed_ > MAX_MOVE_SPEED)
	{
		moveSpeed_ = MAX_MOVE_SPEED;
	}

	// ���x����(������)
	if (moveSpeed_ < -MAX_MOVE_SPEED)
	{
		moveSpeed_ = -MAX_MOVE_SPEED;
	}
}

void Player::Decelerate(float speed)
{

	// �E�����ړ�������������
	if (moveSpeed_ > 0.0f)
	{
		moveSpeed_ -= speed;
		if (moveSpeed_ < 0.0f)
		{
			moveSpeed_ = 0.0f;
		}
	}

	// �������ړ�������������
	if (moveSpeed_ < 0.0f)
	{
		moveSpeed_ += speed;
		if (moveSpeed_ > 0.0f)
		{
			moveSpeed_ = 0.0f;
		}
	}
}

void Player::ProcessJump(void)
{

	// �P���W�����v
	//if (InputManager::GetInstance().IsNew(KEY_INPUT_M) && !isJump_)
	//{
	//	SetJumpPow(-MAX_JUMP_POW);
	//	isJump_ = true;
	//}

	InputManager& ins = InputManager::GetInstance();

	// �ڒn���Ă��Ȃ��ƁA�W�����v���J�n�ł��Ȃ��悤�ɂ���
	if (ins.IsTrgDown(keyConfig_.JUMP) && !isJump_)
	{
		isJump_ = true;
		isPutJumpKey_ = true;
	}


	// ���͎��Ԃɉ����āA�W�����v�ʂ�ύX����
	if (ins.IsNew(keyConfig_.JUMP)
		&& cntJumpInput_ < INPUT_JUMP_FRAME
		&& isPutJumpKey_)
	{

		// �W�����v�J�E���^�𑝂₷
		cntJumpInput_ += 1;

		// A �Ӗ����Ȃ�
		//SetJumpPow(-MAX_JUMP_POW);

		// B �W�����v�ʂɉ��Z�͂ł������A�͂��傫������
		//float pow = jumpPow_ - MAX_JUMP_POW;
		//SetJumpPow(pow);

		// C �W�����v�͂ɕ��z���Z
		float pow = jumpPow_ - (MAX_JUMP_POW / static_cast<float>(INPUT_JUMP_FRAME));
		SetJumpPow(pow);

	}

	// �Q�i�W�����v���֎~����
	if (ins.IsTrgUp(keyConfig_.JUMP))
	{
		// �W�����v�{�^���������ꂽ�Ƃ�
		cntJumpInput_ = INPUT_JUMP_FRAME;
	}
}

void Player::Jump(void)
{

	pos_.y += jumpPow_;

	//// ���̐ڒn(�Փ�)����
	//if (pos_.y > 380.0f)
	//{
	//	pos_.y = 380.0f;

	//	// �n�ʂɂ����̂ŃW�����v�����Z�b�g
	//	isJump_ = false;
	//	SetJumpPow(0.0f);

	// //�ڒn������W�����v�J�E���^��0�ɂ���
	//	cntJumpInput_ = 0;
	//}

}

void Player::SetJumpPow(float pow)
{

	// �W�����v�͂�ݒ�
	jumpPow_ = pow;

	// �d�͂������肷����̂�h��
	if (jumpPow_ > MAX_JUMP_POW)
	{
		jumpPow_ = MAX_JUMP_POW;
	}

}

void Player::AddGravity(void)
{

	// ���݂̃W�����v�͂ɏd�͂�������
	float pow = jumpPow_ + GRAVITY;
	SetJumpPow(pow);

}

void Player::CollisionFoot(void)
{

	// �ڒn����(�����̏Փ˔���)

	// �������W(���S)
	Vector2 footPosC = GetColPos(COL_LR::C, COL_TD::D);

	// �������W(��)
	Vector2 footPosL = GetColPos(COL_LR::L, COL_TD::D);

	// �������W(�E)
	Vector2 footPosR = GetColPos(COL_LR::R, COL_TD::D);

	if (gameScene_->IsCollisionStage(footPosC)
		|| gameScene_->IsCollisionStage(footPosL)
		||gameScene_->IsCollisionStage(footPosR))
	{

		Vector2 mapPos = gameScene_->World2MapPos(footPosC);
		pos_.y = static_cast<float>
			(mapPos.y * Stage::CHIP_SIZE_Y - 1 - hitPos_.y - hitBox_.y);

		// �n�ʂɂ����̂ŃW�����v�����Z�b�g
		isJump_ = false;
		SetJumpPow(0.0f);

		// �ڒn������W�����v�J�E���^�����ɖ߂�
		cntJumpInput_ = 0;

		// �W�����v�L�[�̉�������
		isPutJumpKey_ = false;

	}

	// �󒆔���
	footPosC.y += 1;
	footPosL.y += 1;
	footPosR.y += 1;
	if (!(gameScene_->IsCollisionStage(footPosC)
		|| gameScene_->IsCollisionStage(footPosL)
		|| gameScene_->IsCollisionStage(footPosR)))
	{

		// �ڒn���Ă��Ȃ��̂ŁA�W�����v����ɂ���
		isJump_ = true;

	}

}

void Player::CollisionHead(void)
{

	// �ڒn����(���̏Փ˔���)

	// ���̍��W(���S)
	Vector2 headPosC = GetColPos(COL_LR::C, COL_TD::T);

	// ���̍��W(��)
	Vector2 headPosL = GetColPos(COL_LR::L, COL_TD::T);

	// ���̍��W(�E)
	Vector2 headPosR = GetColPos(COL_LR::R, COL_TD::T);

	if (gameScene_->IsCollisionStage(headPosC)
		|| gameScene_->IsCollisionStage(headPosL)
		|| gameScene_->IsCollisionStage(headPosR))
	{

		Vector2 mapPos = gameScene_->World2MapPos(headPosC);
		pos_.y = static_cast<float>
			(mapPos.y * Stage::CHIP_SIZE_Y
				+ Stage::CHIP_SIZE_Y + SIZE_Y / 2
				+ hitPos_.y - hitBox_.y);

		// �����Ԃ�������A�W�����v�͂�0�ɂ���
		SetJumpPow(0.0f);

	}

}

void Player::CollisionSide(void)
{

	// ���E�̏Փˏ���

	//���̍��W(���S)
	Vector2 headPosL = GetColPos(COL_LR::L, COL_TD::T);

	// ���̍��W(��)
	Vector2 corePosL = GetColPos(COL_LR::L, COL_TD::C);

	// ���̍��W(�E)
	Vector2 footPosL = GetColPos(COL_LR::L, COL_TD::D);

	if (gameScene_->IsCollisionStage(headPosL)
		|| gameScene_->IsCollisionStage(corePosL)
		|| gameScene_->IsCollisionStage(footPosL))
	{

		Vector2 mapPos = gameScene_->World2MapPos(headPosL);
		pos_.x = static_cast<float>
			(mapPos.x * Stage::CHIP_SIZE_X
				+ Stage::CHIP_SIZE_X + hitBox_.x);

	}

	//�E�̍��W(���S)
	Vector2 headPosR = GetColPos(COL_LR::R, COL_TD::T);

	// �E�̍��W(��)
	Vector2 corePosR = GetColPos(COL_LR::R, COL_TD::C);

	// �E�̍��W(�E)
	Vector2 footPosR = GetColPos(COL_LR::R, COL_TD::D);

	if (gameScene_->IsCollisionStage(headPosR)
		|| gameScene_->IsCollisionStage(corePosR)
		|| gameScene_->IsCollisionStage(footPosR))
	{

		Vector2 mapPos = gameScene_->World2MapPos(headPosR);
		pos_.x = static_cast<float>
			(mapPos.x * Stage::CHIP_SIZE_X - 1 - hitPos_.x - hitBox_.x);

	}

}

Vector2 Player::GetColPos(COL_LR lr, COL_TD td)
{

	Vector2 ret = pos_.ToVector2();

	// ���΍��W�𑫂�
	ret.x += hitPos_.x;
	ret.y += hitPos_.y;

	// ���E
	switch (lr)
	{
	case Player::COL_LR::L:
		ret.x -= hitBox_.x;
		break;
	case Player::COL_LR::R:
		ret.x += hitBox_.x;
		break;
	}

	// �㉺
	switch (td)
	{
	case Player::COL_TD::T:
		ret.y -= hitBox_.y;
		break;
	case Player::COL_TD::D:
		ret.y += hitBox_.y;
		break;
	}

	return ret;

}

void Player::Attack(void)
{

	// �U���L�[�������ƁA�e�𐶐�����
	if (InputManager::GetInstance().IsNew(keyConfig_.ATTACK) && stepShotDelay_ <= 0.0f)
	{
		
		// �L���ȋ����擾����
		Shot* shot = GetValidShot();
		
		// ���b�N�}���̘r�̒������A���W�����炷
		Vector2F pos = pos_;

		//if (dir_ == AsoUtility::DIR::LEFT)
		//{
		//	pos.x -= 34.0f;
		//	pos.y += 6.0f;
		//}
		//if (dir_ == AsoUtility::DIR::RIGHT)
		//{
		//	pos.x += 34.0f;
		//	pos.y += 6.0f;
		//}

		// �e�𐶐�(�e���˂̏���)
		Vector2F dir = AsoUtility::Normalize(dir_);

		float powX = 35.0f;
		float movePowX = dir.x * powX;	// �E20 or ��-20
		pos.x += movePowX;

		// �W�����v���Ă�����Ay-10�A���Ă��Ȃ�������y+5
		if (isJump_)
		{
			pos.y -= 10;
		}
		if (!isJump_)
		{
			pos.x += 5;
		}

		shot->CreateShot(pos.ToVector2(), dir);

		// �e�̔��ˌ�̍d�����ԃZ�b�g
		stepShotDelay_ = SHOT_DELAY;

	}

	atkState_ = ATK_STATE::NONE;
	if (stepShotDelay_ > 0.0f)
	{

		// �e���ˌ�̍d�����Ԃ����炵�Ă���
		stepShotDelay_ -= 1.0f / SceneManager::DEFAULT_FPS;

		// �e�������Ă���Ԃ́A�U�����Ƃ���
		atkState_ = ATK_STATE::SHOT;

	}

}

Shot* Player::GetValidShot(void)
{

	size_t size = shots_.size();
	for (int i = 0; i < size; i++)
	{
		if (!shots_[i]->IsAlive())
		{
			// �������Ă��Ȃ��e�Ȃ̂ŁA���̃C���X�^���X�����T�C�N������(�ė��p����)
			return shots_[i];
		}
	}

	// �������Ă��Ȃ��e��������Ȃ�����
	Shot* shot = new Shot(imgShot_);
	shots_.push_back(shot);

	return shot;
}
