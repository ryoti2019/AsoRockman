#pragma once
#include "SceneBase.h"
#include "Vector2.h"

class Stage;
class Player;

class GameScene : public SceneBase
{

public:

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	// ワールド(スクリーン)座標をマップ座標に変換する
	Vector2 World2MapPos(Vector2 worldPos);

	// マップチップを使用した衝突判定
	bool IsCollisionStage(Vector2 worldPos);

private:

	// ステージ
	Stage* stage_;

	// プレイヤー
	Player* player_[2];

	// 衝突判定
	void Collision(void);

};

