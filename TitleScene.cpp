#include <DxLib.h>
#include "Application.h"
#include "SceneManager.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{
	imgTitle_ = LoadGraph("Image/UI/Title.png");
	imgStart_ = LoadGraph("Image/UI/HitStartKey.png");
}

void TitleScene::Update(void)
{
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		// �X�y�[�X�L�[���������ꂽ��A�Q�[���V�[���֑J�ڂ���
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}
}

void TitleScene::Draw(void)
{

	// �w�i
	DrawGraph(0, 0, imgTitle_, true);

	// �X�^�[�g�L�[�摜�̃T�C�Y 337�~53
	// ��ʐ^�񒆂̂�����Ɖ�
	DrawGraph(
		(Application::SCREEN_SIZE_X / 2) - (337 / 2),
		(Application::SCREEN_SIZE_Y / 2) - (53 / 2) + 160,
		imgStart_,
		true
	);

}

void TitleScene::Release(void)
{
	DeleteGraph(imgTitle_);
	DeleteGraph(imgStart_);
}
