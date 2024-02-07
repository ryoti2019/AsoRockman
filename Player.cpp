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


	// ゲームシーンの機能を使えるように
	gameScene_ = scene;

	// プレイヤー種別
	type_ = type;

	// キー設定
	keyConfig_ = config;

	// 画像の読み込み
	LoadImages();

	// アニメーション状態
	animState_ = ANIM_STATE::IDLE;

	// 攻撃状態
	atkState_ = ATK_STATE::NONE;

	// アニメーションカウンタ
	stepAnim_ = 0.0f;

	// 初期座標
	pos_ = { 300.0f, 180.0f };

	// プレイヤーの向き
	dir_ = AsoUtility::DIR::LEFT;

	// 移動速度の初期化
	moveSpeed_ = 0.0f;

	// ジャンプ力
	jumpPow_ = 0.0f;

	// ジャンプ判定
	isJump_ = true;

	// ジャンプキー入力カウンタ
	// スタート直後は、1回着地しないとジャンプさせない
	cntJumpInput_ = INPUT_JUMP_FRAME;

	// ジャンプキー押下判定
	isPutJumpKey_ = false;

	// 衝突判定用：中心座標(pos_からの相対座標)
	hitPos_ = { 0, 8 };

	// 衝突判定用：範囲
	hitBox_ = { 16, 24 };

	// ショットの硬直時間
	stepShotDelay_ = 0.0f;

}


void Player::Update(void)
{

	// モーションの初期化
	animState_ = ANIM_STATE::IDLE;

	// プレイヤーの移動操作
	ProcessMove();

	// 減速
	Decelerate(MOVE_DEC);

	// 移動(実際の座標移動)
	Move();

	// 左右の衝突判定
	CollisionSide();

	// プレイヤーのジャンプ操作
	ProcessJump();

	// 重力を常にかける
	AddGravity();

	// ジャンプ
	Jump();

	// 足元の衝突判定
	CollisionFoot();

	// 頭の衝突座標
	CollisionHead();

	// 攻撃
	Attack();

}

void Player::Draw(void)
{

	// ジャンプアニメーション設定
	if (isJump_)
	{
		animState_ = ANIM_STATE::JUMP;
	}

	// アニメーション状態
	int animState = static_cast<int>(animState_);

	// 攻撃状態
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

	// アニメーション種別のループ
	for (int m = 0; m < animMax; m++)
	{
		// 攻撃種別のループ
		for (int k = 0; k < atkMax; k++)
		{
			// アニメーションコマのループ
			for (int n = 0; n < MAX_NUM_ANIM; n++)
			{
				DeleteGraph(images_[m][k][n]);
			}
		}
	}

	// 弾画像の開放
	DeleteGraph(imgShot_);

	// 弾の開放
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

	// 待機 ※１つしかない画像は個別に読み込み
	anim = static_cast<int>(ANIM_STATE::IDLE);
	images_[anim][atkNone][0] = LoadGraph((basePath + "Idle.png").c_str());
	images_[anim][atkShot][0] = LoadGraph((basePath + "IdleShot.png").c_str());

	// ジャンプ
	anim = static_cast<int>(ANIM_STATE::JUMP);
	images_[anim][atkNone][0] = LoadGraph((basePath + "Jump.png").c_str());
	images_[anim][atkShot][0] = LoadGraph((basePath + "JumpShot.png").c_str());

	// 被ダメージ：攻撃無し
	anim = static_cast<int>(ANIM_STATE::DAMAGED);
	images_[anim][atkNone][0] = LoadGraph((basePath + "Damaged.png").c_str());

	// 走る
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

	// 弾画像
	imgShot_ = LoadGraph((basePath + "Shot.png").c_str());

}

void Player::DrawPlayer(int handleId)
{

	// 座標変換
	Vector2 pos = pos_.ToVector2();

	// プレイヤーの向き
	bool isLeftDir = true;
	if (dir_ == AsoUtility::DIR::RIGHT)
	{
		isLeftDir = false;
	}

	// 描画(座標、拡大率、回転、ハンドルID、透過フラグ、横反転、縦反転)
	DrawRotaGraph(
		pos.x, pos.y,
		1.0, 0.0,
		handleId, true,
		isLeftDir);

}

void Player::DrawDebug(void)
{

	DrawFormatString(0, 0, 0x000000, "プレイヤー座標(%.2f, %.2f)", pos_.x, pos_.y);

	DrawFormatString(0, 20, 0x000000, "移動速度 : %.2f", moveSpeed_);

	DrawFormatString(0, 20, 0x000000, "ジャンプ力 : %.2f", jumpPow_);

	Vector2 pos = pos_.ToVector2();

	// プレイヤーの画像枠
	DrawBox(pos.x - SIZE_X / 2, pos.y - SIZE_Y / 2,
		pos.x + SIZE_X / 2, pos.y + SIZE_Y / 2, 0x000000, false);

	DrawBox(pos.x - 3, pos.y - 3, pos.x + 3, pos.y + 3, 0xff0000, true);

	// オレンジ
	int color = 0xff8c00;

	// デバッグ用：足元衝突判定
	Vector2 footPos = pos;
	footPos.y += (8 + 24);

	// 足元：中央
	DrawBox(footPos.x - 3, footPos.y - 3, // 左下座標
			footPos.x + 3, footPos.y + 3, // 右下座標
			color, true);

	// 足元：左
	DrawBox(footPos.x - (32 / 2) - 3, footPos.y - 3,
		footPos.x - (32 / 2) + 3, footPos.y + 3, color, true);

	// 足元：右
	DrawBox(footPos.x + (32 / 2) - 3, footPos.y - 3,
		footPos.x + (32 / 2) + 3, footPos.y + 3, color, true);

}

void Player::ProcessMove(void)
{

	// 左向き
	if (InputManager::GetInstance().IsNew(keyConfig_.LEFT))
	{
		dir_ = AsoUtility::DIR::LEFT;

		// 加速
		Accele(-MOVE_ACC);

		animState_ = ANIM_STATE::RUN;
	}

	// 右向き
	if (InputManager::GetInstance().IsNew(keyConfig_.RIGHT))
	{
		dir_ = AsoUtility::DIR::RIGHT;

		// 加速
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

	// 速度制限(右方向)
	if (moveSpeed_ > MAX_MOVE_SPEED)
	{
		moveSpeed_ = MAX_MOVE_SPEED;
	}

	// 速度制限(左方向)
	if (moveSpeed_ < -MAX_MOVE_SPEED)
	{
		moveSpeed_ = -MAX_MOVE_SPEED;
	}
}

void Player::Decelerate(float speed)
{

	// 右方向移動を減速させる
	if (moveSpeed_ > 0.0f)
	{
		moveSpeed_ -= speed;
		if (moveSpeed_ < 0.0f)
		{
			moveSpeed_ = 0.0f;
		}
	}

	// 左方向移動を減速させる
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

	// 単純ジャンプ
	//if (InputManager::GetInstance().IsNew(KEY_INPUT_M) && !isJump_)
	//{
	//	SetJumpPow(-MAX_JUMP_POW);
	//	isJump_ = true;
	//}

	InputManager& ins = InputManager::GetInstance();

	// 接地していないと、ジャンプを開始できないようにする
	if (ins.IsTrgDown(keyConfig_.JUMP) && !isJump_)
	{
		isJump_ = true;
		isPutJumpKey_ = true;
	}


	// 入力時間に応じて、ジャンプ量を変更する
	if (ins.IsNew(keyConfig_.JUMP)
		&& cntJumpInput_ < INPUT_JUMP_FRAME
		&& isPutJumpKey_)
	{

		// ジャンプカウンタを増やす
		cntJumpInput_ += 1;

		// A 意味がない
		//SetJumpPow(-MAX_JUMP_POW);

		// B ジャンプ量に加算はできたが、力が大きすぎる
		//float pow = jumpPow_ - MAX_JUMP_POW;
		//SetJumpPow(pow);

		// C ジャンプ力に分配加算
		float pow = jumpPow_ - (MAX_JUMP_POW / static_cast<float>(INPUT_JUMP_FRAME));
		SetJumpPow(pow);

	}

	// ２段ジャンプを禁止する
	if (ins.IsTrgUp(keyConfig_.JUMP))
	{
		// ジャンプボタンが離されたとき
		cntJumpInput_ = INPUT_JUMP_FRAME;
	}
}

void Player::Jump(void)
{

	pos_.y += jumpPow_;

	//// 仮の接地(衝突)判定
	//if (pos_.y > 380.0f)
	//{
	//	pos_.y = 380.0f;

	//	// 地面についたのでジャンプをリセット
	//	isJump_ = false;
	//	SetJumpPow(0.0f);

	// //接地したらジャンプカウンタを0にする
	//	cntJumpInput_ = 0;
	//}

}

void Player::SetJumpPow(float pow)
{

	// ジャンプ力を設定
	jumpPow_ = pow;

	// 重力がかかりすぎるのを防ぐ
	if (jumpPow_ > MAX_JUMP_POW)
	{
		jumpPow_ = MAX_JUMP_POW;
	}

}

void Player::AddGravity(void)
{

	// 現在のジャンプ力に重力を加える
	float pow = jumpPow_ + GRAVITY;
	SetJumpPow(pow);

}

void Player::CollisionFoot(void)
{

	// 接地判定(足元の衝突判定)

	// 足元座標(中心)
	Vector2 footPosC = GetColPos(COL_LR::C, COL_TD::D);

	// 足元座標(左)
	Vector2 footPosL = GetColPos(COL_LR::L, COL_TD::D);

	// 足元座標(右)
	Vector2 footPosR = GetColPos(COL_LR::R, COL_TD::D);

	if (gameScene_->IsCollisionStage(footPosC)
		|| gameScene_->IsCollisionStage(footPosL)
		||gameScene_->IsCollisionStage(footPosR))
	{

		Vector2 mapPos = gameScene_->World2MapPos(footPosC);
		pos_.y = static_cast<float>
			(mapPos.y * Stage::CHIP_SIZE_Y - 1 - hitPos_.y - hitBox_.y);

		// 地面についたのでジャンプをリセット
		isJump_ = false;
		SetJumpPow(0.0f);

		// 接地したらジャンプカウンタを元に戻す
		cntJumpInput_ = 0;

		// ジャンプキーの押下判定
		isPutJumpKey_ = false;

	}

	// 空中判定
	footPosC.y += 1;
	footPosL.y += 1;
	footPosR.y += 1;
	if (!(gameScene_->IsCollisionStage(footPosC)
		|| gameScene_->IsCollisionStage(footPosL)
		|| gameScene_->IsCollisionStage(footPosR)))
	{

		// 接地していないので、ジャンプ判定にする
		isJump_ = true;

	}

}

void Player::CollisionHead(void)
{

	// 接地判定(頭の衝突判定)

	// 頭の座標(中心)
	Vector2 headPosC = GetColPos(COL_LR::C, COL_TD::T);

	// 頭の座標(左)
	Vector2 headPosL = GetColPos(COL_LR::L, COL_TD::T);

	// 頭の座標(右)
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

		// 頭がぶつかったら、ジャンプ力を0にする
		SetJumpPow(0.0f);

	}

}

void Player::CollisionSide(void)
{

	// 左右の衝突処理

	//左の座標(中心)
	Vector2 headPosL = GetColPos(COL_LR::L, COL_TD::T);

	// 左の座標(左)
	Vector2 corePosL = GetColPos(COL_LR::L, COL_TD::C);

	// 左の座標(右)
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

	//右の座標(中心)
	Vector2 headPosR = GetColPos(COL_LR::R, COL_TD::T);

	// 右の座標(左)
	Vector2 corePosR = GetColPos(COL_LR::R, COL_TD::C);

	// 右の座標(右)
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

	// 相対座標を足す
	ret.x += hitPos_.x;
	ret.y += hitPos_.y;

	// 左右
	switch (lr)
	{
	case Player::COL_LR::L:
		ret.x -= hitBox_.x;
		break;
	case Player::COL_LR::R:
		ret.x += hitBox_.x;
		break;
	}

	// 上下
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

	// 攻撃キーを押すと、弾を生成する
	if (InputManager::GetInstance().IsNew(keyConfig_.ATTACK) && stepShotDelay_ <= 0.0f)
	{
		
		// 有効な球を取得する
		Shot* shot = GetValidShot();
		
		// ロックマンの腕の長さ文、座標をずらす
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

		// 弾を生成(弾発射の準備)
		Vector2F dir = AsoUtility::Normalize(dir_);

		float powX = 35.0f;
		float movePowX = dir.x * powX;	// 右20 or 左-20
		pos.x += movePowX;

		// ジャンプしていたら、y-10、していなかったらy+5
		if (isJump_)
		{
			pos.y -= 10;
		}
		if (!isJump_)
		{
			pos.x += 5;
		}

		shot->CreateShot(pos.ToVector2(), dir);

		// 弾の発射後の硬直時間セット
		stepShotDelay_ = SHOT_DELAY;

	}

	atkState_ = ATK_STATE::NONE;
	if (stepShotDelay_ > 0.0f)
	{

		// 弾発射後の硬直時間を減らしていく
		stepShotDelay_ -= 1.0f / SceneManager::DEFAULT_FPS;

		// 弾を撃っている間は、攻撃中とする
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
			// 生存していない弾なので、このインスタンスをリサイクルする(再利用する)
			return shots_[i];
		}
	}

	// 生存していない弾が見つからなかった
	Shot* shot = new Shot(imgShot_);
	shots_.push_back(shot);

	return shot;
}
