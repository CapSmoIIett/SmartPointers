#pragma once

#include "defaultmemoryfunctions.h"
#include <functional>

//template <typename T, typename Deleter = std::function<void(T*)>, typename Allocator = std::function<T*(void)>>
struct CtrlBlock
{
	std::atomic<size_t> Counter;
	std::atomic<size_t> WeakCounter;

	CtrlBlock(size_t c = 0, size_t wc = 0) :
		Counter(c),
        WeakCounter(wc)
	{ }
};


bool operator==(const CtrlBlock& left, const CtrlBlock& right)
{
	return left.Counter == right.Counter &&
		left.WeakCounter == right.WeakCounter;//&&
		//left.Deleter == right.Deleter &&
		//left.Allocator == right.Allocator;
}

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
		Ctrl(new CtrlBlock(1))
	{ }

	constexpr shared_ptr(T* ptr) noexcept :
		Pointer(ptr),
		Ctrl(new CtrlBlock(1))
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
		Ctrl = new CtrlBlock(1);
	}

	T* get() const noexcept { return Pointer; }
	T& operator*() const noexcept { return *Pointer; }
	T* operator->() const noexcept { return Pointer; }



private:
	T* Pointer;
	CtrlBlock* Ctrl;
	

	void clean()
	{
		if (Ctrl == nullptr && Pointer == nullptr)
			return;

		Ctrl->Counter--;
		if (Ctrl->Counter <= 0 && Ctrl->WeakCounter <= 0)
		{
			if (Pointer != nullptr)
			{
				delete Pointer;
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

	template<typename A, typename B>
	friend bool operator==(const shared_ptr<A>& left, const shared_ptr<B>& right);
	template<typename T, typename U>
	friend shared_ptr<T> static_pointer_cast(shared_ptr<U> const& spt) noexcept;
    template<class T, class U>
	friend shared_ptr<T> dynamic_pointer_cast(shared_ptr<U> const& spt) noexcept;
};


template<class T, class U>
shared_ptr<T> static_pointer_cast(shared_ptr<U> const& spt) noexcept
{
	auto p = static_cast<typename T*>(spt.get());
    shared_ptr<T> ptr;
    ptr.Pointer = p;
    ptr.Ctrl = spt.Ctrl;
    ptr.Ctrl->Counter++;

    return ptr;
}

template<class T, class U>
shared_ptr<T> dynamic_pointer_cast(shared_ptr<U> const& spt) noexcept
{
	if (auto p = dynamic_cast<typename T*>(spt.get()))
	{
		shared_ptr<T> ptr;
        ptr.Pointer = p;
        ptr.Ctrl = spt.Ctrl;
        ptr.Ctrl->Counter++;

		return ptr;
	}
	else
		return shared_ptr<T>();
}

template<class T, class U>
shared_ptr<T> const_pointer_cast(shared_ptr<U> const& spt) noexcept
{
	auto p = const_cast<typename T*>(spt.get());
	return std::shared_ptr<T>(p);
}

template<typename A, typename B>
bool operator==(const shared_ptr<A>& left, const shared_ptr<B>& right)
{
	return left.Pointer == right.Pointer &&
        *(left.Ctrl) == *(right.Ctrl);
}
