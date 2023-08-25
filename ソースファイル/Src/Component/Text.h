//@file Text.h

#ifndef COMPONENT_TEXT_H_INCLUDED
#define COMPONENT_TEXT_H_INCLUDED
#include "../Application/Component.h"
#include "../Application/GameObject.h"
#include <string>

//テキストコンポーネント

class Text :public Component
{
public:
	Text() = default;
	virtual ~Text() = default;

	//更新処理
	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		gameObject.Scale = scale;
		const float fontSizeX = 16 * scale;
		const float fontSizeY = 32 * scale;

		gameObject.spriteList.clear();

		for (size_t i = 0; i < std::size(text); ++i)
		{
			const char c = text[i];
			gameObject.AddSprite({
				static_cast<float>(c % 16) / 16,
				static_cast<float>(c / 16) / 8,
				1.0f / 16.0f,1.0f / 8.0f }, fontSizeX * (0.5f + static_cast<float>(i)),
				fontSizeY * 0.5f, scale);
			
		}
	}

	//文字を設定
	void SetText(const std::string& t, float s = 1)
	{
		text = t;
		scale = s;
	}

	std::string text;
	float scale = 1;

};


#endif  COMPONENT_TEXT_H_INCLUDED
