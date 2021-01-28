#pragma once

namespace LGG
{

template<typename T>
class WeakPtrList : public std::list<std::weak_ptr<T>>
{
public:
    template<typename Func>
    auto forEachAndClean(Func function) {
        for (auto iter = this->begin(); iter != this->end(); ) {
            auto sp = iter->lock();
            if (sp != nullptr) {
                function(*sp);
                ++iter;
            }
            else {
                iter = this->erase(iter);
            }
        }
    }
};

}