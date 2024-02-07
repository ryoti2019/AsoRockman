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

	// �������ꂽ�摜��ǂݍ���
	LoadDivGraph("Image/Stage/Map.png",
		NUM_MAP_CHIPS,
		NUM_MAP_CHIPS_X,
		NUM_MAP_CHIPS_Y,
		CHIP_SIZE_X,
		CHIP_SIZE_Y,
		&imgMapChips_[0]
	);


	// �O���t�@�C��(CSV)����}�b�v�f�[�^��ǂݍ���
	LoadGroundCsvData();

}

void Stage::Update(void)
{
}

void Stage::Draw(void)
{

	// �}�b�v�\��
	for (int y = 0; y < MAP_SIZE_Y; y++)
	{
		for (int x = 0; x < MAP_SIZE_X; x++)
		{
			// 0 ,1, 2, 3, 4
			DrawGraph(x * CHIP_SIZE_X, y * CHIP_SIZE_Y, imgMapChips_[map_[y][x]], true);

			// �f�o�b�O�p
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

	// �t�@�C���̓ǂݍ���
	std::ifstream ifs = std::ifstream("Data/Map.csv");
	if (!ifs)
	{
		// �G���[������
		return;
	}

	// �n��f�[�^�̏�����
	for (int y = 0; y < MAP_SIZE_Y; y++)
	{
		for (int x = 0; x < MAP_SIZE_X; x++)
		{
			map_[y][x] = -1;
		}
	}

	// �t�@�C����1�s���ǂݍ���ŁAline�Ɋi�[����
	std::string line;					// �P�s�̕�������
	std::vector < std::string> split;	// �P�s�̕�������
	int x = 0;
	int y = 0;
	int chipNo = 0;

	// �t�@�C�����P�s���ǂݍ���
	while (getline(ifs, line))
	{

		// 1�s���̕�������J���}��؂�ŕ���
		split = AsoUtility::Split(line, ',');

		for (int x = 0; x < split.size(); x++)
		{

			// string�^����int�^�ɕϊ�
			chipNo = stoi(split[x]);

			// �Q�����z��Ƀ}�b�v�`�b�v�ԍ����i�[
			map_[y][x] = chipNo;

		}

		y++;
	}

}

int Stage::GetChipNo(Vector2 mapPos)
{
	return map_[mapPos.y][mapPos.x];
}

