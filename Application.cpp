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

	// アプリケーションの初期設定
	SetWindowText("2216008_寺師遼");

	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	if (DxLib_Init() == -1)
	{
		isError_ = true;
		return;
	}

	// 入力制御初期化
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();

	// シーン管理初期化
	SceneManager::CreateInstance();

}

void Application::Run(void)
{

	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		InputManager::GetInstance().Update();

		SceneManager::GetInstance().Update();

		// デバッグ用の描画前処理
		_dbgStartDraw();

		SceneManager::GetInstance().Draw();

		// デバッグ用の描画処理
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
