#include <test>

#include "memory/test_suite.h"

int main(int argc, char const *argv[]) {

	jang::test_registry::instance().run();

	jang::io::console::output_line(DEFAULT_COLOR_SCHEME, jang::test_registry::instance().size());

	return 0;
}
