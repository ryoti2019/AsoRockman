#pragma once
#include "Vector2F.h"
#include "AsoUtility.h"

class GameScene;
class Shot;

class Player
{

public:

	static const int SIZE_X = 96; // 横サイズ
	static const int SIZE_Y = 64; // 縦サイズ

	// 最大アニメーション数(走るモーションのみ)
	static const int MAX_NUM_ANIM = 4;

	// アニメーションスピード
	static constexpr float ANIM_SPEED = 0.1f;

	// 移動速度の最大値
	static constexpr float MAX_MOVE_SPEED = 3.5f;

	// 加速(accelerator=アクセレレーター)
	static constexpr float MOVE_ACC = 0.25f;

	// 減速(decelerate=ディセラレイト)
	static constexpr float MOVE_DEC = 0.05f;

	// 最大ジャンプ力
	static constexpr float MAX_JUMP_POW = 8.5f;

	// 重力
	static constexpr float GRAVITY = 0.25f;

	// ジャンプキー入力を受け付けるフレーム数
	static constexpr int INPUT_JUMP_FRAME = 6;

	// 弾発射後の硬直時間
	static constexpr float SHOT_DELAY = 1.0f;

	// アニメーション状態
	enum class ANIM_STATE
	{
		IDLE,
		RUN,
		JUMP,
		DAMAGED,
		MAX
	};

	// 攻撃状態
	enum class ATK_STATE
	{
		NONE,
		SHOT,
		MAX
	};

	// 衝突座標(左右)
	enum class COL_LR
	{
		C,
		L,
		R
	};

	// 衝突座標(上下)
	enum class COL_TD
	{
		C,
		T,
		D
	};

	// プレイヤー
	enum class TYPE
	{
		PLAYER_1,
		PLAYER_2
	};

	// キーコンフィグ
	struct KEY_CONFIG
	{
		int UP;
		int DOWN;
		int LEFT;
		int RIGHT;
		int ATTACK;
		int JUMP;
	};

	// コンストラクタ
	Player(void);

	// デストラクタ
	~Player(void);

	void Init(GameScene* scene, TYPE type, KEY_CONFIG config);
	void Update(void);
	void Draw(void);
	void Release(void);

	// 弾の取得
	std::vector<Shot*> GetShots(void);

private:

	// ゲームシーンのポインタ変数
	GameScene* gameScene_;

	// 状態ごとの画像(モーション、攻撃、アニメーション数)
	//int images_[4][2][4];

	int images_[static_cast<int>(ANIM_STATE::MAX)]
		[static_cast<int>(ATK_STATE::MAX)][MAX_NUM_ANIM];
	
	// 座標
	Vector2F pos_;

	// アニメーション状態
	ANIM_STATE animState_;

	// 攻撃状態
	ATK_STATE atkState_;

	// アニメーションカウンタ
	float stepAnim_;

	// プレイヤーの向き
	AsoUtility::DIR dir_;

	// 移動速度
	float moveSpeed_;

	// ジャンプ力
	float jumpPow_;

	// ジャンプ中判定
	bool isJump_;

	// ジャンプキーの押下判定
	bool isPutJumpKey_;

	// ジャンプキー入力カウンタ
	int cntJumpInput_;

	// 衝突判定用：中心座標(pos_からの相対座標)
	Vector2 hitPos_;

	// 衝突判定用：範囲
	Vector2 hitBox_;

	// ショット
	std::vector<Shot*> shots_;

	// プレイヤー種別
	TYPE type_;

	// キー設定
	KEY_CONFIG keyConfig_;

	// 弾画像
	int imgShot_;

	// 弾発射後の硬直時間計算用
	float stepShotDelay_;

	// 画像の読み込み
	void LoadImages(void);

	// プレイヤーの描画
	void DrawPlayer(int handleId);

	// デバッグの描画
	void DrawDebug(void);

	// プレイヤーの移動操作
	void ProcessMove(void);

	// 移動(実際の座標移動)
	void Move(void);

	// 加速(スピードを加える)
	void Accele(float speed);

	// 原則(スピードを減少させる)
	void Decelerate(float speed);

	// プレイヤーのジャンプ操作
	void ProcessJump(void);

	// 移動(実際の座標移動)
	void Jump(void);

	// ジャンプ力の設定
	void SetJumpPow(float pow);

	// 重力をかける
	void AddGravity(void);

	// 足元の衝突判定
	void CollisionFoot(void);

	// 頭の衝突判定
	void CollisionHead(void);

	// 左右の衝突判定
	void CollisionSide(void);

	// 衝突判定を取得する
	Vector2 GetColPos(COL_LR lr, COL_TD td);

	// 攻撃
	void Attack(void);

	// 有効な弾を取得する
	Shot* GetValidShot(void);
};

