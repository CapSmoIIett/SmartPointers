
#include "../googletest/googletest/include/gtest/gtest.h"


#include "../src/unique_ptr.h"
#include "../src/shared_ptr.h"
#include "../src/weak_ptr.h"

#include "../src/widget.h"

#include <memory>

TEST(Simple, simple)
{
    EXPECT_EQ(1, 1);
}

TEST(UNIQUE_PTR, INT)
{
    std::unique_ptr<int> upt1(new int(3));
    unique_ptr<int> upt2(new int(3));

    EXPECT_EQ(*upt1, *upt2);

    (*upt1)++;

    EXPECT_NE(*upt1, *upt2);

    (*upt2)++;

    EXPECT_EQ(*upt1, *upt2);
}

TEST(UNIQUE_PTR, CHAR)
{
    std::unique_ptr<char> upt1(new char('a'));
    unique_ptr<char> upt2(new char('a'));

    EXPECT_EQ(*upt1, *upt2);
}



TEST(UNIQUE_PTR, CONSTRUCTORS)
{
    std::unique_ptr<int> upt1(new int(0));
    unique_ptr<int> upt2(new int(0));

    EXPECT_EQ(*upt1, *upt2);

    upt1.reset(nullptr);
    upt2.reset(nullptr);

    EXPECT_TRUE(upt1.get() == nullptr);
    EXPECT_TRUE(upt2.get() == nullptr);

    std::unique_ptr<int> u3(std::move(upt1));
    unique_ptr<int> u4(std::move(upt2));

    EXPECT_TRUE(upt1.get() == nullptr);
    EXPECT_TRUE(upt2.get() == nullptr);

    EXPECT_TRUE(u3.get() == nullptr);
    EXPECT_TRUE(u4.get() == nullptr);

    u3.reset(new int(5));
    u4.reset(new int(5));

    EXPECT_EQ(*u3, *u4);
}


TEST(UNIQUE_PTR, RELEASE)
{
    std::unique_ptr<int> upt1(new int(0));
    unique_ptr<int> upt2(new int(0));

    auto p1 = upt1.release();
    auto p2 = upt2.release();

    EXPECT_TRUE(upt1.get() == nullptr);
    EXPECT_TRUE(upt2.get() == nullptr);

    EXPECT_EQ(*p1, *p2);
}

TEST(UNIQUE_PTR, SWAP)
{
    unique_ptr<int> upt1(new int(1));
    unique_ptr<int> upt2(new int(2));

    EXPECT_TRUE(*upt1 == 1);
    EXPECT_TRUE(*upt2 == 2);

    upt2.swap(upt1);

    EXPECT_TRUE(*upt1 == 2);
    EXPECT_TRUE(*upt2 == 1);

    upt1.swap(upt2);

    EXPECT_TRUE(*upt1 == 1);
    EXPECT_TRUE(*upt2 == 2);
}



TEST(UNIQUE_PTR_ARRAY, INT)
{
    /*int size = 10;
    std::unique_ptr<int[]> upt1(new int[size]);
    unique_ptr<int[]> upt2(new int[size]);

    for (int i = 0; i < size; i++)
        upt1[i] = i;

    for (int i = 0; i < size; i++)
        upt2[i] = i;

    for (int i = 0; i < size; i++)
    //    EXPECT_EQ(upt1[i], upt2[i]);

    for (int i = 0; i < size; i++)
    {
        upt1[i]++;
        upt2[i]++;
    }
    */

    //for (int i = 0; i < size; i++)
//    int i = 0;
//        EXPECT_EQ(upt1[i], upt2[i]);
}




TEST(SHARED_PTR, INT)
{
    std::shared_ptr<int> spt1(new int(2));
    shared_ptr<int> spt2(new int (2));

    EXPECT_EQ(*spt1, *spt2);

    (*spt1)++;

    EXPECT_NE(*spt1, *spt2);

    (*spt2)++;

    EXPECT_EQ(*spt1, *spt2);

}

TEST(SHARED_PTR, CHAR)
{
    std::shared_ptr<int> spt1(new int(2));
    shared_ptr<int> spt2(new int (2));

    EXPECT_EQ(*spt1, *spt2);
}

TEST(SHARED_PTR, CONSTRUCTORS)
{
    std::shared_ptr<int> spt1(new int(2));
    shared_ptr<int> spt2(new int (2));

    EXPECT_EQ(*spt1, *spt2);

    /*
    spt1.reset(nullptr);
    spt2.reset(nullptr);

    EXPECT_TRUE(upt1.get() == nullptr);
    EXPECT_TRUE(upt2.get() == nullptr);
    */

    {
        std::shared_ptr<int> spt3(spt1);
        shared_ptr<int> spt4(spt2);

        (*spt3)++;
        (*spt4)++;

        EXPECT_EQ(*spt3, *spt4);
    }

    EXPECT_EQ(*spt1, *spt2);

}

TEST(SHARED_PTR, SWAP)
{
    shared_ptr<int> spt1(new int(1));
    shared_ptr<int> spt2(new int (2));

    EXPECT_TRUE(*spt1 == 1);
    EXPECT_TRUE(*spt2 == 2);

    spt2.swap(spt1);

    EXPECT_TRUE(*spt1 == 2);
    EXPECT_TRUE(*spt2 == 1);

    spt1.swap(spt2);

    EXPECT_TRUE(*spt1 == 1);
    EXPECT_TRUE(*spt2 == 2);
}





TEST(WEAK_PTR, CONSTRUCTORS)
{
    std::shared_ptr<int> spt1(new int(2));
    shared_ptr<int> spt2(new int (2));

    std::weak_ptr<int> wpt1(spt1);
    weak_ptr<int> wpt2(spt2);

    EXPECT_EQ(wpt1.expired(), wpt2.expired());


    std::shared_ptr<int> spt3(wpt1);
    shared_ptr<int> spt4(wpt2);

    EXPECT_EQ(wpt1.use_count(), wpt2.use_count());
}

TEST(WEAK_PTR, CONSTRUCTORS1)
{
    std::shared_ptr<int> spt1(new int(2));
    shared_ptr<int> spt2(new int (2));

    std::shared_ptr<int> spt3(new int(2));
    shared_ptr<int> spt4(new int (2));

    std::weak_ptr<int> wpt1(spt1);
    weak_ptr<int> wpt2(spt2);

    spt1 = spt3;
    spt2 = spt4;

    EXPECT_EQ(wpt1.use_count(), wpt2.use_count());
    EXPECT_TRUE(wpt1.expired());
    EXPECT_TRUE(wpt2.expired());

    wpt1 = spt3;
    wpt2 = spt4;

    EXPECT_EQ(wpt1.use_count(), wpt2.use_count());
    EXPECT_EQ(wpt1.use_count(), 2);
}

TEST(WEAK_PTR, CONSTRUCTORS2)
{

    std::weak_ptr<int> wpt1;
    weak_ptr<int> wpt2;

    {
        std::shared_ptr<int> spt1(new int(2));
        shared_ptr<int> spt2(new int(2));

        wpt1 = spt1;
        wpt2 = spt2;

        EXPECT_EQ(wpt1.use_count(), wpt2.use_count());
        EXPECT_FALSE(wpt1.expired());
        EXPECT_FALSE(wpt2.expired());
    }

    EXPECT_EQ(wpt1.use_count(), wpt2.use_count());
    EXPECT_TRUE(wpt1.expired());
    EXPECT_TRUE(wpt2.expired());

}

TEST(WEAK_PTR, LOCK)
{
    std::shared_ptr<int> spt1(new int(2));
    shared_ptr<int> spt2(new int (2));

    std::weak_ptr<int> wpt1(spt1);
    weak_ptr<int> wpt2(spt2);

    EXPECT_EQ(wpt1.use_count(), wpt2.use_count());

    auto spt3 = wpt1.lock();
    auto spt4 = wpt2.lock();

    EXPECT_EQ(wpt1.use_count(), wpt2.use_count());
    EXPECT_EQ(*spt3, *spt4);
}

TEST(WEAK_PTR, LOCK1)
{
    std::weak_ptr<int> wpt1;
    weak_ptr<int> wpt2;

    {
        std::shared_ptr<int> spt1(new int(2));
        shared_ptr<int> spt2(new int(2));

        wpt1 = spt1;
        wpt2 = spt2;

        EXPECT_EQ(wpt1.use_count(), wpt2.use_count());
        EXPECT_FALSE(wpt1.expired());
        EXPECT_FALSE(wpt2.expired());

        auto spt3 = wpt1.lock();
        auto spt4 = wpt2.lock();

        EXPECT_EQ(wpt1.use_count(), wpt2.use_count());
        EXPECT_EQ(*spt3, *spt4);
    }

    auto spt3 = wpt1.lock();
    auto spt4 = wpt2.lock();

    EXPECT_EQ(wpt1.use_count(), wpt2.use_count());
    EXPECT_EQ(spt3.get(), nullptr);
    EXPECT_EQ(spt4.get(), nullptr);
}

TEST(WEAK_PTR, SWAP)
{
    shared_ptr<int> spt1(new int(1));
    shared_ptr<int> spt2(new int (2));

    weak_ptr<int> wpt1(spt1);
    weak_ptr<int> wpt2(spt2);

    EXPECT_EQ(wpt1.use_count(), wpt2.use_count());
    EXPECT_FALSE(wpt1.expired());
    EXPECT_FALSE(wpt2.expired());

    {
        auto spt3 = wpt1.lock();
        auto spt4 = wpt2.lock();

        EXPECT_EQ(*spt3, 1);
        EXPECT_EQ(*spt4, 2);
    }

    wpt2.swap(wpt1);

    {
        auto spt3 = wpt1.lock();
        auto spt4 = wpt2.lock();

        EXPECT_EQ(*spt3, 2);
        EXPECT_EQ(*spt4, 1);
    }

    wpt1.swap(wpt2);

    {
        auto spt3 = wpt1.lock();
        auto spt4 = wpt2.lock();

        EXPECT_EQ(*spt3, 1);
        EXPECT_EQ(*spt4, 2);
    }
}



TEST (WIDGET, CONSTRUCTOR)
{
    auto root = Widget::CreateWidget();

    auto node1 = Widget::CreateWidget(root);
    auto node2 = Widget::CreateWidget(root);
    auto node3 = Widget::CreateWidget(root);

    auto node11 = Widget::CreateWidget(node1);
    auto node12 = Widget::CreateWidget(node1);
    auto node13 = Widget::CreateWidget(node1);

    auto node121 = Widget::CreateWidget(node12);
    auto node122 = Widget::CreateWidget(node12);
    auto node123 = Widget::CreateWidget(node12);
    
    EXPECT_EQ(root, node1->GetParent().lock());
    EXPECT_EQ(root, node2->GetParent().lock());
    EXPECT_EQ(root, node3->GetParent().lock());

    EXPECT_EQ(node1, (*root)[0]);
    EXPECT_EQ(node2, (*root)[1]);
    EXPECT_EQ(node3, (*root)[2]);

    EXPECT_EQ(node1, node11->GetParent().lock());
    EXPECT_EQ(node1, node12->GetParent().lock());
    EXPECT_EQ(node1, node13->GetParent().lock());

    EXPECT_EQ(node11, (*node1)[0]);
    EXPECT_EQ(node12, (*node1)[1]);
    EXPECT_EQ(node13, (*node1)[2]);

    EXPECT_EQ(node12, node121->GetParent().lock());
    EXPECT_EQ(node12, node122->GetParent().lock());
    EXPECT_EQ(node12, node123->GetParent().lock());

    EXPECT_EQ(node121, (*node12)[0]);
    EXPECT_EQ(node122, (*node12)[1]);
    EXPECT_EQ(node123, (*node12)[2]);
}

TEST(WIDGET, TYPE)
{
    const auto root = Widget::CreateWidget();
    EXPECT_EQ(root->GetClassName(), "Widget");
    EXPECT_EQ(root->GetClassType(), typeid(root.get()));
}




TEST (CALENDARWIDGET, CONSTRUCTOR)
{
    auto root = CalendarWidget::CreateCalendarWidget();

    auto node1 = CalendarWidget::CreateCalendarWidget(root);
    auto node2 = CalendarWidget::CreateCalendarWidget(root);
    auto node3 = CalendarWidget::CreateCalendarWidget(root);

    auto node11 = CalendarWidget::CreateCalendarWidget(node1);
    auto node12 = CalendarWidget::CreateCalendarWidget(node1);
    auto node13 = CalendarWidget::CreateCalendarWidget(node1);

    auto node121 = CalendarWidget::CreateCalendarWidget(node12);
    auto node122 = CalendarWidget::CreateCalendarWidget(node12);
    auto node123 = CalendarWidget::CreateCalendarWidget(node12);
    
    EXPECT_EQ(dynamic_pointer_cast<Widget>(root), (node1->GetParent().lock()));
    EXPECT_EQ(root, node2->GetParent().lock());
    EXPECT_EQ(root, node3->GetParent().lock());

    EXPECT_EQ(node1, (*root)[0]);
    EXPECT_EQ(node2, (*root)[1]);
    EXPECT_EQ(node3, (*root)[2]);

    EXPECT_EQ(node1, node11->GetParent().lock());
    EXPECT_EQ(node1, node12->GetParent().lock());
    EXPECT_EQ(node1, node13->GetParent().lock());

    EXPECT_EQ(node11, (*node1)[0]);
    EXPECT_EQ(node12, (*node1)[1]);
    EXPECT_EQ(node13, (*node1)[2]);

    EXPECT_EQ(node12, node121->GetParent().lock());
    EXPECT_EQ(node12, node122->GetParent().lock());
    EXPECT_EQ(node12, node123->GetParent().lock());

    EXPECT_EQ(node121, (*node12)[0]);
    EXPECT_EQ(node122, (*node12)[1]);
    EXPECT_EQ(node123, (*node12)[2]);
}

TEST(CALENDARWIDGET, TYPE)
{
    const auto root = CalendarWidget::CreateCalendarWidget();
    EXPECT_EQ(root->GetClassName(), "CalendarWidget");
    EXPECT_EQ(root->GetClassType(), typeid(root.get()));
}




TEST (TABWIDGET, CONSTRUCTOR)
{
    auto root = TabWidget::CreateTabWidget();

    auto node1 = TabWidget::CreateTabWidget(root);
    auto node2 = TabWidget::CreateTabWidget(root);
    auto node3 = TabWidget::CreateTabWidget(root);

    auto node11 = TabWidget::CreateTabWidget(node1);
    auto node12 = TabWidget::CreateTabWidget(node1);
    auto node13 = TabWidget::CreateTabWidget(node1);

    auto node121 = TabWidget::CreateTabWidget(node12);
    auto node122 = TabWidget::CreateTabWidget(node12);
    auto node123 = TabWidget::CreateTabWidget(node12);
    
    EXPECT_EQ(root, node1->GetParent().lock());
    EXPECT_EQ(root, node2->GetParent().lock());
    EXPECT_EQ(root, node3->GetParent().lock());

    EXPECT_EQ(node1, (*root)[0]);
    EXPECT_EQ(node2, (*root)[1]);
    EXPECT_EQ(node3, (*root)[2]);

    EXPECT_EQ(node1, node11->GetParent().lock());
    EXPECT_EQ(node1, node12->GetParent().lock());
    EXPECT_EQ(node1, node13->GetParent().lock());

    EXPECT_EQ(node11, (*node1)[0]);
    EXPECT_EQ(node12, (*node1)[1]);
    EXPECT_EQ(node13, (*node1)[2]);

    EXPECT_EQ(node12, node121->GetParent().lock());
    EXPECT_EQ(node12, node122->GetParent().lock());
    EXPECT_EQ(node12, node123->GetParent().lock());

    EXPECT_EQ(node121, (*node12)[0]);
    EXPECT_EQ(node122, (*node12)[1]);
    EXPECT_EQ(node123, (*node12)[2]);
}

TEST(TABWIDGET, TYPE)
{
    const auto root = TabWidget::CreateTabWidget();
    EXPECT_EQ(root->GetClassName(), "TabWidget");
    EXPECT_EQ(root->GetClassType(), typeid(root.get()));
}


//*/
