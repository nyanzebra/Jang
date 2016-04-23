#pragma once

#include <memory>
#include <map>	
#include <string>
#include <functional>
#include <vector>

#include "io/console.h"
#include "macros/string.h"

namespace ftl {

#define MAJOR_SYMBOL '='
#define MINOR_SYMBOL '-'
#define SEPARATOR_SYMBOL '~'
#define FIELD_SIZE 40

#define SUCCESS_COLOR GREEN, BLACK
#define FAILURE_COLOR RED, BLACK
#define UNDECIDED_COLOR BLUE, BLACK

#define DEFAULT_COLOR_SCHEME WHITE, BLACK

class test_suite;

class test_registry {
public:
	test_registry(const test_registry&) = delete;
	test_registry(test_registry&&) = delete;
	~test_registry() = default;

	static test_registry& instance() {
		static test_registry instance;
		return instance;
	}

	template<typename T, typename = std::enable_if_t<std::is_base_of<test_suite, T>::value>>
	void insert(const std::string& name, T* suite) { _test_suites[name] = std::unique_ptr<T>(suite); }

	void run() { _run(); }

	const std::size_t size() const { return _test_suites.size(); }

private:
	test_registry() = default;

	void _run();

	const bool _menu(std::map<std::string, std::unique_ptr<test_suite>>::const_iterator it);

	void _print_options() {
		io::console::output_line(DEFAULT_COLOR_SCHEME, "Test Options:");
		io::console::output_line(DEFAULT_COLOR_SCHEME, "c <continue>\t\tto proceed to next suite");
		io::console::output_line(DEFAULT_COLOR_SCHEME, "r <repeat>\t\tto repeat the last test suite");
		io::console::output_line(DEFAULT_COLOR_SCHEME, "s <skip>\t\tto skip the next test suite");
		io::console::output_line(DEFAULT_COLOR_SCHEME, "l <list>\t\tto list the remaining test suites");
		io::console::output_line(DEFAULT_COLOR_SCHEME, "[testsuite]\t\tskips to testsuite, if haven't executed");
		io::console::output_line(DEFAULT_COLOR_SCHEME, "q <quit>\t\tto quit the test suite program");
	}

	const std::string _center(const std::string& name, const char& symbol) {
		std::string center(FIELD_SIZE / 2, symbol);
		center += " " + name + std::string(FIELD_SIZE / 2, symbol);
		return center;
	}

	void _header(const std::string& name, const char& symbol) { io::console::output_line(DEFAULT_COLOR_SCHEME, _center(name, symbol)); }

	void _footer(const char& symbol) { io::console::output_line(DEFAULT_COLOR_SCHEME, std::string(FIELD_SIZE, symbol), '\n'); }

	void _results(const size_t& successes, const size_t& failures, const size_t& undecided) {
		io::console::output_line();

		_header("RESULT", MAJOR_SYMBOL);

		io::console::output_line(SUCCESS_COLOR, "Tests passed: ", successes);
		io::console::output_line(FAILURE_COLOR, "Tests failed: ", failures);
		io::console::output_line(UNDECIDED_COLOR, "Tests undecided: ", undecided);

		_footer(MAJOR_SYMBOL);
		_footer(SEPARATOR_SYMBOL);
	}
private:
	std::map<std::string, std::unique_ptr<test_suite>> _test_suites;
};

class test_suite {
public:
	test_suite() = default;
	test_suite(const test_suite&) = delete;
	test_suite(test_suite&&) = delete;
	~test_suite() = default;

	virtual void init() = 0;

	const size_t& successes() { return _successes; }

	const size_t& failures() { return _failures; }

	const std::map<std::string, std::function<void()>>& tests() const { return _tests; }

	void print_errors() {
		for (auto diagnostic : _diagnostics) {
			io::console::output_line(RED, WHITE, diagnostic._file + " ", diagnostic._line, ": ", diagnostic._message);
		}
		_diagnostics.clear();
	}

protected:
	void add_diagnostic(const std::string& message, const std::string& file, const size_t& line) { _diagnostics.push_back(diagnostic(message, file, line)); }

protected:
	std::map<std::string, std::function<void()>> _tests;

	struct diagnostic {
		diagnostic(const std::string& message, const std::string& file, const size_t& line) : _message(message), _file(file), _line(line) {}
		~diagnostic() = default;
		std::string _message = "N/A";
		std::string _file = "N/A";
		size_t _line = 0;
	};

	std::vector<diagnostic> _diagnostics;

	size_t _successes = 0, _failures = 0;
};

const bool test_registry::_menu(std::map<std::string, std::unique_ptr<test_suite>>::const_iterator it) {
	_print_options();
	std::string input = "";
	io::console::input(input);

	if (!input.compare("c") || !input.compare("continue")) {
		return true;
	} else if (!input.compare("r") || !input.compare("repeat")) {
		--it;
		return true;
	} else if (!input.compare("s") || !input.compare("skip")) {
		if (it == _test_suites.end()) {
			return false;
		}
		++it;
		return true;
	} else if (!input.compare("l") || !input.compare("list")) {
		for (auto it = _test_suites.cbegin(); it != _test_suites.cend(); ++it) {
			io::console::output_line(UNDECIDED_COLOR, it->first);
		}
		io::console::output_line();
		_menu(it);
	} else if (!input.compare("q") || !input.compare("quit")) {
		return false;
	} else {
		io::console::output_line(FAILURE_COLOR, "ERROR:\tmust provide valid input");
		_menu(it);
	}

	return false;
}

void test_registry::_run() {
	for (auto it = _test_suites.cbegin(); it != _test_suites.cend(); ++it) {
		if (_menu(it)) {
			_header(it->first, MAJOR_SYMBOL);

			it->second->init();

			auto tests = it->second->tests();

			size_t successes = 0, failures = 0, undecided = 0;

			for (auto test : tests) {
				_header(test.first, MINOR_SYMBOL);

				test.second();

				if (successes < it->second->successes()) {
					successes = it->second->successes();
					io::console::output_line(SUCCESS_COLOR, "Passed...");
				} else if (failures < it->second->failures()) {
					failures = it->second->failures();
					io::console::output_line(FAILURE_COLOR, "Failed...");
					it->second->print_errors();
				} else {
					undecided++;
					io::console::output_line(UNDECIDED_COLOR, "Undecided...");
				}
			}

			_footer(MAJOR_SYMBOL);

			_results(successes, failures, undecided);
		} else {
			return;
		}
	}
}

#define TEST_SUITE(name)\
	class CONCATENATE(name, _test_suite) : public ftl::test_suite {\
	public:\
		CONCATENATE(name, _test_suite)() { ftl::test_registry::instance().insert<CONCATENATE(name, _test_suite)>(AS_STRING(name),this); }\
		CONCATENATE(name, _test_suite)(const CONCATENATE(name, _test_suite)&) =default;\
		CONCATENATE(name, _test_suite)(CONCATENATE(name, _test_suite)&&)=default;\
		~CONCATENATE(name, _test_suite)() = default;\
		virtual void init() override;\
	} CONCATENATE(name, _test_suite_instance);\
	void CONCATENATE(name, _test_suite)::init()

#define TEST(name) _tests[AS_STRING(name)] = [&]()

#define ASSERT(condition, message)\
	if (!condition) {\
		add_diagnostic(AS_STRING(condition message), __FILE__,__LINE__);\
		_failures++;\
	} else {\
		_successes++;\
	}

#define FAIL() ASSERT(false, ", forced failure.")

#define PASS() ASSERT(true, ", ERROR.")

#define ASSERT_TRUE(condition) ASSERT(condition, ", is a false condition")

#define ASSERT_FALSE(condition) ASSERT(!condition, ", is a true condition")

#define ASSERT_EQUAL(a, b) ASSERT_TRUE((a == b))

#define ASSERT_NOT_EQUAL(a,b) ASSERT_TRUE((a != b))

#define ASSERT_NULL(a) ASSERT_EQUAL(a, NULL)

#define ASSERT_NULLPTR(a) ASSERT_EQUAL(a, std::nullptr_t)

#define ASSERT_NOT_NULL(a) ASSERT_NOT_EQUAL(a, NULL)

#define ASSERT_NOT_NULLPTR(a) ASSERT_NOT_EQUAL(a, std::nullptr_t)

} // ftl
