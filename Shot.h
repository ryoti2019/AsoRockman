#pragma once
#include "AsoUtility.h"
#include "Vector2F.h"
class Player;
class Blast;

class Shot
{

public:

	// 弾のサイズ
	static constexpr int IMG_SIZE_X = 16;
	static constexpr int IMG_SIZE_Y = 16;

	// 弾の生存時間
	static constexpr float SHOT_ALIVE = 1.5f;

	// コンストラクタ(弾画像のハンドルID)
	Shot(int img);

	// デストラクタ
	~Shot(void);

	// 弾の生成(弾の進行方向、表示開始座標)
	void CreateShot(Vector2 pos, Vector2F dir);

	// 更新ステップ
	void Update(void);

	// 描画
	void Draw(void);

	// 開放処理
	void Release(void);

	// 生存判定
	bool IsAlive(void);

	// 生存時間の設定
	void SetAlive(float time);

	// 弾の座標の取得
	Vector2F GetPos(void);

	// 弾を爆発させる
	void ShotBlast(void);

private:

	// 爆発エフェクト
	Blast* blast_;

	// ショット画像
	int img_;

	// 方向
	Vector2F dir_;

	// 弾の座標
	Vector2F pos_;

	// 弾の移動速度
	float speed_;

	// 弾の生存時間計算用
	float stepAlive_;

};

