#pragma once

#include <vector>
#include <memory>
#include <string>

class Widget 
{
public:
    Widget(const std::shared_ptr<Widget>& parent = nullptr) :
        Parent(parent)
    {
        //parent->AddChild(this);
    }

     ~Widget()
    {

    }

    static std::shared_ptr<Widget> CreateWidget(const std::shared_ptr<Widget>& parent = nullptr)
    {
        auto ptr = std::shared_ptr<Widget>(new Widget(parent));
        if (parent != nullptr)
            parent->AddChild(ptr);
        return ptr;
    }

    virtual inline std::string GetClassName()
    {
        return "Widget";
    }

    void AddChild(std::shared_ptr<Widget> child)
    {
        this->Childrens.push_back(child);
    }

    std::weak_ptr<Widget> GetParent() const
    {
        return Parent;
    }

    std::vector <std::shared_ptr<Widget>> GetChildrens () const
    {
        return Childrens;
    }
    std::shared_ptr<Widget> operator[](int i)
    {
        if (Childrens.size() < i)
            return std::shared_ptr<Widget>();

        return Childrens[i];
    }

private:
    std::weak_ptr<Widget> Parent;
    std::vector <std::shared_ptr<Widget>> Childrens;

};


class CalendarWidget : public Widget
{
        
    
};

class TabWidget : public Widget
{
    
};