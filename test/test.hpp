#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <typeinfo>
#include <type_traits>
#include <exception>
#include <cstddef>       // nullptr_t
#include <cmath>         // abs
#include <chrono>
#include <sstream>

namespace test
{
    static const int PASS = 0;
    static const int FAIL = 1;
    namespace internal
    {
        namespace color
        {
            enum ansi
            {
                fg_red = 31,
                fg_green = 32,
                fg_yellow = 33,
                fg_blue = 34,
                fg_dflt = 39,
                bg_red = 41,
                bg_green = 42,
                bg_blue = 44,
                bg_dflt = 49,
                reset = 0
            };

            struct modifier
            {
                constexpr static const char* _cmd = "\033[";
                color::ansi _val{ fg_dflt };

                constexpr explicit modifier(ansi val) : _val(val) {}

                friend std::ostream& operator<<(std::ostream& os, const modifier& mod)
                {
                    return os << _cmd << mod._val << "m";
                }
            };

            constexpr static const auto FG_RED = color::modifier(ansi::fg_red);
            constexpr static const auto FG_GRN = color::modifier(ansi::fg_green);
            constexpr static const auto FG_YLW = color::modifier(ansi::fg_yellow);
            constexpr static const auto RESET = color::modifier(ansi::reset);
        }

        namespace check
        {
            struct no {};

            template<typename U, typename T>
            struct is_explicitly_convertible
            {
                enum { value = std::is_constructible<T, U>::value && !std::is_convertible<U, T>::value };
            };

            template<typename T, typename U> no operator==(const T&, const U&);
            template<typename T, typename U> no operator!=(const T&, const U&);
            template<typename T, typename U> no operator<(const T&, const U&);
            template<typename T, typename U> no operator<=(const T&, const U&);
            template<typename T, typename U> no operator>(const T&, const U&);
            template<typename T, typename U> no operator>=(const T&, const U&);
            template<typename T, typename U> no operator<<(const T&, const U&);

            template<typename T, typename U = T>
            struct eq_exists
            {
                enum { value = !std::is_same<decltype(*(T*)(0) == *(U*)(0)), no>::value };
            };

            template<typename T, typename U = T>
            struct ne_exists
            {
                enum { value = !std::is_same<decltype(*(T*)(0) != *(U*)(0)), no>::value };
            };

            template<typename T, typename U = T>
            struct lt_exists
            {
                enum { value = !std::is_same<decltype(*(T*)(0) < *(U*)(0)), no>::value };
            };

            template<typename T, typename U = T>
            struct lte_exists
            {
                enum { value = !std::is_same<decltype(*(T*)(0) <= *(U*)(0)), no>::value };
            };

            template<typename T, typename U = T>
            struct gt_exists
            {
                enum { value = !std::is_same<decltype(*(T*)(0) > * (U*)(0)), no>::value };
            };

            template<typename T, typename U = T>
            struct gte_exists
            {
                enum { value = !std::is_same<decltype(*(T*)(0) >= *(U*)(0)), no>::value };
            };

            template<typename T, typename U = T>
            struct stream_exists
            {
                enum { value = !std::is_same<decltype(*(T*)(0) << *(U*)(0)), no>::value };
            };

            template <typename T, class = void>
            struct is_streamable : std::false_type { using type = void; };

            template <typename T>
            struct is_streamable<T, std::enable_if_t<
                std::is_convertible_v<
                decltype(std::declval<std::ostream&>() << std::declval<T>()),
                std::ostream&
                >
                >> : std::true_type {
                    using type = T;
                };
        }

        struct fail : public std::exception
        {
            const char* what() const throw()
            {
                return "Test Case Failure";
            }
        };

        constexpr static const auto ULP = 2;
    }

    class TestCase
    {
    public:
        virtual void run() = 0;
        auto name()
        {
            return _feature + "." + _name;
        }

        auto disabled()
        {
            return _disabled;
        }

        TestCase(const std::string& feature, const std::string& name, bool disabled = false)
            : _feature(feature)
            , _name(name)
            , _disabled(disabled)
        {
        }
        TestCase() = default;
        virtual ~TestCase() = default;
    private:
        std::string _feature;
        std::string _name;
        bool _disabled;
    };

    class Registry
    {
    public:
        static Registry& inst()
        {
            static Registry instance;
            return instance;
        }

        static int& result()
        {
            static int instance{ PASS };
            return instance;
        }

        void register_on_start_callback(void(*callback)(const std::string& msg))
        {
            _start_callback = callback;
        }

        void register_on_end_callback(void(*callback)(const std::string& msg, bool pass))
        {
            _end_callback = callback;
        }

        void register_on_test_start_callback(void(*callback)(const std::string& msg))
        {
            _test_start_callback = callback;
        }

        void register_on_test_end_callback(void(*callback)(const std::string& msg, bool pass))
        {
            _test_end_callback = callback;
        }

        template<typename TestCaseType>
        TestCase* create(const char* test_case_name)
        {
            _total_tests++;
            _tests[test_case_name].emplace_back(new TestCaseType());
            return _tests[test_case_name].back();
        }

        bool isFiltered(const std::string& filter, const std::string& testName)
        {
            if (filter.empty())
                return false;

            {
                //cout << "i:  " << i << endl;
                for (std::size_t i = 0, k = 0; k < testName.size();)
                {
                    //cout << "k:  " << k << endl;
                    if (filter[i] == '*')
                    {
                        // loop through until a match is found for the next filter item
                        while (k < testName.size())
                        {
                            if (i + 1 < filter.size())
                            {
                                // still more wildcard to parse
                                if (filter[i + 1] == testName[k])
                                {
                                    // the current character matches the next filter character
                                    i++;
                                    break;
                                }
                                else
                                {
                                    // still in while card
                                }
                            }
                            else
                            {
                                // wild card is last char
                                return false;
                            }
                            k++;
                        }
                    }
                    else if (i < filter.size())
                    {
                        // check that characters match
                        if (filter[i] != testName[k])
                        {
                            return true;
                        }
                        else
                        {
                            i++;
                            k++;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
            return false;
        }

        int run_all_tests(int argc = 0, char* argv[] = nullptr)
        {
            std::string filter = "";
            for (int i = 1; i < argc; i++)
            {
                auto line = std::string(argv[i]);
                auto pos = line.find("--filter=");
                filter = line.substr(pos + 9);
            }
            auto count = 0;
            std::vector<std::string> failed_tests;
            std::vector<std::string> disabled_tests;
            auto skipped_tests = 0;

            std::cout << "[==========] " << "Running " << _tests.size() << " tests" << std::endl;
            if(!filter.empty())
                std::cout << "[----------] " << "Filtering on " << filter << std::endl;

            if (_start_callback) _start_callback("Starting Unit Tests");

            auto start = std::chrono::high_resolution_clock::now();
            
            for (const auto& class_vec : _tests)
            {
                auto c_start = std::chrono::high_resolution_clock::now();
                std::cout << "[----------] " << class_vec.second.size() << " tests for " << class_vec.first << std::endl;
                for (const auto& test : class_vec.second)
                {
                    if (isFiltered(filter, test->name()))
                    {
                        skipped_tests++;
                        continue;
                    }
                    if (_test_start_callback) _test_start_callback(std::string("Running Test ") + test->name());
                    std::cout << test::internal::color::RESET;
                    auto t_start = std::chrono::high_resolution_clock::now();
                    auto t_end = t_start;

                    if (!test->disabled())
                    {
                        bool result = false;
                        std::cout << "[ RUN      ] " << test->name() << std::endl;
                        try
                        {
                            Registry::result() = PASS;
                            test->run();
                            t_end = std::chrono::high_resolution_clock::now();
                            if (FAIL == Registry::result())
                            {
                                // test case failed
                                std::cout << "[  " << test::internal::color::FG_RED << "FAILED" <<
                                    test::internal::color::RESET << "  ] " << test->name() << " (" << std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start).count() << " ms)" << std::endl;
                                failed_tests.push_back(test->name());
                            }
                            else
                            {
                                // test case passed
                                std::cout << "[       " << test::internal::color::FG_GRN << "OK" <<
                                    test::internal::color::RESET << " ] " << test->name() << " (" << std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start).count() << " ms)" << std::endl;
                                result = true;
                                count++;
                            }
                        }
                        catch (test::internal::fail& /*e*/)
                        {
                            // test case failed
                            t_end = std::chrono::high_resolution_clock::now();
                            std::cout << "[  FAILED  ] " << test->name() << std::endl;
                            failed_tests.push_back(test->name());
                        }
                        catch (const std::exception& e)
                        {
                            // user application threw an error
                            std::cout << "[     EXCP ] " << test->name();// << std::endl;
                            std::cout << " e: " << e.what() << std::endl;
                            failed_tests.push_back(test->name());
                        }
                        if (_test_end_callback) _test_end_callback(std::string("Finished Test ") + test->name(), result);
                    }
                    else
                    {
                        disabled_tests.push_back(test->name());
                    }
                }
                auto c_end = std::chrono::high_resolution_clock::now();
                std::cout << "[----------] " << class_vec.second.size() << " test(s) from " << class_vec.first << " (" 
                        << std::chrono::duration_cast<std::chrono::milliseconds>(c_end - c_start).count() << " ms)" << std::endl;
                std::cout << std::endl;
            }

            auto end = std::chrono::high_resolution_clock::now();

            if (_end_callback) _end_callback("Finished Unit Tests", failed_tests.empty());

            std::cout << "[==========] " << (_total_tests - skipped_tests) << " tests (" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms)" << std::endl;
            auto passed_tests = _total_tests - skipped_tests - failed_tests.size() - disabled_tests.size();
            std::cout << "[  " << test::internal::color::FG_GRN << "PASSED" << test::internal::color::RESET << "  ] " << passed_tests << " tests" << std::endl;

            if (skipped_tests > 0)
            {
                // at least one disabled test
                std::cout << "[ " << test::internal::color::FG_YLW << "SKIPPED " << test::internal::color::RESET << " ] " << skipped_tests << " tests" << std::endl;
            }

            if (!disabled_tests.empty())
            {
                // at least one disabled test
                std::cout << "[ " << test::internal::color::FG_YLW << "DISABLED" << test::internal::color::RESET << " ] " << disabled_tests.size() << " tests" << std::endl;
                for (const auto& test : disabled_tests)
                {
                    std::cout << "[ " << test::internal::color::FG_YLW << "DISABLED" << test::internal::color::RESET << " ] " << test << std::endl;
                }
            }

            if (!failed_tests.empty())
            {
                // at least one failure
                std::cout << "[  " << test::internal::color::FG_RED << "FAILED" << test::internal::color::RESET << "  ] " << failed_tests.size() << " tests" << std::endl;

                for (const auto& test : failed_tests)
                {
                    std::cout << "[  " << test::internal::color::FG_RED << "FAILED" << test::internal::color::RESET << "  ] " << test << std::endl;
                }
            }
            
            return failed_tests.empty();
        }

        ~Registry()
        {
            for (auto class_vec : _tests)
            {
                for (auto test : class_vec.second)
                {
                    delete test;
                    test = nullptr;
                }
            }

            _tests.clear();
        }

    private:
        Registry() = default;
        unsigned int _total_tests{ 0u };

        std::unordered_map<std::string, std::vector<TestCase*>> _tests;

        void(*_test_start_callback)(const std::string& msg) = nullptr;
        void(*_test_end_callback)(const std::string& msg, bool pass) = nullptr;
        void(*_start_callback)(const std::string& msg) = nullptr;
        void(*_end_callback)(const std::string& msg, bool pass) = nullptr;
    };

    template<typename T, typename U>
    struct matcher_base
    {
        auto& operator<<(std::ostream& os) { return os << to_string(); }
        virtual std::string to_string() const { return "na"; }
        virtual bool check() const { return false; }
        virtual bool exec(const T& lhs, const U& rhs) const { return false; }
    };

    template<typename T, typename U>
    struct matcher_ne : matcher_base<T, U>
    {
        std::string to_string() const override { return "!="; }
        bool check() const override { return test::internal::check::ne_exists<T, U>::value; }
        bool exec(const T& lhs, const U& rhs) const override { return lhs != rhs; }
    };

    template<typename T, typename U>
    struct matcher_eq : matcher_base<T, U>
    {
        std::string to_string() const override { return "=="; }
        bool check() const override { return test::internal::check::eq_exists<T, U>::value; }
        bool exec(const T& lhs, const U& rhs) const override { return lhs == static_cast<T>(rhs); }
    };

    template<typename T, typename U>
    struct matcher_aleq : matcher_base<T, U>
    {
        std::string to_string() const override { return "~="; }
        bool check() const override { return true; }
        bool exec(const T& lhs, const U& rhs) const override
        {
            return std::abs(lhs - rhs) <= std::numeric_limits<T>::epsilon() * std::abs(lhs + rhs) * test::internal::ULP
                // unless the result is subnormal
                // encapsulate the call to min to avoid macro expansion
                || std::abs(lhs - rhs) < (std::numeric_limits<T>::min)();
        }
    };

    template<typename T, typename U>
    struct matcher_lt : matcher_base<T, U>
    {
        std::string to_string() const override { return "<"; }
        bool check() const override { return test::internal::check::lt_exists<T, U>::value; }
        bool exec(const T& lhs, const U& rhs) const override { return lhs < rhs; }
    };

    template<typename T, typename U>
    struct matcher_lte : matcher_base<T, U>
    {
        std::string to_string() const override { return "<="; }
        bool check() const override { return test::internal::check::lte_exists<T, U>::value; }
        bool exec(const T& lhs, const U& rhs) const override { return lhs <= rhs; }
    };

    template<typename T, typename U>
    struct matcher_gt : matcher_base<T, U>
    {
        std::string to_string() const override { return ">"; }
        bool check() const override { return test::internal::check::gt_exists<T, U>::value; }
        bool exec(const T& lhs, const U& rhs) const override { return lhs > rhs; }
    };

    template<typename T, typename U>
    struct matcher_gte : matcher_base<T, U>
    {
        std::string to_string() const override { return ">="; }
        bool check() const override { return test::internal::check::gte_exists<T, U>::value; }
        bool exec(const T& lhs, const U& rhs) const override { return lhs >= rhs; }
    };


    template<typename T, typename std::enable_if<!test::internal::check::is_streamable<T>::value, T>::type * = nullptr>
    inline std::string print_helper(const T& in)
    {
        return typeid(T).name();
    }

    template<typename T, typename std::enable_if<test::internal::check::is_streamable<T>::value && !std::is_pointer<T>::value, T>::type * = nullptr>
    inline std::string print_helper(const T& in)
    {
        // use the overloaded operator
        std::stringstream ss;
        ss << in;
        return ss.str();
    }

    template<typename T, typename std::enable_if<test::internal::check::is_streamable<T>::value>::type * = nullptr>
    inline std::string print_helper(const T* in)
    {
        // use the overloaded operator
        std::stringstream ss;
        ss << (*in);
        return ss.str();
    }

    template<typename T, typename std::enable_if<!test::internal::check::is_streamable<T>::value>::type * = nullptr>
    inline std::string print_helper(const T* in)
    {
        return typeid(T).name();
    }

    static inline std::string print_helper(const nullptr_t& in)
    {
        return "null";
    }

    template<typename T, typename U>
    static inline void handle_comp(const matcher_base<T, U>& matcher, const T& lhs, const U& rhs,
        const char* file_name, int line, bool halt_on_fail = false)
    {
        if (!matcher.check() && !std::is_constructible<T, U>::value)
        {
            // failed, print the information we need to find the failure
            std::cout << "[          ] " << matcher.to_string() << " operator missing @ " << file_name <<
                "@" << ":" << line << std::endl;
            if (halt_on_fail)
            {
                throw test::internal::fail();
            }

            Registry::result() = FAIL;
        }
        else if (!matcher.exec(lhs, rhs))
        {
            // this test failed
            std::cout << "[          ] comparison failure (" << print_helper(lhs) << matcher.to_string() << print_helper(rhs) << ") @ " << file_name <<
                ":" << line << std::endl;
            if (halt_on_fail)
            {
                throw test::internal::fail();
            }

            Registry::result() = FAIL;
        }
    }

    namespace internal
    {
        template<typename T, typename U>
        static inline void EXPECT_EQ(const char* file_name, int line, const T& lhs, const U& rhs)
        {
            handle_comp(matcher_eq<T, U>(), lhs, rhs, file_name, line);
        }

        template<typename T, typename U>
        static inline void EXPECT_ALMOST_EQ(const char* file_name, int line, const T& lhs, const U& rhs)
        {
            handle_comp(matcher_aleq<T, U>(), lhs, rhs, file_name, line);
        }

        template<typename T, typename U>
        inline void ASSERT_ALMOST_EQ(const char* file_name, int line, const T& lhs, const U& rhs)
        {
            handle_comp(matcher_aleq<T, U>(), lhs, rhs, file_name, line, true);
        }

        template<typename T, typename U>
        inline void ASSERT_EQ(const char* file_name, int line, const T& lhs, const U& rhs)
        {
            handle_comp(matcher_eq<T, U>(), lhs, rhs, file_name, line, true);
        }

        template<typename T, typename U>
        static inline void EXPECT_NE(const char* file_name, int line, const T& lhs, const U& rhs)
        {
            handle_comp(matcher_ne<T, U>(), lhs, rhs, file_name, line);
        }

        template<typename T, typename U>
        inline void ASSERT_NE(const char* file_name, int line, const T& lhs, const U& rhs)
        {
            handle_comp(matcher_ne<T, U>(), lhs, rhs, file_name, line, true);
        }

        template<typename T, typename U>
        static inline void EXPECT_LT(const char* file_name, int line, const T& lhs, const U& rhs)
        {
            handle_comp(matcher_lt<T, U>(), lhs, rhs, file_name, line);
        }

        template<typename T, typename U>
        inline void ASSERT_LT(const char* file_name, int line, const T& lhs, const U& rhs)
        {
            handle_comp(matcher_lt<T, U>(), lhs, rhs, file_name, line, true);
        }

        template<typename T, typename U>
        static inline void EXPECT_LTE(const char* file_name, int line, const T& lhs, const U& rhs)
        {
            handle_comp(matcher_lte<T, U>(), lhs, rhs, file_name, line);
        }

        template<typename T, typename U>
        inline void ASSERT_LTE(const char* file_name, int line, const T& lhs, const U& rhs)
        {
            handle_comp(matcher_lte<T, U>(), lhs, rhs, file_name, line, true);
        }

        template<typename T, typename U>
        static inline void EXPECT_GT(const char* file_name, int line, const T& lhs, const U& rhs)
        {
            handle_comp(matcher_gt<T, U>(), lhs, rhs, file_name, line);
        }

        template<typename T, typename U>
        inline void ASSERT_GT(const char* file_name, int line, const T& lhs, const U& rhs)
        {
            handle_comp(matcher_gt<T, U>(), lhs, rhs, file_name, line, true);
        }

        template<typename T, typename U>
        static inline void EXPECT_GTE(const char* file_name, int line, const T& lhs, const U& rhs)
        {
            handle_comp(matcher_gte<T, U>(), lhs, rhs, file_name, line);
        }

        template<typename T, typename U>
        inline void ASSERT_GTE(const char* file_name, int line, const T& lhs, const U& rhs)
        {
            handle_comp(matcher_gte<T, U>(), lhs, rhs, file_name, line, true);
        }

        template<typename T>
        static inline void EXPECT_TRUE(const char* file_name, int line, const T& lhs)
        {
            handle_comp(matcher_eq<T, bool>(), lhs, true, file_name, line);
        }

        template<typename T>
        inline void ASSERT_TRUE(const char* file_name, int line, const T& lhs)
        {
            handle_comp(matcher_eq<T, bool>(), lhs, true, file_name, line, true);
        }

        template<typename T>
        static inline void EXPECT_FALSE(const char* file_name, int line, const T& lhs)
        {
            handle_comp(matcher_eq<T, bool>(), lhs, false, file_name, line);
        }

        template<typename T>
        inline void ASSERT_FALSE(const char* file_name, int line, const T& lhs)
        {
            handle_comp(matcher_eq<T, bool>(), lhs, false, file_name, line, true);
        }

        template<typename T>
        static inline void EXPECT_NULL(const char* file_name, int line, const T& lhs)
        {
            handle_comp(matcher_eq<T, nullptr_t>(), lhs, nullptr, file_name, line);
        }

        template<typename T>
        inline void ASSERT_NULL(const char* file_name, int line, const T& lhs)
        {
            handle_comp(matcher_eq<T, nullptr_t>(), lhs, nullptr, file_name, line, true);
        }
    }

#define EXPECT_EQ(...)        test::internal::EXPECT_EQ(__FILE__,__LINE__,__VA_ARGS__)
#define EXPECT_ALMOST_EQ(...) test::internal::EXPECT_ALMOST_EQ(__FILE__,__LINE__,__VA_ARGS__)
#define EXPECT_LT(...)        test::internal::EXPECT_LT(__FILE__,__LINE__,__VA_ARGS__)
#define EXPECT_LTE(...)       test::internal::EXPECT_LTE(__FILE__,__LINE__,__VA_ARGS__)
#define EXPECT_GT(...)        test::internal::EXPECT_GT(__FILE__,__LINE__,__VA_ARGS__)
#define EXPECT_GTE(...)       test::internal::EXPECT_GTE(__FILE__,__LINE__,__VA_ARGS__)
#define EXPECT_NE(...)        test::internal::EXPECT_NE(__FILE__,__LINE__,__VA_ARGS__)
#define EXPECT_TRUE(...)      test::internal::EXPECT_TRUE(__FILE__,__LINE__,__VA_ARGS__)
#define EXPECT_FALSE(...)     test::internal::EXPECT_FALSE(__FILE__,__LINE__,__VA_ARGS__)
#define EXPECT_NULL(...)      test::internal::EXPECT_NULL(__FILE__,__LINE__,__VA_ARGS__)

#define ASSERT_EQ(...)        test::internal::ASSERT_EQ(__FILE__,__LINE__,__VA_ARGS__)
#define ASSERT_ALMOST_EQ(...) test::internal::ASSERT_ALMOST_EQ(__FILE__,__LINE__,__VA_ARGS__)
#define ASSERT_LT(...)        test::internal::ASSERT_LT(__FILE__,__LINE__,__VA_ARGS__)
#define ASSERT_LTE(...)       test::internal::ASSERT_LTE(__FILE__,__LINE__,__VA_ARGS__)
#define ASSERT_GT(...)        test::internal::ASSERT_GT(__FILE__,__LINE__,__VA_ARGS__)
#define ASSERT_GTE(...)       test::internal::ASSERT_GTE(__FILE__,__LINE__,__VA_ARGS__)
#define ASSERT_NE(...)        test::internal::ASSERT_NE(__FILE__,__LINE__,__VA_ARGS__)
#define ASSERT_TRUE(...)      test::internal::ASSERT_TRUE(__FILE__,__LINE__,__VA_ARGS__)
#define ASSERT_FALSE(...)     test::internal::ASSERT_FALSE(__FILE__,__LINE__,__VA_ARGS__)
#define ASSERT_NULL(...)      test::internal::ASSERT_NULL(__FILE__,__LINE__,__VA_ARGS__)
#define TOKEN_PASTEx(x, y) x ## y
#define TOKEN_PASTE(x, y) TOKEN_PASTEx(x, y)

#define TEST(testcase, testname) \
        class testcase##testname : public test::TestCase{ \
        public: \
            testcase##testname(): TestCase(#testcase, #testname){\
            } \
            virtual void run() final; \
            ~testcase##testname() = default; \
       private: \
            static const TestCase* _self; \
    }; \
    const test::TestCase* testcase##testname::_self = test::Registry::inst().create<testcase##testname>(#testcase); \
    void testcase##testname::run()

#define TEST_DISABLED(testcase, testname) \
        class testcase##testname : public test::TestCase{ \
        public: \
            testcase##testname(): TestCase(#testcase, #testname, true){\
            } \
            virtual void run() final; \
            ~testcase##testname() = default; \
        private: \
            static const TestCase* _self; \
    }; \
    const test::TestCase* testcase##testname::_self = test::Registry::inst().create<testcase##testname>(#testcase); \
    void testcase##testname::run()

    // no overload for nullptr exists, add one
    template<typename C, typename T>
    std::basic_ostream<C, T>& operator<<(std::basic_ostream<C, T>& os, std::nullptr_t)
    {
        return os << (void*) nullptr;
    }
}