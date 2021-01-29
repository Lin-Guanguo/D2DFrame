#pragma once
#include "WeakPtrList.h"

namespace LGG
{

class IUpdateWithTime;
class GameTimer;

class UpdateList : public WeakPtrList<IUpdateWithTime>
{
public:
	void update(GameTimer& timer);

};

}
