#include "test.hpp"

int main(int argc, char* argv[])
{
	//fnx::log_info("unit.log", false)("# test library");
	//auto on_start = [](const std::string& msg) { fnx::log_info("unit.log")(msg); };
	//auto on_end = [](const std::string& msg, bool pass) { fnx::log_info("unit.log")(msg) << (pass ? "Pass" : "Fail"); };
	//auto on_test_start = [](const std::string& msg) { fnx::log_info("unit.log")(msg); };
	//auto on_test_end = [](const std::string& msg, bool pass) { fnx::log_info("unit.log")(msg) << (pass ? "Pass" : "Fail"); };
	auto& reg = test::Registry::inst();
	//reg.register_on_start_callback(on_start);
	//reg.register_on_end_callback(on_end);
	//reg.register_on_test_start_callback(on_test_start);
	//reg.register_on_test_end_callback(on_test_end);
	return reg.run_all_tests(argc, argv);
}