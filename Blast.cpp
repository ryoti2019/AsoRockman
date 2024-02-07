#include <string>
#include <DxLib.h>
#include "SceneManager.h"
#include "Vector2.h"
#include "Blast.h"

void Blast::Init(COLOR color)
{

	std::string path = "Image/Effect/";

	switch (color)
	{
	case Blast::COLOR::CYAN:
		path += "EffectCyan.png";
		break;
	case Blast::COLOR::RED:
		path += "EffectRed.png";
		break;
	case Blast::COLOR::MAGENTA:
		path += "EffectMagenta.png";
			break;
	case Blast::COLOR::YELLOW:
		path += "EffectYellow.png";
		break;
	case Blast::COLOR::BLACK:
		path += "EffectBlack.png";
		break;
	}

	LoadDivGraph(
		path.c_str(),
		MAX_IMG_TYPE, MAX_IMG_TYPE, 1,
		IMG_SIZE_X, IMG_SIZE_Y, images_);

	// 粒子の初期化
	InitParticle();

}

void Blast::Update(void)
{

	PARTICLE* particle;

	size_t size = particles_.size();
	for (int i = 0; i < size; i++)
	{

		// 実体のポインタを抜き出す(何度も配列検索させないため)
		particle = &particles_[i];

		// 生存していなければ、処理を飛ばす
		if (particle->life <= 0.0f)
		{
			continue;
		}

		// 生存時間を減らす
		particle->life -= 1.0f / SceneManager::DEFAULT_FPS;

		// 移動(方向×スピード)
		particle->pos.x += particle->dir.x * particle->speed;
		particle->pos.y += particle->dir.y * particle->speed;

	}

}

void Blast::Draw(void)
{

	PARTICLE* particle;

	size_t size = particles_.size();
	for (int i = 0; i < size; i++)
	{

		// 実体のポインタを抜き出す(何度も配列検索させないため)
		particle = &particles_[i];

		// 生存していなければ、描画しない
		if (particle->life <= 0.0f)
		{
			continue;
		}

		// 移動
		Vector2 pos = particle->pos.ToVector2();

		float rate = particle->life / MAX_LIFE;
		if (rate < 0.5f)
		{
			// 生存時間が短くなる(=率が下がる)=透明に近づく
			int value = static_cast<int>(255.0f * rate);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, value);
		}

		// 描画
		DrawRotaGraph(pos.x, pos.y, 1.0, 0.0, images_[particle->type], true);

		// 描画モードを元に戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	}

}

void Blast::Release(void)
{

	// 画像メモリの破棄
	for (int i = 0; i < MAX_IMG_TYPE; i++)
	{
		DeleteGraph(images_[i]);
	}

	// 可変長配列のクリア
	particles_.clear();

}

void Blast::CreateEffect(Vector2F pos)
{
	PARTICLE* particle;

	size_t size = particles_.size();
	for (int i = 0; i < size; i++)
	{

		particle = &particles_[i];

		// 位置
		particle->pos = pos;

		// 方向はInitで設定済み

		//大きさ(ランダム0～5)
		int size = GetRand(static_cast<int>(MAX_IMG_TYPE - 1));
		particle->type = size;

		// 移動速度(ランダム)
		//MIN_SPEED～MAX_SPEED
		int speed = MIN_SPEED + GetRand(MAX_SPEED - MIN_SPEED);
		particle->speed = speed;

		// 生存時間
		particle->life = MAX_LIFE;

	}
}

void Blast::InitParticle(void)
{

	//deg		rad
	//45		0.7854		rad = deg * π / 180
	//45		0.7854		deg = rad * 180 / π

	float deg = 0.0f;	// デグリー(度数) 45度
	float rad = 0.0f;	// ラジアン(弧度) 0.785rad

	// 12個の粒子を作る(12回ループ)
	while (deg < 360.0f)
	{

		// 粒子構造体の実態を作る
		PARTICLE particle;

		// デグリーをラジアンに変換
		rad = deg * DX_PI_F / 180.0f;

		// 角度から方向(ベクトル)を求める
		particle.dir = { cosf(rad), sinf(rad) };

		// 可変長配列の中に入れて、後で制御する
		particles_.push_back(particle);

		// 指定角度(30度)分、進行する
		deg += SPLIT_ANGLE;

	}

}
