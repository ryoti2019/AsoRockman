#pragma once
#include <string>
class Application
{

public:

	// スクリーンサイズ
	static constexpr int SCREEN_SIZE_X = 768;
	static constexpr int SCREEN_SIZE_Y = 480;

	// 画像などのデータパス関連
	static const std::string PATH_IMG;
	static const std::string PATH_IMG_PLAYER1;
	static const std::string PATH_IMG_PLAYER2;

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static Application& GetInstance(void);

	// 初期化
	void Init(void);

	// ゲームループの開始
	void Run(void);

	// リソースの解放
	void Release(void);

	// エラー判定
	bool IsError(void);

private:

	// 静的インスタンス
	static Application* instance_;

	// コンストラクタ
	Application(void);

	// コピーコンストラクタ
	Application(const Application& ins);

	// デストラクタ
	~Application(void);

	// エラー判定
	bool isError_;

};