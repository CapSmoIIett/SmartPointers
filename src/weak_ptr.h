#pragma once

#include "defaultmemoryfunctions.h"
# include "shared_ptr.h"

template<typename T>
class weak_ptr
{
public:
    // constructors
    constexpr weak_ptr() noexcept :
        Pointer(nullptr), Ctrl(nullptr)
    { }

    weak_ptr(weak_ptr const& other) noexcept
    {
        this->Pointer = other->Pointer;
        this->Ctrl = other->Ctrl;
        this->Ctrl->WeakCounter++;
    }

    weak_ptr(shared_ptr<T>& spt)
    {
        this->Pointer = spt.Pointer;
        this->Ctrl = spt.Ctrl;
        this->Ctrl->WeakCounter++;
    }

    ~weak_ptr()
    {
        clean();
    }

    weak_ptr& operator=(weak_ptr const& other) noexcept
    {
        if (*this == r)
            return *this;

        this->Pointer = other.Pointer;
        this->Ctrl = other.Ctrl;

        this->Ctrl->WeakCounter++;

        return this;
    }

    weak_ptr& operator=( shared_ptr<T>& spt) 
    {
        this->Pointer = spt.Pointer;
        this->Ctrl = spt.Ctrl;

        this->Ctrl->WeakCounter++;

        return *this;
    }

    void swap(weak_ptr& other) noexcept
    {
        std::swap(this->Pointer, other.Pointer);
        std::swap(this->Ctrl, other.Ctrl);
    }

    void reset() noexcept { clean(); }
    long use_count() const noexcept { return Ctrl->Counter; }
    bool expired() const noexcept { return Ctrl->Counter <= 0; }

    shared_ptr<T> lock() const noexcept
    {
        if (Ctrl->Counter > 0)
            return shared_ptr<T>(*this);
        return shared_ptr<T>(nullptr);
    }

private:
	T* Pointer;
	CtrlBlock<T>* Ctrl;

	void clean()
	{
		Ctrl->WeakCounter--;
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

    friend class shared_ptr<T>;
};