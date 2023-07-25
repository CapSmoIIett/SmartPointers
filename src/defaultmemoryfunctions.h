#pragma once

template <typename T>
struct default_delete
{
    constexpr default_delete() noexcept = default;
    default_delete(const default_delete&) noexcept = default;

    template <typename U>
    default_delete(const default_delete<U>&)
    {}

    void operator()(T* ptr) const noexcept
    { delete ptr; }
};


template <typename T>
struct default_delete<T[]>
{
    constexpr default_delete() noexcept = default;
    default_delete(const default_delete&) noexcept = default;

    template <typename U>
    default_delete(const default_delete<U[]>&)
    {}

    void operator()(T* ptr) const noexcept
    { delete[] ptr;}

};


template <typename T>
struct default_allocator
{
    constexpr default_allocator() noexcept = default;
    default_allocator(const default_allocator&) noexcept = default;

    template <typename U>
    default_allocator(const default_allocator<U>&)
    {}

    T* operator()(T) const noexcept
    { return new ptr(T); }

    T* operator()() const noexcept
    { return new ptr; }
    
};


