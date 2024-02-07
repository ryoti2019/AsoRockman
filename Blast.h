#pragma once
#include <vector>
#include "Vector2F.h"

class Blast
{

public:

	// 画像の種類(粒子の大きさが異なる)
	static constexpr int MAX_IMG_TYPE = 6;

	// 画像サイズ横
	static constexpr int IMG_SIZE_X = 16;
	// 画像サイズ縦
	static constexpr int IMG_SIZE_Y = 16;

	// 粒子の拡散(分割)角度
	static constexpr float SPLIT_ANGLE = 30.0f;

	// 粒子の生存時間
	static constexpr float MAX_LIFE = 0.2f;

	// 移動速度(最小)
	static constexpr float MIN_SPEED = 2.0f;

	// 移動速度(最大)
	static constexpr float MAX_SPEED = 8.0f;

	// 粒子の色
	enum class COLOR {
		CYAN,
		RED,
		MAGENTA,
		YELLOW,
		BLACK,
		MAX
	};

	// 粒子の１つずつの情報
	struct PARTICLE {
		Vector2F pos = { 0.0f, 0.0f };
		Vector2F dir = { 0.0f, 0.0f };
		int type = 0;
		float speed = 0.0f;
		float life = 0.0f;
	};

	void Init(COLOR color);
	void Update(void);
	void Draw(void);
	void Release(void);

	void CreateEffect(Vector2F pos);

private:

	// 画像
	int images_[MAX_IMG_TYPE];

	// 粒子
	std::vector<PARTICLE> particles_;

	// 粒子の初期化
	void InitParticle(void);

};


