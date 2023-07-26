#pragma once

#include "defaultmemoryfunctions.h"
#include <functional>

//template <typename T, typename Deleter = std::function<void(T*)>, typename Allocator = std::function<T*(void)>>
template <typename T>
struct CtrlBlock
{
	std::atomic<size_t> Counter;
	std::atomic<size_t> WeakCounter;
	std::function<void(T*)> Deleter;
	std::function<T*(T)> Allocator;

	CtrlBlock(size_t c, size_t wc, std::function<void(T*)> d , std::function<T*(T)> a) :
        Counter(c), WeakCounter(wc), Deleter(d), Allocator(a)
    { }

	CtrlBlock(size_t c = 0, size_t wc = 0) :
		Counter(c),
        WeakCounter(wc),
        Deleter([](T* ptr) { delete ptr; }),
        Allocator([](T val) { return new T(val); })
	{ }

};

template<typename T>
class weak_ptr;

template<class T>
class shared_ptr
{
public:
	typedef T element_type;

	// constructors
	constexpr shared_ptr() noexcept :
		Pointer(nullptr),
		Ctrl(new CtrlBlock<T>(1))
	{ }

	constexpr shared_ptr(T* ptr) noexcept :
		Pointer(ptr),
		Ctrl(new CtrlBlock<T>(1))
	{ }
	shared_ptr(const shared_ptr& obj)
	{
		this->Pointer = obj.Pointer;
		this->Ctrl = obj.Ctrl;

		if (obj.Pointer != nullptr && obj.Ctrl != nullptr)
			obj.Ctrl->Counter++;
	}

	shared_ptr(const weak_ptr<T> & wpt)
	{
		this->Pointer = wpt.Pointer;
		this->Ctrl = wpt.Ctrl;

		this->Ctrl->Counter++;
	}

	// destructor:
	~shared_ptr()
	{
		clean();
	}

	shared_ptr& operator=(const shared_ptr& other) noexcept
	{
		if (this == &other)
			return *this;
		clean();

		Pointer = other.Pointer;
		Ctrl = other.Ctrl;
		Ctrl->Counter++;

		return *this;
	}

	void swap(shared_ptr& other) noexcept
	{
		std::swap(Pointer, other.Pointer);
		std::swap(Ctrl, other.Ctrl);
	}
	void reset() noexcept
	{
		clean();
	}
	void reset(T* ptr)
	{
		clean();
		Pointer = ptr;
		Ctrl = new CtrlBlock<T>(1);
	}

	T* get() const noexcept { return Pointer; }
	T& operator*() const noexcept { return *Pointer; }
	T* operator->() const noexcept { return Pointer; }

private:
	T* Pointer;
	CtrlBlock<T>* Ctrl;
	

	void clean()
	{
		Ctrl->Counter--;
		if (Ctrl->Counter <= 0 && Ctrl->WeakCounter <= 0)
		{
			if (Pointer != nullptr)
			{
				Ctrl->Deleter(Pointer);
				Pointer = nullptr;
			}

			if (Ctrl != nullptr)
			{
                delete Ctrl;
				Ctrl = nullptr;
            }
		}
	    
	}

	friend class weak_ptr<T>;
};

