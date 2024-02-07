#pragma once
#include <string>
#include <vector>
#include "Vector2.h"
#include "Vector2F.h"

class AsoUtility
{

public:

	// �ړ�����
	enum class DIR
	{
		UP,
		RIGHT,
		DOWN,
		LEFT,
		MAX
	};

	// ������̕���
	static std::vector<std::string> Split(std::string& line, char delimiter);

	// �l�̌ܓ�
	static int Round(float value);

	// �l�̌ܓ�
	static Vector2 Round(Vector2F value);

	// �l�̌ܓ�
	static Vector2 Round(float x, float y);

	// �P�ʃx�N�g���ɕϊ�
	static Vector2F Normalize(AsoUtility::DIR dir);

};

