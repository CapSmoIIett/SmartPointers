#pragma once

#include "defaultmemoryfunctions.h"
#include <functional>

//template <typename T, typename Deleter = std::function<void(T*)>, typename Allocator = std::function<T*(void)>>
template <typename T>
struct CtrlBlock
{
	std::atomic<size_t> Counter;
	int WeakCounter;
	std::function<void(T*)> Deleter;
	std::function<T*(T)> Allocator;

	CtrlBlock(int c, int wc, std::function<void(T*)> d , std::function<T*(T)> a) :
        Counter(c), WeakCounter(wc), Deleter(d), Allocator(a)
		{ }

	CtrlBlock(int c = 0, int wc = 0) :
		Counter(c), WeakCounter(wc), Deleter([](T* ptr) { delete ptr; }), Allocator([](T val) { return new T(val);  })
	{ }

};


template<class T>
class shared_ptr
{
public:
	typedef T element_type;

	// constructors:
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

	/*
	template <typename Y, typename D>
	share_ptr(Y* ptr, D d)
	{
		this->Pointer = ptr;
		this->Ctrl = new CtrlBlock<T>(1) ;
	}
	//*/
	/*
	explicit shared_ptr(const weak_ptr<T>& wpt)
	{
		this->Pointer = wpt.Pointer;
		this->Ctrl = wpt.Ctrl;
	}
	/*
	template<class Y> explicit shared_ptr(Y* p);
	template<class Y, class D> shared_ptr(Y* p, D d);
	template<class Y, class D, class A> shared_ptr(Y* p, D d, A a);
	template <class D> shared_ptr(nullptr_t p, D d);
	template <class D, class A> shared_ptr(nullptr_t p, D d, A a);
	template<class Y> shared_ptr(const shared_ptr<Y>& r, T *p) noexcept;
	shared_ptr(const shared_ptr& r) noexcept;
	template<class Y> shared_ptr(const shared_ptr<Y>& r) noexcept;
	shared_ptr(shared_ptr&& r) noexcept;
	template<class Y> shared_ptr(shared_ptr<Y>&& r) noexcept;
	template<class Y> explicit shared_ptr(const weak_ptr<Y>& r);
	template<class Y> shared_ptr(auto_ptr<Y>&& r);
	template <class Y, class D> shared_ptr(unique_ptr<Y, D>&& r);
	shared_ptr(nullptr_t) : shared_ptr() { }
	*/

	// destructor:
	~shared_ptr()
	{
		clean();
	}

	// assignment:
	shared_ptr& operator=(const shared_ptr& r) noexcept
	{
		if (*this == r)
			return this;

		clean();

		Pointer = r.Pointer;
		Ctrl = r.Ctrl;
		Ctrl->Counter++;
	}

	/*
	template<class Y> shared_ptr& operator=(const shared_ptr<Y>& r) noexcept;
	shared_ptr& operator=(shared_ptr&& r) noexcept;
	template<class Y> shared_ptr& operator=(shared_ptr<Y>&& r);
	template<class Y> shared_ptr& operator=(auto_ptr<Y>&& r);
	*/
	//template <class Y, class D> shared_ptr& operator=(unique_ptr<Y, D>&& r);

	// modifiers:
	void swap(shared_ptr& other) noexcept
	{
		std::swap(Pointer, other.Pointer);
		std::swap(Ctrl, other.Ctrl);
	}
	void reset() noexcept
	{
		clean();
	}
	template<class Y>
	void reset(Y* ptr)
	{
		clean();
		Pointer = ptr;
		Ctrl = new CtrlBlock<T>(1);
	}
	/*
	template<class Y, class D> void reset(Y* p, D d);
	template<class Y, class D, class A> void reset(Y* p, D d, A a);
	*/

	// observers:
	T* get() const noexcept { return Pointer; }
	T& operator*() noexcept { return *Pointer; }
	T* operator->() noexcept { return Pointer; }
    /*
    long use_count() const noexcept;
    bool unique() const noexcept;
    explicit operator bool() const noexcept;
    template<class U> bool owner_before(shared_ptr<U> const& b) const;
    template<class U> bool owner_before(weak_ptr<U> const& b) const;
	*/


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
};

typedef unsigned int uint;

template<class T>
class my_shared_ptr
{
private:
	T* ptr = nullptr;
	uint* refCount = nullptr;

public:
	my_shared_ptr() : ptr(nullptr), refCount(new uint(0)) // default constructor
	{
	}

	my_shared_ptr(T* ptr) : ptr(ptr), refCount(new uint(1)) // constructor
	{
	}

	/*** Copy Semantics ***/
	my_shared_ptr(const my_shared_ptr& obj) // copy constructor
	{
		this->ptr = obj.ptr; // share the underlying pointer
		this->refCount = obj.refCount;
		if (nullptr != obj.ptr)
		{
			(*this->refCount)++; // if the pointer is not null, increment the refCount
		}
	}

	my_shared_ptr& operator=(const my_shared_ptr& obj) // copy assignment
	{
		__cleanup__(); // cleanup any existing data

		// Assign incoming object's data to this object
		this->ptr = obj.ptr; // share the underlying pointer
		this->refCount = obj.refCount;
		if (nullptr != obj.ptr)
		{
			(*this->refCount)++; // if the pointer is not null, increment the refCount
		}
	}

	/*** Move Semantics ***/
	my_shared_ptr(my_shared_ptr&& dyingObj) // move constructor
	{
		this->ptr = dyingObj.ptr; // share the underlying pointer
		this->refCount = dyingObj.refCount;

		dyingObj.ptr = dyingObj.refCount = nullptr; // clean the dying object
	}

	my_shared_ptr& operator=(my_shared_ptr&& dyingObj) // move assignment
	{
		__cleanup__(); // cleanup any existing data

		this->ptr = dyingObj.ptr; // share the underlying pointer
		this->refCount = dyingObj.refCount;

		dyingObj.ptr = dyingObj.refCount = nullptr; // clean the dying object
	}

	uint get_count() const
	{
		return *refCount; // *this->refCount
	}

	T* get() const
	{
		return this->ptr;
	}

	T* operator->() const
	{
		return this->ptr;
	}

	T& operator*() const
	{
		return this->ptr;
	}

	~my_shared_ptr() // destructor
	{
		__cleanup__();
	}

private:
	void __cleanup__()
	{
		(*refCount)--;
		if (*refCount == 0)
		{
			if (nullptr != ptr)
				delete ptr;
			delete refCount;
		}
	}
};