
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

    void operator()(T* ptr) const noexcept;

    template <typename U>
    void operator()(U* ptr) const = delete;
};
