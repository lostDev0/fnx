#include "test.hpp"
#include "fnx/fnx.hpp"

namespace tester
{
	static int counter = 0;
	void increment() { tester::counter++; }
	void decrement() { tester::counter--; }
	
	struct payload
	{
		int val{ 0 };
		payload(int v) : val{ v } {}
		payload() {}
		~payload() {}
	};

	bool handle_payload(const payload& p) { tester::counter = p.val; return false; }

	struct foo_i
	{
		foo_i() {};
		void set_x(int) {};
		virtual ~foo_i() {};
	};

	struct foo : foo_i
	{
		int x{ 0 };
		int y{ 1 };

		void set_x(int x) { this->x = x; }
		void set_y(int y) { this->y = y; }
		bool handle_payload(const payload& p) { x = p.val; return false; }

		bool operator==(const foo& other) const { return x == other.x && y == other.y; }
		bool operator!=(const foo& other) const { return !(*this == other); }

        foo() = default;
        ~foo() = default;
        foo(int x, int y) : x(x), y(y) {}
	};

	int test_val = 0;
	bool set_test_val(foo f)
	{
		test_val = f.x;
		return true;
	}

	bool ignore_test_val(foo f) { return true; }

	struct callback_foo
	{
		callback_foo()
			: c{ fnx::bind(*this, &callback_foo::set_x) }
		{
		}
		void set_x(int x) { this->x = x; }
		fnx::function_ref<void(int)> c;
		int x{ 0 };
	};

	// test create, assign, remove, copy, destroy
	int create{ 0 }, assign{ 0 }, remove{ 0 }, destroy{ 0 };

	//auto create_callback(const fnx::entity_created_evt& evt) {create++; return true; };
	//auto destroy_callback(const fnx::entity_destroyed_evt& evt) {destroy++; return true; };
	//auto assign_callback(const fnx::component_added_evt<fnx::transform>& evt) {assign++; return true; };
	//auto remove_callback(const fnx::component_removed_evt<fnx::transform>& evt) {remove++; return true; };
}

using namespace tester;
using namespace fnx;


TEST(memory, heap_allocator)
{
    fnx::heap_pool_allocator<tester::foo> pool;
    auto val = pool.create(42, 1337);
    EXPECT_EQ(42, val->x);
    EXPECT_EQ(1337, val->y);
    // note that destroy doesn't change the pointer to null
    pool.destroy(val);
}

TEST(function_ref, global)
{
	auto func = fnx::bind(&tester::set_test_val);
	tester::foo obj;
	obj.x = 123;
	func(obj);
	EXPECT_EQ(123, tester::test_val);
}

TEST(function_ref, member)
{
	tester::foo obj;
	auto func = fnx::bind(obj, &tester::foo::set_x);
	func(456);
	EXPECT_EQ(456, obj.x);
}

TEST(function_ref, comparison)
{
	auto func = fnx::bind(&tester::set_test_val);
	auto func2 = fnx::bind(&tester::set_test_val);
	auto func3 = fnx::bind(&tester::ignore_test_val);

	EXPECT_EQ(func, func2);
	EXPECT_NE(func, func3);

	tester::foo obj;
	tester::foo obj2;

	auto func4 = fnx::bind(obj, &tester::foo::set_x);
	auto func5 = fnx::bind(obj, &tester::foo::set_y);
	auto func6 = fnx::bind(obj, &tester::foo::set_x);
	auto func7 = fnx::bind(obj2, &tester::foo::set_x);

	EXPECT_EQ(func4, func6);
	EXPECT_NE(func4, func5);
	EXPECT_NE(func4, func7);
	EXPECT_EQ(func4, fnx::bind(obj, &tester::foo::set_x));
}

TEST(function_ref, copy)
{
	tester::foo obj;
	auto original = fnx::bind(obj, &tester::foo::set_x);
	auto clone1(original);
	auto clone2 = original;

	clone1(42);
	EXPECT_EQ(obj.x, 42);
	clone2(123);
	EXPECT_EQ(obj.x, 123);
}

TEST(function_ref, container)
{
	tester::foo obj;
	std::vector<fnx::function_ref<void(int)>> vs;
	for (auto i = 0; i < 10; ++i)
	{
		vs.emplace_back(fnx::bind(obj, &tester::foo::set_x));
	}

	int i = 42;
	for (const auto& f : vs)
	{
		f(i);
		EXPECT_EQ(obj.x, i);
		i++;
	}
}

TEST(function_ref, ctor)
{
	tester::callback_foo obj;
	obj.c(42);
	EXPECT_EQ(obj.x, 42);
}

TEST(function_ref, bind)
{
	tester::foo obj;
	auto r = fnx::bind(obj, &tester::foo::set_x);
	r(42);
	EXPECT_EQ(obj.x, 42);
}