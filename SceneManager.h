#pragma once
#include <chrono>
#include <DxLib.h>
class Fader;
class SceneBase;

class SceneManager
{

public:

	static constexpr float DEFAULT_FPS = 60.0f;

	// �V�[���Ǘ��p
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME,
		GAMEOVER,
	};

	// �C���X�^���X�̐���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static SceneManager& GetInstance(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// �V�[���J�ږ���
	void ChangeScene(SCENE_ID nextId);

private:

	// �ÓI�C���X�^���X
	static SceneManager* instance_;

	SCENE_ID sceneId_;
	SCENE_ID waitSceneId_;

	// �t�F�[�h
	Fader* fader_;

	// �e��V�[��
	SceneBase* scene_;

	// �V�[���J�ڒ�����
	bool isSceneChanging_;
	
	// �R���X�g���N�^
	SceneManager(void);

	// �f�X�g���N�^
	~SceneManager(void);

	// �V�[���J��
	void DoChangeScene(SCENE_ID sceneId);

	// �t�F�[�h����
	void Fade(void);

};