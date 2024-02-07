#pragma once
#include <chrono>
#include <DxLib.h>
class Fader;
class SceneBase;

class SceneManager
{

public:

	static constexpr float DEFAULT_FPS = 60.0f;

	// シーン管理用
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME,
		GAMEOVER,
	};

	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static SceneManager& GetInstance(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// シーン遷移命令
	void ChangeScene(SCENE_ID nextId);

private:

	// 静的インスタンス
	static SceneManager* instance_;

	SCENE_ID sceneId_;
	SCENE_ID waitSceneId_;

	// フェード
	Fader* fader_;

	// 各種シーン
	SceneBase* scene_;

	// シーン遷移中判定
	bool isSceneChanging_;
	
	// コンストラクタ
	SceneManager(void);

	// デストラクタ
	~SceneManager(void);

	// シーン遷移
	void DoChangeScene(SCENE_ID sceneId);

	// フェード処理
	void Fade(void);

};