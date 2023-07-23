

#include<type_traits>

#include "defaultmemoryfunctions.h"



template <typename T, typename Deleter = default_delete<T>>
class unique_ptr
{
public:

    // Constructors
    constexpr unique_ptr() = default;
    explicit unique_ptr(T* ptr) noexcept
        : Pointer(ptr)
    {}
    unique_ptr(T* ptr, const Deleter& deleter)
        : Pointer(ptr), _Deleter(deleter)
    {}
    unique_ptr(unique_ptr&& other) noexcept
        : Pointer(other.release()), _Deleter(other._Deleter)
    {}
    template <typename U, typename D>
    unique_ptr(unique_ptr<U, D>&& other) noexcept
        : Pointer(other.release()), _Deleter(std::forward<D>(other.get_deleter()))
    {}
    
    unique_ptr(const unique_ptr& other) = delete;


    // Destructors
    ~unique_ptr() { _Deleter(Pointer); }


    unique_ptr& operator=(const unique_ptr& other) = delete;
    unique_ptr& operator=(unique_ptr&& other) noexcept
    {
        unique_ptr(std::move(other)).swap(*this);
        return *this;
    }
    template <class U, class D> unique_ptr& operator=(unique_ptr<U, D>&& u) noexcept;


    T* operator->() const noexcept { return Pointer; }
    T& operator*() const noexcept { return *Pointer; }
    T* get() const noexcept { return Pointer; }
    Deleter& get_deleter() const noexcept { return _Deleter; }
    explicit operator bool() { return Pointer != nullptr; }

    T* release() noexcept
    {
        auto old_ptr = Pointer;
        Pointer = nullptr;
        return old_ptr;
    }

    void reset(T* ptr) noexcept
    {
        _Deleter(Pointer);
        Pointer = ptr;
    }

    void swap(unique_ptr& other) noexcept
    {
        std::swap(Pointer, other.Pointer);
    }


private:
    T* Pointer = nullptr;
    Deleter _Deleter = Deleter();
};


template <typename T, typename Deleter>
class unique_ptr<T[], Deleter>
{
public:

    // Constructors
    constexpr unique_ptr() = default;
    explicit unique_ptr(T* ptr) noexcept
        : Pointer(ptr)
    {}
    unique_ptr(T* ptr, const Deleter& deleter)
        : Pointer(ptr), _Deleter(deleter)
    {}
    unique_ptr(unique_ptr&& other) noexcept
        : Pointer(other.release()), _Deleter(other._Deleter)
    {}
    template <typename U, typename D>
    unique_ptr(unique_ptr<U, D>&& other) noexcept
        : Pointer(other.release()), _Deleter(std::forward<D>(other.get_deleter()))
    {}
    
    unique_ptr(const unique_ptr& other) = delete;


    // Destructors
    ~unique_ptr() { _Deleter(Pointer); }


    unique_ptr& operator=(const unique_ptr& other) = delete;
    unique_ptr& operator=(unique_ptr&& other) noexcept
    {
        unique_ptr(std::move(other)).swap(*this);
        return *this;
    }
    template <class U, class D> unique_ptr& operator=(unique_ptr<U, D>&& u) noexcept;


    T& operator[](int i) const noexcept { return *Pointer + i * sizeof(T); }
    T* get() const noexcept { return Pointer; }
    Deleter& get_deleter() const noexcept { return _Deleter; }
    explicit operator bool() { return Pointer != nullptr; }

    T* release() noexcept
    {
        auto old_ptr = Pointer;
        Pointer = nullptr;
        return old_ptr;
    }

    void reset(T* ptr) noexcept
    {
        _Deleter(Pointer);
        Pointer = ptr;
    }

    void swap(unique_ptr& other) noexcept
    {
        using std::swap;
        swap(Pointer, other.Pointer);
    }


private:
    T* Pointer = nullptr;
    Deleter _Deleter = Deleter();
};

