#pragma once

#include <vector>
#include <string>

#include <typeinfo>

#include "../src/shared_ptr.h"
#include "../src/weak_ptr.h"
#include "../src/unique_ptr.h"


class Widget 
{
public:
    Widget(const shared_ptr<Widget>& parent = nullptr) :
        Parent(weak_ptr<Widget> (parent))
    {
        //parent->AddChild(this);
    }

     virtual ~Widget()
    {
    }

    static shared_ptr<Widget> CreateWidget(const shared_ptr<Widget>& parent = nullptr)
    {
        auto ptr = shared_ptr<Widget>(new Widget(parent));
        if (parent.get() != nullptr)
            parent->AddChild(ptr);
        return ptr;
    }

    virtual inline std::string GetClassName()
    {
        return "Widget";
    }

    virtual const std::type_info& GetClassType()
    {
        return typeid(this);
    }

    void AddChild(shared_ptr<Widget> child)
    {
        this->Childrens.push_back(child);
    }

    weak_ptr<Widget> GetParent() const
    {
        return Parent;
    }

    std::vector <shared_ptr<Widget>> GetChildrens () const
    {
        return Childrens;
    }
    shared_ptr<Widget> operator[](int i)
    {
        if (Childrens.size() < i)
            return shared_ptr<Widget>();

        return Childrens[i];
    }

protected:
    weak_ptr<Widget> Parent;
    std::vector <shared_ptr<Widget>> Childrens;

};


class CalendarWidget : public Widget
{
public:
    CalendarWidget(const shared_ptr<Widget>& parent = nullptr) :
        Widget(parent)
    {
        //parent->AddChild(this);
    }

    CalendarWidget(const shared_ptr<CalendarWidget>& parent = nullptr) :
        Widget(dynamic_pointer_cast<Widget>(parent))
    {
        //parent->AddChild(this);
    }

    static shared_ptr<CalendarWidget> CreateCalendarWidget(const shared_ptr<Widget>& parent = nullptr)
    {
        auto ptr = shared_ptr<CalendarWidget>(new CalendarWidget(parent));
        if (parent.get() != nullptr)
            parent->AddChild(dynamic_pointer_cast<Widget>(ptr));
        return ptr;
    }

    static shared_ptr<CalendarWidget> CreateCalendarWidget(const shared_ptr<CalendarWidget>& parent)
    {
        auto ptr = shared_ptr<CalendarWidget>(new CalendarWidget(parent));
        if (parent.get() != nullptr)
            parent->AddChild(dynamic_pointer_cast<Widget>(ptr));
        return ptr;
    }


    virtual inline std::string GetClassName() override
    {
        return "CalendarWidget";
    }

    virtual const std::type_info& GetClassType() override
    {
        return typeid(this);
    }
};

class TabWidget : public Widget
{
public:

    TabWidget(const shared_ptr<Widget>& parent = nullptr) :
        Widget(parent)
    {
        //parent->AddChild(this);
    }

    TabWidget(const shared_ptr<TabWidget>& parent = nullptr) :
        Widget(dynamic_pointer_cast<Widget>(parent))
    {
        //parent->AddChild(this);
    }

    static shared_ptr<TabWidget> CreateTabWidget(const shared_ptr<Widget>& parent = nullptr)
    {
        auto ptr = shared_ptr<TabWidget>(new TabWidget(parent));
        if (parent.get() != nullptr)
            parent->AddChild(dynamic_pointer_cast<Widget>(ptr));
        return ptr;
    }

    static shared_ptr<TabWidget> CreateTabWidget(const shared_ptr<TabWidget>& parent)
    {
        auto ptr = shared_ptr<TabWidget>(new TabWidget(parent));
        if (parent.get() != nullptr)
            parent->AddChild(dynamic_pointer_cast<Widget>(ptr));
        return ptr;
    }

    virtual inline std::string GetClassName() override
    {
        return "TabWidget";
    }

    virtual const std::type_info& GetClassType() override
    {
        return typeid(this);
    }

};