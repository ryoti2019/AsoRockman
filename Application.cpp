#include <DxLib.h>
#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Application.h"

Application* Application::instance_ = nullptr;

const std::string Application::PATH_IMG = "Image/";
const std::string Application::PATH_IMG_PLAYER1 = PATH_IMG + "Player/P1/";
const std::string Application::PATH_IMG_PLAYER2 = PATH_IMG + "Player/P2/";

Application::Application(void)
{
	isError_ = false;
}

Application::Application(const Application& ins)
{
}

Application::~Application(void)
{
}

void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::Init(void)
{

	// �A�v���P�[�V�����̏����ݒ�
	SetWindowText("2216008_���t��");

	// �E�B���h�E�T�C�Y
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	if (DxLib_Init() == -1)
	{
		isError_ = true;
		return;
	}

	// ���͐��䏉����
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();

	// �V�[���Ǘ�������
	SceneManager::CreateInstance();

}

void Application::Run(void)
{

	// �Q�[�����[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		InputManager::GetInstance().Update();

		SceneManager::GetInstance().Update();

		// �f�o�b�O�p�̕`��O����
		_dbgStartDraw();

		SceneManager::GetInstance().Draw();

		// �f�o�b�O�p�̕`�揈��
		_dbgDraw();

		ScreenFlip();

	}

}

void Application::Release(void)
{

	if (DxLib_End() == -1)
	{
		isError_ = true;
		return;
	}

}

bool Application::IsError(void)
{
	return isError_;
}
