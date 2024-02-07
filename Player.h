#pragma once
#include "Vector2F.h"
#include "AsoUtility.h"

class GameScene;
class Shot;

class Player
{

public:

	static const int SIZE_X = 96; // ���T�C�Y
	static const int SIZE_Y = 64; // �c�T�C�Y

	// �ő�A�j���[�V������(���郂�[�V�����̂�)
	static const int MAX_NUM_ANIM = 4;

	// �A�j���[�V�����X�s�[�h
	static constexpr float ANIM_SPEED = 0.1f;

	// �ړ����x�̍ő�l
	static constexpr float MAX_MOVE_SPEED = 3.5f;

	// ����(accelerator=�A�N�Z�����[�^�[)
	static constexpr float MOVE_ACC = 0.25f;

	// ����(decelerate=�f�B�Z�����C�g)
	static constexpr float MOVE_DEC = 0.05f;

	// �ő�W�����v��
	static constexpr float MAX_JUMP_POW = 8.5f;

	// �d��
	static constexpr float GRAVITY = 0.25f;

	// �W�����v�L�[���͂��󂯕t����t���[����
	static constexpr int INPUT_JUMP_FRAME = 6;

	// �e���ˌ�̍d������
	static constexpr float SHOT_DELAY = 1.0f;

	// �A�j���[�V�������
	enum class ANIM_STATE
	{
		IDLE,
		RUN,
		JUMP,
		DAMAGED,
		MAX
	};

	// �U�����
	enum class ATK_STATE
	{
		NONE,
		SHOT,
		MAX
	};

	// �Փˍ��W(���E)
	enum class COL_LR
	{
		C,
		L,
		R
	};

	// �Փˍ��W(�㉺)
	enum class COL_TD
	{
		C,
		T,
		D
	};

	// �v���C���[
	enum class TYPE
	{
		PLAYER_1,
		PLAYER_2
	};

	// �L�[�R���t�B�O
	struct KEY_CONFIG
	{
		int UP;
		int DOWN;
		int LEFT;
		int RIGHT;
		int ATTACK;
		int JUMP;
	};

	// �R���X�g���N�^
	Player(void);

	// �f�X�g���N�^
	~Player(void);

	void Init(GameScene* scene, TYPE type, KEY_CONFIG config);
	void Update(void);
	void Draw(void);
	void Release(void);

	// �e�̎擾
	std::vector<Shot*> GetShots(void);

private:

	// �Q�[���V�[���̃|�C���^�ϐ�
	GameScene* gameScene_;

	// ��Ԃ��Ƃ̉摜(���[�V�����A�U���A�A�j���[�V������)
	//int images_[4][2][4];

	int images_[static_cast<int>(ANIM_STATE::MAX)]
		[static_cast<int>(ATK_STATE::MAX)][MAX_NUM_ANIM];
	
	// ���W
	Vector2F pos_;

	// �A�j���[�V�������
	ANIM_STATE animState_;

	// �U�����
	ATK_STATE atkState_;

	// �A�j���[�V�����J�E���^
	float stepAnim_;

	// �v���C���[�̌���
	AsoUtility::DIR dir_;

	// �ړ����x
	float moveSpeed_;

	// �W�����v��
	float jumpPow_;

	// �W�����v������
	bool isJump_;

	// �W�����v�L�[�̉�������
	bool isPutJumpKey_;

	// �W�����v�L�[���̓J�E���^
	int cntJumpInput_;

	// �Փ˔���p�F���S���W(pos_����̑��΍��W)
	Vector2 hitPos_;

	// �Փ˔���p�F�͈�
	Vector2 hitBox_;

	// �V���b�g
	std::vector<Shot*> shots_;

	// �v���C���[���
	TYPE type_;

	// �L�[�ݒ�
	KEY_CONFIG keyConfig_;

	// �e�摜
	int imgShot_;

	// �e���ˌ�̍d�����Ԍv�Z�p
	float stepShotDelay_;

	// �摜�̓ǂݍ���
	void LoadImages(void);

	// �v���C���[�̕`��
	void DrawPlayer(int handleId);

	// �f�o�b�O�̕`��
	void DrawDebug(void);

	// �v���C���[�̈ړ�����
	void ProcessMove(void);

	// �ړ�(���ۂ̍��W�ړ�)
	void Move(void);

	// ����(�X�s�[�h��������)
	void Accele(float speed);

	// ����(�X�s�[�h������������)
	void Decelerate(float speed);

	// �v���C���[�̃W�����v����
	void ProcessJump(void);

	// �ړ�(���ۂ̍��W�ړ�)
	void Jump(void);

	// �W�����v�͂̐ݒ�
	void SetJumpPow(float pow);

	// �d�͂�������
	void AddGravity(void);

	// �����̏Փ˔���
	void CollisionFoot(void);

	// ���̏Փ˔���
	void CollisionHead(void);

	// ���E�̏Փ˔���
	void CollisionSide(void);

	// �Փ˔�����擾����
	Vector2 GetColPos(COL_LR lr, COL_TD td);

	// �U��
	void Attack(void);

	// �L���Ȓe���擾����
	Shot* GetValidShot(void);
};

