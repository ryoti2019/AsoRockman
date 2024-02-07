#include <string>
#include <DxLib.h>
#include "SceneManager.h"
#include "Vector2.h"
#include "Blast.h"

void Blast::Init(COLOR color)
{

	std::string path = "Image/Effect/";

	switch (color)
	{
	case Blast::COLOR::CYAN:
		path += "EffectCyan.png";
		break;
	case Blast::COLOR::RED:
		path += "EffectRed.png";
		break;
	case Blast::COLOR::MAGENTA:
		path += "EffectMagenta.png";
			break;
	case Blast::COLOR::YELLOW:
		path += "EffectYellow.png";
		break;
	case Blast::COLOR::BLACK:
		path += "EffectBlack.png";
		break;
	}

	LoadDivGraph(
		path.c_str(),
		MAX_IMG_TYPE, MAX_IMG_TYPE, 1,
		IMG_SIZE_X, IMG_SIZE_Y, images_);

	// ���q�̏�����
	InitParticle();

}

void Blast::Update(void)
{

	PARTICLE* particle;

	size_t size = particles_.size();
	for (int i = 0; i < size; i++)
	{

		// ���̂̃|�C���^�𔲂��o��(���x���z�񌟍������Ȃ�����)
		particle = &particles_[i];

		// �������Ă��Ȃ���΁A�������΂�
		if (particle->life <= 0.0f)
		{
			continue;
		}

		// �������Ԃ����炷
		particle->life -= 1.0f / SceneManager::DEFAULT_FPS;

		// �ړ�(�����~�X�s�[�h)
		particle->pos.x += particle->dir.x * particle->speed;
		particle->pos.y += particle->dir.y * particle->speed;

	}

}

void Blast::Draw(void)
{

	PARTICLE* particle;

	size_t size = particles_.size();
	for (int i = 0; i < size; i++)
	{

		// ���̂̃|�C���^�𔲂��o��(���x���z�񌟍������Ȃ�����)
		particle = &particles_[i];

		// �������Ă��Ȃ���΁A�`�悵�Ȃ�
		if (particle->life <= 0.0f)
		{
			continue;
		}

		// �ړ�
		Vector2 pos = particle->pos.ToVector2();

		float rate = particle->life / MAX_LIFE;
		if (rate < 0.5f)
		{
			// �������Ԃ��Z���Ȃ�(=����������)=�����ɋ߂Â�
			int value = static_cast<int>(255.0f * rate);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, value);
		}

		// �`��
		DrawRotaGraph(pos.x, pos.y, 1.0, 0.0, images_[particle->type], true);

		// �`�惂�[�h�����ɖ߂�
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	}

}

void Blast::Release(void)
{

	// �摜�������̔j��
	for (int i = 0; i < MAX_IMG_TYPE; i++)
	{
		DeleteGraph(images_[i]);
	}

	// �ϒ��z��̃N���A
	particles_.clear();

}

void Blast::CreateEffect(Vector2F pos)
{
	PARTICLE* particle;

	size_t size = particles_.size();
	for (int i = 0; i < size; i++)
	{

		particle = &particles_[i];

		// �ʒu
		particle->pos = pos;

		// ������Init�Őݒ�ς�

		//�傫��(�����_��0�`5)
		int size = GetRand(static_cast<int>(MAX_IMG_TYPE - 1));
		particle->type = size;

		// �ړ����x(�����_��)
		//MIN_SPEED�`MAX_SPEED
		int speed = MIN_SPEED + GetRand(MAX_SPEED - MIN_SPEED);
		particle->speed = speed;

		// ��������
		particle->life = MAX_LIFE;

	}
}

void Blast::InitParticle(void)
{

	//deg		rad
	//45		0.7854		rad = deg * �� / 180
	//45		0.7854		deg = rad * 180 / ��

	float deg = 0.0f;	// �f�O���[(�x��) 45�x
	float rad = 0.0f;	// ���W�A��(�ʓx) 0.785rad

	// 12�̗��q�����(12�񃋁[�v)
	while (deg < 360.0f)
	{

		// ���q�\���̂̎��Ԃ����
		PARTICLE particle;

		// �f�O���[�����W�A���ɕϊ�
		rad = deg * DX_PI_F / 180.0f;

		// �p�x�������(�x�N�g��)�����߂�
		particle.dir = { cosf(rad), sinf(rad) };

		// �ϒ��z��̒��ɓ���āA��Ő��䂷��
		particles_.push_back(particle);

		// �w��p�x(30�x)���A�i�s����
		deg += SPLIT_ANGLE;

	}

}
