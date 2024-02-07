#include<string>
#include <fstream>
#include <sstream>
#include <DxLib.h>
#include "AsoUtility.h"
#include "GameScene.h"
#include "Stage.h"

Stage::Stage(void)
{
}

Stage::~Stage(void)
{
}

void Stage::Init(void)
{

	// 分割された画像を読み込む
	LoadDivGraph("Image/Stage/Map.png",
		NUM_MAP_CHIPS,
		NUM_MAP_CHIPS_X,
		NUM_MAP_CHIPS_Y,
		CHIP_SIZE_X,
		CHIP_SIZE_Y,
		&imgMapChips_[0]
	);


	// 外部ファイル(CSV)からマップデータを読み込む
	LoadGroundCsvData();

}

void Stage::Update(void)
{
}

void Stage::Draw(void)
{

	// マップ表示
	for (int y = 0; y < MAP_SIZE_Y; y++)
	{
		for (int x = 0; x < MAP_SIZE_X; x++)
		{
			// 0 ,1, 2, 3, 4
			DrawGraph(x * CHIP_SIZE_X, y * CHIP_SIZE_Y, imgMapChips_[map_[y][x]], true);

			// デバッグ用
			if (y == 13)
			{
				int tmpX = (x * CHIP_SIZE_X);
				int tmpY = (y * CHIP_SIZE_Y);
				DrawBox(tmpX - 3, tmpY - 3, tmpX + 3, tmpY + 3, 0xff0000, true);
			}
		}
	}
}

void Stage::Release(void)
{

	for (int i = 0; i < NUM_MAP_CHIPS; i++)
	{
		DeleteGraph(imgMapChips_[i]);
	}

}

void Stage::LoadGroundCsvData(void)
{

	// ファイルの読み込み
	std::ifstream ifs = std::ifstream("Data/Map.csv");
	if (!ifs)
	{
		// エラーが発生
		return;
	}

	// 地上データの初期化
	for (int y = 0; y < MAP_SIZE_Y; y++)
	{
		for (int x = 0; x < MAP_SIZE_X; x++)
		{
			map_[y][x] = -1;
		}
	}

	// ファイルを1行ずつ読み込んで、lineに格納する
	std::string line;					// １行の文字列情報
	std::vector < std::string> split;	// １行の文字列情報
	int x = 0;
	int y = 0;
	int chipNo = 0;

	// ファイルを１行ずつ読み込む
	while (getline(ifs, line))
	{

		// 1行分の文字列をカンマ区切りで分割
		split = AsoUtility::Split(line, ',');

		for (int x = 0; x < split.size(); x++)
		{

			// string型からint型に変換
			chipNo = stoi(split[x]);

			// ２次元配列にマップチップ番号を格納
			map_[y][x] = chipNo;

		}

		y++;
	}

}

int Stage::GetChipNo(Vector2 mapPos)
{
	return map_[mapPos.y][mapPos.x];
}

