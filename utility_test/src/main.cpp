#include "test.h"

#include "memory/test_suite.h"

int main(int argc, char const *argv[]) {

	ftl::test_registry::instance().run();

	ftl::io::console::output_line(DEFAULT_COLOR_SCHEME, ftl::test_registry::instance().size());

	return 0;
}
