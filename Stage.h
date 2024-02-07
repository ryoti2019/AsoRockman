#pragma once
#include "SceneBase.h"
#include "Vector2.h"

class Stage
{

public:

	// ステージ画像の横の分割数
	static constexpr int NUM_MAP_CHIPS_X = 13;

	// ステージ画像の縦の分割数
	static constexpr int NUM_MAP_CHIPS_Y = 1;

	// ステージ画像の分割総数
	static constexpr int NUM_MAP_CHIPS = NUM_MAP_CHIPS_X * NUM_MAP_CHIPS_Y;

	// マップチップサイズ
	static constexpr int CHIP_SIZE_X = 32;
	static constexpr int CHIP_SIZE_Y = 32;

	// マップのサイズ
	static constexpr int MAP_SIZE_X = 48;
	static constexpr int MAP_SIZE_Y = 15;

	// コンストラクタ
	Stage(void);

	// デストラクタ
	~Stage(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// 外部ファイルから地上のステージデータを読み込む
	void LoadGroundCsvData(void);

	// マップチップ番号を取得
	int GetChipNo(Vector2 mapPos);

private:

	// マップ画像
	int imgMapChips_[NUM_MAP_CHIPS];

	// マップデータ
	int map_[MAP_SIZE_Y][MAP_SIZE_X];

};

