#include "test.hpp"
#include "fnx/fnx.hpp"

TEST(math, random)
{
    EXPECT_EQ(0, fnx::random(0,0));
    auto val = fnx::random(25, 75);
    EXPECT_LT(24, val);
    EXPECT_GT(76, val);
}

TEST(math, value_from_ratio)
{
    auto val = fnx::calc_value(0, 100, .5f);
    EXPECT_EQ(50, val);
}

TEST(math, ratio_from_value)
{
    auto val = fnx::calc_ratio(0, 100, 50);
    EXPECT_ALMOST_EQ(.5f, val);
}

TEST(math, minimum)
{
    EXPECT_EQ(0, fnx::minimum(0,100));
    EXPECT_EQ(-1, fnx::minimum(-1,100));
    EXPECT_EQ(100, fnx::minimum(789,100));
    EXPECT_ALMOST_EQ(.5f, fnx::minimum(.5f,1.f));
    EXPECT_ALMOST_EQ(5.0, fnx::minimum(10.0, 5));
}

TEST(math, maximum)
{
    EXPECT_EQ(100, fnx::maximum(0,100));
    EXPECT_EQ(100, fnx::maximum(-1,100));
    EXPECT_EQ(789, fnx::maximum(789,100));
    EXPECT_ALMOST_EQ(1.f, fnx::maximum(.5f,1.f));
    EXPECT_ALMOST_EQ(10.0, fnx::maximum(10.0, 5));
}

TEST(math, clamp)
{
    EXPECT_EQ(100, fnx::clamp(1000,0,100));
    EXPECT_EQ(0, fnx::clamp(-1000,0,100));
    EXPECT_EQ(50, fnx::clamp(50,0,100));
}

TEST(math, factorial_sum)
{
    EXPECT_EQ(15, fnx::factorial_sum(5));
}

TEST(math, factorial)
{
    EXPECT_EQ(120, fnx::factorial(5));
}