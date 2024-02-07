#pragma once
#include <string>
#include <vector>
#include "Vector2.h"
#include "Vector2F.h"

class AsoUtility
{

public:

	// 移動方向
	enum class DIR
	{
		UP,
		RIGHT,
		DOWN,
		LEFT,
		MAX
	};

	// 文字列の分割
	static std::vector<std::string> Split(std::string& line, char delimiter);

	// 四捨五入
	static int Round(float value);

	// 四捨五入
	static Vector2 Round(Vector2F value);

	// 四捨五入
	static Vector2 Round(float x, float y);

	// 単位ベクトルに変換
	static Vector2F Normalize(AsoUtility::DIR dir);

};

