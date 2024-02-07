#pragma once
#include <string>
class Application
{

public:

	// �X�N���[���T�C�Y
	static constexpr int SCREEN_SIZE_X = 768;
	static constexpr int SCREEN_SIZE_Y = 480;

	// �摜�Ȃǂ̃f�[�^�p�X�֘A
	static const std::string PATH_IMG;
	static const std::string PATH_IMG_PLAYER1;
	static const std::string PATH_IMG_PLAYER2;

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static Application& GetInstance(void);

	// ������
	void Init(void);

	// �Q�[�����[�v�̊J�n
	void Run(void);

	// ���\�[�X�̉��
	void Release(void);

	// �G���[����
	bool IsError(void);

private:

	// �ÓI�C���X�^���X
	static Application* instance_;

	// �R���X�g���N�^
	Application(void);

	// �R�s�[�R���X�g���N�^
	Application(const Application& ins);

	// �f�X�g���N�^
	~Application(void);

	// �G���[����
	bool isError_;

};