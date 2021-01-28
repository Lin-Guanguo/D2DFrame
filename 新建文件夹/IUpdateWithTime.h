#pragma once

namespace LGG
{

class GameTimer;

class IUpdateWithTime
{
public:
	virtual void update(const GameTimer& timer) = 0;

};

}