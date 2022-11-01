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

TEST(matrix4x4, determinant)
{
    fnx::matrix4x4 in(1.0, 7.0, 14.0, 0.0, 9.0, 4.0, 18.0, 18.0, 2.0, 4.0, 5.0, 5.0, 1.0, 7.0, 1.0, 11.0);
    EXPECT_ALMOST_EQ(1552.0, in.getDeterminant());
}

TEST(matrix4x4, addition)
{
    fnx::matrix4x4 a(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
    fnx::matrix4x4 b(1,5,9,13,2,6,10,14,3,7,11,15,4,8,12,16);
    fnx::matrix4x4 e(2,6,10,14,
                     6,10,14,18,
                     10,14,18,22,
                     14,18,22,26);
    auto c = a + b;
    EXPECT_EQ(e,c);
}

TEST(matrix4x4, subtraction)
{
    fnx::matrix4x4 a(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
    fnx::matrix4x4 b(1,5,9,13,2,6,10,14,3,7,11,15,4,8,12,16);
    fnx::matrix4x4 e(0,-2,-4,-9,
                     2,0,-9,18,
                     4,9,0,22,
                     9,18,22,0);
    auto c = a - b;
    EXPECT_EQ(e,c);
}

TEST(matrix4x4, multiplication)
{
    fnx::matrix4x4 a(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
    fnx::matrix4x4 b(1,5,9,13,2,6,10,14,3,7,11,15,4,8,12,16);
    fnx::matrix4x4 e(0,-2,-4,-9,
                     2,0,-9,18,
                     4,9,0,22,
                     9,18,22,0);
    auto c = a * b;
    EXPECT_EQ(e,c);
}

TEST(matrix4x4, multiplication_quaternion)
{
    fnx::matrix4x4 a(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
    fnx::vector4 b(1,5,9,13);
    fnx::matrix4x4 e(0,-2,-4,-9,
                     2,0,-9,18,
                     4,9,0,22,
                     9,18,22,0);
    //auto c = a * b;
    // TODO
    //EXPECT_EQ(e,c);
}