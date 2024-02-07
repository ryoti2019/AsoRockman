#pragma once
#include <vector>
#include "Vector2F.h"

class Blast
{

public:

	// �摜�̎��(���q�̑傫�����قȂ�)
	static constexpr int MAX_IMG_TYPE = 6;

	// �摜�T�C�Y��
	static constexpr int IMG_SIZE_X = 16;
	// �摜�T�C�Y�c
	static constexpr int IMG_SIZE_Y = 16;

	// ���q�̊g�U(����)�p�x
	static constexpr float SPLIT_ANGLE = 30.0f;

	// ���q�̐�������
	static constexpr float MAX_LIFE = 0.2f;

	// �ړ����x(�ŏ�)
	static constexpr float MIN_SPEED = 2.0f;

	// �ړ����x(�ő�)
	static constexpr float MAX_SPEED = 8.0f;

	// ���q�̐F
	enum class COLOR {
		CYAN,
		RED,
		MAGENTA,
		YELLOW,
		BLACK,
		MAX
	};

	// ���q�̂P���̏��
	struct PARTICLE {
		Vector2F pos = { 0.0f, 0.0f };
		Vector2F dir = { 0.0f, 0.0f };
		int type = 0;
		float speed = 0.0f;
		float life = 0.0f;
	};

	void Init(COLOR color);
	void Update(void);
	void Draw(void);
	void Release(void);

	void CreateEffect(Vector2F pos);

private:

	// �摜
	int images_[MAX_IMG_TYPE];

	// ���q
	std::vector<PARTICLE> particles_;

	// ���q�̏�����
	void InitParticle(void);

};


