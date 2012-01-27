#pragma once
#include <memory>
#include "../MXLib/MXAnimation.h"

class GraphicResources
{
public:
	GraphicResources();

	void Init(MX::Draw &draw);


	std::shared_ptr<MX::Animation> snake_head;
	std::shared_ptr<MX::Animation> snake_body;

	std::shared_ptr<MX::Animation> flower1;
	std::shared_ptr<MX::Animation> flower2;

	std::shared_ptr<MX::Animation> background;

};

extern GraphicResources GraphicRes;