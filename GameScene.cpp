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
	// ステージの初期化
	stage_ = new Stage();
	stage_->Init();

	// プレイヤー1の初期化
	player_[0] = new Player();
	Player::KEY_CONFIG keyP1 = {
		KEY_INPUT_UP, KEY_INPUT_DOWN, KEY_INPUT_LEFT,
		KEY_INPUT_RIGHT, KEY_INPUT_N, KEY_INPUT_M
	};
	player_[0]->Init(this, Player::TYPE::PLAYER_1, keyP1);

	// プレイヤー2の初期化
	player_[1] = new Player();
	Player::KEY_CONFIG keyP2 = {
		KEY_INPUT_W, KEY_INPUT_S, KEY_INPUT_A,
		KEY_INPUT_D, KEY_INPUT_LCONTROL, KEY_INPUT_LSHIFT
	};
	player_[1]->Init(this, Player::TYPE::PLAYER_2, keyP2);

}

void GameScene::Update(void)
{

	// ステージの更新
	stage_->Update();

	// プレイヤーの更新
	player_[0]->Update();
	player_[1]->Update();

	// 弾の更新
	auto shots = player_[0]->GetShots();

	for (auto shot : shots)	// 範囲for文
	{
		shot->Update();
	}

	// 弾の更新
	auto shots2 = player_[1]->GetShots();

	for (auto shot : shots2)	// 範囲for文
	{
		shot->Update();
	}

	//std::vector<Shot*> shots = player_->GetShots();

	//for (Shot* shot : shots)
	//{
	//	shot->Update();
	//}

	// 衝突判定
	Collision();


}

void GameScene::Draw(void)
{
	// ステージの描画
	stage_->Draw();

	// プレイヤーの描画
	player_[0]->Draw();
	player_[1]->Draw();

	// 弾の描画
	auto shots = player_[0]->GetShots();

	for (auto shot : shots)	// 範囲for文
	{
		shot->Draw();
	}

	// 弾の描画
	auto shots2 = player_[1]->GetShots();

	for (auto shot : shots2)	// 範囲for文
	{
		shot->Draw();
	}

}

void GameScene::Release(void)
{
	// ステージ開放
	stage_->Release();
	delete stage_;

	// プレイヤー開放
	player_[0]->Release();
	delete player_[0];

	// プレイヤー開放
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

	// ワールド座標からマップ座標に変換する
	Vector2 mapPos = World2MapPos(worldPos);

	// マップ座標からマップチップ番号を取得する
	int chipNo = stage_->GetChipNo(mapPos);

	// マップチップが8以上の場合は、衝突する
	if (chipNo >= 8)
	{
		return true;
	}

	return false;

}

void GameScene::Collision(void)
{

	// 弾情報を取得する
	std::vector<Shot*> shots = player_[0]->GetShots();
	for (Shot* shot : shots)
	{

		// 弾が生存していなければ、処理を飛ばす
		if (!shot->IsAlive())
		{
			continue;
		}

		// 弾とステージの衝突判定
		Vector2 shotPos = shot->GetPos().ToVector2();

		if (IsCollisionStage(shotPos))
		{
			// 爆発エフェクトを発生させる
			shot->ShotBlast();
		}
	}

	// 弾情報を取得する
	std::vector<Shot*> shots2 = player_[1]->GetShots();
	for (Shot* shot : shots2)
	{

		// 弾が生存していなければ、処理を飛ばす
		if (!shot->IsAlive())
		{
			continue;
		}

		// 弾とステージの衝突判定
		Vector2 shotPos = shot->GetPos().ToVector2();

		if (IsCollisionStage(shotPos))
		{
			// 爆発エフェクトを発生させる
			shot->ShotBlast();
		}
	}

}
