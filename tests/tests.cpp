
#include "../googletest/googletest/include/gtest/gtest.h"


#include "../src/unique_ptr.h"
#include "../src/shared_ptr.h"
#include "../src/weak_ptr.h"

#include <memory>

TEST(Simple, simple)
{
    EXPECT_EQ(1, 1);
}

TEST(UNIQUE_PTR, INT)
{
    std::unique_ptr<int> u1(new int(3));
    unique_ptr<int> u2(new int(3));

    EXPECT_EQ(*u1, *u2);

    (*u1)++;

    EXPECT_NE(*u1, *u2);

    (*u2)++;

    EXPECT_EQ(*u1, *u2);
}

TEST(UNIQUE_PTR, CHAR)
{
    std::unique_ptr<char> u1(new char('a'));
    unique_ptr<char> u2(new char('a'));

    EXPECT_EQ(*u1, *u2);
}



TEST(UNIQUE_PTR, CONSTRUCTORS)
{
    std::unique_ptr<int> u1(new int(0));
    unique_ptr<int> u2(new int(0));

    EXPECT_EQ(*u1, *u2);

    u1.reset(nullptr);
    u2.reset(nullptr);

    EXPECT_TRUE(u1.get() == nullptr);
    EXPECT_TRUE(u2.get() == nullptr);

    std::unique_ptr<int> u3(std::move(u1));
    unique_ptr<int> u4(std::move(u2));

    EXPECT_TRUE(u1.get() == nullptr);
    EXPECT_TRUE(u2.get() == nullptr);

    EXPECT_TRUE(u3.get() == nullptr);
    EXPECT_TRUE(u4.get() == nullptr);

    u3.reset(new int(5));
    u4.reset(new int(5));

    EXPECT_EQ(*u3, *u4);
}


TEST(UNIQUE_PTR, RELEASE)
{
    std::unique_ptr<int> u1(new int(0));
    unique_ptr<int> u2(new int(0));

    auto p1 = u1.release();
    auto p2 = u2.release();

    EXPECT_TRUE(u1.get() == nullptr);
    EXPECT_TRUE(u2.get() == nullptr);

    EXPECT_EQ(*p1, *p2);
}

TEST(UNIQUE_PTR, SWAP)
{
    unique_ptr<int> u1(new int(1));
    unique_ptr<int> u2(new int(2));

    EXPECT_TRUE(*u1 == 1);
    EXPECT_TRUE(*u2 == 2);

    u2.swap(u1);

    EXPECT_TRUE(*u1 == 2);
    EXPECT_TRUE(*u2 == 1);

    u1.swap(u2);

    EXPECT_TRUE(*u1 == 1);
    EXPECT_TRUE(*u2 == 2);
}




TEST(UNIQUE_PTR_ARRAY, INT)
{
    /*int size = 10;
    std::unique_ptr<int[]> u1(new int[size]);
    unique_ptr<int[]> u2(new int[size]);

    for (int i = 0; i < size; i++)
        u1[i] = i;

    for (int i = 0; i < size; i++)
        u2[i] = i;

    for (int i = 0; i < size; i++)
    //    EXPECT_EQ(u1[i], u2[i]);

    for (int i = 0; i < size; i++)
    {
        u1[i]++;
        u2[i]++;
    }
    */

    //for (int i = 0; i < size; i++)
//    int i = 0;
//        EXPECT_EQ(u1[i], u2[i]);
}




TEST(SHARED_PTR, INT)
{
    std::shared_ptr<int> s1(new int(2));
    shared_ptr<int> s2(new int (2));

    EXPECT_EQ(*s1, *s2);

    (*s1)++;

    EXPECT_NE(*s1, *s2);

    (*s2)++;

    EXPECT_EQ(*s1, *s2);

}

TEST(SHARED_PTR, CHAR)
{
    std::shared_ptr<int> s1(new int(2));
    shared_ptr<int> s2(new int (2));

    EXPECT_EQ(*s1, *s2);
}

TEST(SHARED_PTR, CONSTRUCTORS)
{
    std::shared_ptr<int> s1(new int(2));
    shared_ptr<int> s2(new int (2));

    EXPECT_EQ(*s1, *s2);

    /*
    s1.reset(nullptr);
    s2.reset(nullptr);

    EXPECT_TRUE(u1.get() == nullptr);
    EXPECT_TRUE(u2.get() == nullptr);
    */

    {
        std::shared_ptr<int> s3(s1);
        shared_ptr<int> s4(s2);

        (*s3)++;
        (*s4)++;

        EXPECT_EQ(*s3, *s4);
    }

    EXPECT_EQ(*s1, *s2);

}

TEST(SHARED_PTR, SWAP)
{
    shared_ptr<int> s1(new int(1));
    shared_ptr<int> s2(new int (2));

    EXPECT_TRUE(*s1 == 1);
    EXPECT_TRUE(*s2 == 2);

    s2.swap(s1);

    EXPECT_TRUE(*s1 == 2);
    EXPECT_TRUE(*s2 == 1);

    s1.swap(s2);

    EXPECT_TRUE(*s1 == 1);
    EXPECT_TRUE(*s2 == 2);
}





TEST(WEAK_PTR, CONSTRUCTORS)
{
    std::shared_ptr<int> s1(new int(2));
    shared_ptr<int> s2(new int (2));

    std::weak_ptr<int> w1(s1);
    weak_ptr<int> w2(s2);

    EXPECT_EQ(w1.expired(), w2.expired());


    std::shared_ptr<int> s3(w1);
    shared_ptr<int> s4(w2);

    EXPECT_EQ(w1.use_count(), w2.use_count());
}

TEST(WEAK_PTR, CONSTRUCTORS1)
{
    std::shared_ptr<int> s1(new int(2));
    shared_ptr<int> s2(new int (2));

    std::shared_ptr<int> s3(new int(2));
    shared_ptr<int> s4(new int (2));

    std::weak_ptr<int> w1(s1);
    weak_ptr<int> w2(s2);

    s1 = s3;
    s2 = s4;

    EXPECT_EQ(w1.use_count(), w2.use_count());
    EXPECT_TRUE(w1.expired());
    EXPECT_TRUE(w2.expired());

    w1 = s3;
    w2 = s4;

    EXPECT_EQ(w1.use_count(), w2.use_count());
    EXPECT_EQ(w1.use_count(), 2);
}

TEST(WEAK_PTR, CONSTRUCTORS2)
{

    std::weak_ptr<int> w1;
    weak_ptr<int> w2;

    {
        std::shared_ptr<int> s1(new int(2));
        shared_ptr<int> s2(new int(2));

        w1 = s1;
        w2 = s2;

        EXPECT_EQ(w1.use_count(), w2.use_count());
        EXPECT_FALSE(w1.expired());
        EXPECT_FALSE(w2.expired());
    }

    EXPECT_EQ(w1.use_count(), w2.use_count());
    EXPECT_TRUE(w1.expired());
    EXPECT_TRUE(w2.expired());

}

TEST(WEAK_PTR, LOCK)
{
    std::shared_ptr<int> s1(new int(2));
    shared_ptr<int> s2(new int (2));

    std::weak_ptr<int> w1(s1);
    weak_ptr<int> w2(s2);

    EXPECT_EQ(w1.use_count(), w2.use_count());

    auto s3 = w1.lock();
    auto s4 = w2.lock();

    EXPECT_EQ(w1.use_count(), w2.use_count());
    EXPECT_EQ(*s3, *s4);
}

TEST(WEAK_PTR, LOCK1)
{
    std::weak_ptr<int> w1;
    weak_ptr<int> w2;

    {
        std::shared_ptr<int> s1(new int(2));
        shared_ptr<int> s2(new int(2));

        w1 = s1;
        w2 = s2;

        EXPECT_EQ(w1.use_count(), w2.use_count());
        EXPECT_FALSE(w1.expired());
        EXPECT_FALSE(w2.expired());

        auto s3 = w1.lock();
        auto s4 = w2.lock();

        EXPECT_EQ(w1.use_count(), w2.use_count());
        EXPECT_EQ(*s3, *s4);
    }

    auto s3 = w1.lock();
    auto s4 = w2.lock();

    EXPECT_EQ(w1.use_count(), w2.use_count());
    EXPECT_EQ(s3.get(), nullptr);
    EXPECT_EQ(s4.get(), nullptr);
}

TEST(WEAK_PTR, SWAP)
{
    shared_ptr<int> s1(new int(1));
    shared_ptr<int> s2(new int (2));

    weak_ptr<int> w1(s1);
    weak_ptr<int> w2(s2);

    EXPECT_EQ(w1.use_count(), w2.use_count());
    EXPECT_FALSE(w1.expired());
    EXPECT_FALSE(w2.expired());

    {
        auto s3 = w1.lock();
        auto s4 = w2.lock();

        EXPECT_EQ(*s3, 1);
        EXPECT_EQ(*s4, 2);
    }

    w2.swap(w1);

    {
        auto s3 = w1.lock();
        auto s4 = w2.lock();

        EXPECT_EQ(*s3, 2);
        EXPECT_EQ(*s4, 1);
    }

    w1.swap(w2);

    {
        auto s3 = w1.lock();
        auto s4 = w2.lock();

        EXPECT_EQ(*s3, 1);
        EXPECT_EQ(*s4, 2);
    }
}

//*/
