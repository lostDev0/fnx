#include "test.hpp"
#include "fnx/fnx.hpp"

TEST(id_manager, reclaim)
{
	fnx::id_manager<int, 2> id_manager;
    auto val = id_manager.get_handle();
    EXPECT_EQ(0, val);
    EXPECT_EQ(1, id_manager.available().size());
    id_manager.reclaim(0);  // id that was provided
	id_manager.reclaim(1);  // id that wasn't provided
	id_manager.reclaim(2);  // not an id that could be provided
	EXPECT_EQ(2, id_manager.available().size());
}

TEST(singleton, simple)
{
    {
        auto [val, _] = fnx::singleton<int>::acquire();
        EXPECT_EQ(0, val);
        val = 42;
    }
    {
        auto [val, _] = fnx::singleton<int>::acquire();
        EXPECT_EQ(42, val);
    }
}

TEST(tween, int)
{
    auto val = fnx::tween<int>(0,10);
    EXPECT_EQ(0, val.get(0.0));
    EXPECT_EQ(3, val.get(.33));
    EXPECT_EQ(5, val.get(.5));
    EXPECT_EQ(10, val.get(1.0));
}

TEST(tween, float)
{
    auto val = fnx::tween<float>(0.f,10.f);
    EXPECT_ALMOST_EQ(0.f, val.get(0.0));
    EXPECT_ALMOST_EQ(3.3f, val.get(.33));
    EXPECT_ALMOST_EQ(5.f, val.get(.5));
    EXPECT_ALMOST_EQ(10.f, val.get(1.0));
}