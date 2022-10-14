#include "test.hpp"
#include "fnx/fnx.hpp"

TEST(containers, dequeue)
{
    fnx::dequeue<int> container;
    container.emplace_back(3);
    auto& c = container.front();
    auto& d = container.back();
    auto e = int();
    container.push_back(e);
    EXPECT_EQ(c, d);
    EXPECT_NE(c, e);
    EXPECT_EQ(2, container.size());
}

TEST(containers, unordered_vector)
{
    fnx::unordered_vector<int> container{ 1,2,3,4,5 };
    
    {
        auto total = 0;
        for (auto i : container)
        {
            total += i;
        }
        EXPECT_EQ(fnx::factorial_sum(5), total);
    }	

    auto found = container.find(3);
    EXPECT_NE(found, container.end());
    
    container.erase(found);
    {
        auto total = 0;
        for (auto i : container)
        {
            total += i;
        }
        EXPECT_EQ(12, total);
    }
}

TEST(containers, ring_buffer)
{
    fnx::ring_buffer<int,3> container({1,2,3});
    EXPECT_EQ(3, container.size());
    EXPECT_EQ(1, container.pop());
    EXPECT_EQ(2, container.size());
    container.push(4);
    EXPECT_EQ(3, container.size());    
    EXPECT_EQ(2, container.pop());
    EXPECT_EQ(3, container.pop());
    EXPECT_EQ(4, container.pop());
}