#include "pch.h"
#include "UpdateList.h"
#include "IUpdateWithTime.h"

void LGG::UpdateList::update(GameTimer& timer)
{
	this->forEachAndClean(
		[&timer](IUpdateWithTime& o) {
			o.update(timer);
		}
	);
}
