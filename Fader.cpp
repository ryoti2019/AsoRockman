#include <DxLib.h>
#include "Application.h"
#include "Fader.h"

Fader::Fader(void)
{
	state_ = STATE::NONE;
	alpha_ = 0.0f;
	isPreEnd_ = true;
	isEnd_ = true;
}

Fader::~Fader(void)
{
}

void Fader::Init(void)
{
	state_ = STATE::NONE;
	alpha_ = 0.0f;
	isPreEnd_ = true;
	isEnd_ = true;
}

void Fader::Update(void)
{

	if (isEnd_)
	{
		// �t�F�[�h�������I�����Ă����牽�����Ȃ�
		return;
	}
	
	if (isPreEnd_)
	{
		// 1�t���[����(Draw��)�ɏI���Ƃ���
		isEnd_ = true;
		return;
	}

	switch (state_)
	{

	case STATE::NONE:
		return;

	case STATE::FADE_OUT:
		alpha_ += SPEED_ALPHA;
		if (alpha_ > 255)
		{
			// �t�F�[�h�I��
			alpha_ = 255;
			isPreEnd_ = true;
		}
		break;

	case STATE::FADE_IN:
		alpha_ -= SPEED_ALPHA;
		if (alpha_ < 0)
		{
			// �t�F�[�h�I��
			alpha_ = 0;
			isPreEnd_ = true;
		}
		break;

	default:
		return;

	}

}

void Fader::Draw(void)
{

	switch (state_)
	{
	case STATE::NONE:
		return;
	case STATE::FADE_OUT:
	case STATE::FADE_IN:
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha_));
		DrawBox(
			0, 0,
			Application::SCREEN_SIZE_X,
			Application::SCREEN_SIZE_Y,
			0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		break;
	}

}

Fader::STATE Fader::GetState(void)
{
	return state_;
}

bool Fader::IsEnd(void)
{
	return isEnd_;
}

void Fader::SetFade(STATE state)
{
	state_ = state;
	if (state_ != STATE::NONE)
	{
		isPreEnd_ = false;
		isEnd_ = false;
	}
}
