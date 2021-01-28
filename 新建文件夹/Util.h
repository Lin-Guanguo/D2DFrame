#pragma once

namespace LGG
{

template <class P>
void SafeRelease(P*& pT)
{
	if (pT) {
        pT->Release();
        pT = NULL;
	}
}

class Direct2DError
{
	HRESULT hr_;
	PCWCHAR message_;
public:
	Direct2DError(HRESULT hr, PCWCHAR message = L"NonMessage") : hr_(hr), message_(message) {}

	PCWCHAR what() const { return message_; }

	HRESULT hresult() const { return hr_; }
};

#define THROW_ON_FAILED(hr) { if (FAILED(hr)) { throw LGG::Direct2DError{ hr }; } }
#define THROW_ON_FAILED(hr, message) { if (FAILED(hr)) { throw LGG::Direct2DError{ hr, message }; } }

namespace Format 
{
    static void concat(int i, std::wstring& str) { str.append(std::to_wstring(i)); }
    static void concat(long i, std::wstring& str) { str.append(std::to_wstring(i)); }
    static void concat(long long i, std::wstring& str) { str.append(std::to_wstring(i)); }
    static void concat(unsigned int i, std::wstring& str) { str.append(std::to_wstring(i)); }
    static void concat(unsigned long i, std::wstring& str) { str.append(std::to_wstring(i)); }
    static void concat(unsigned long long i, std::wstring& str) { str.append(std::to_wstring(i)); }
    static void concat(double i, std::wstring& str) { str.append(std::to_wstring(i)); }
    static void concat(const wchar_t* cstr, std::wstring& str) { str.append(cstr); }
    static void concat(std::wstring_view cstr, std::wstring& str) { str.append(cstr); }
    static void concat(void* ptr, std::wstring& str) { str.append(std::to_wstring(reinterpret_cast<__int64>(ptr))); }

    template<typename... T>
    static std::wstring concatTowstring(T... args)
    {
        std::wstring s;
        constexpr int AVERAGE_ARG_LEN = 16;
        s.reserve(AVERAGE_ARG_LEN * sizeof...(args));
        (concat(args, s), ...);
        return std::move(s);
    }
}

template<typename... T>
void DebugOutPut(T... args)
{
    OutputDebugString(Format::concatTowstring(std::forward<T>(args)..., L"\n").data());
}

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

