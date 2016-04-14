#include <stdio.h>
#include <stdlib.h>
#include <widget_service.h>
#include <string.h>

typedef int (*test_fn)(int argc, char **argv);

typedef struct _test_func_t {
	const char *cmd;
	test_fn fn;
	const char *usage;
} test_func_t;

int get_pkg_id(int argc, char **argv)
{
	char *pkg_id;

	if (argc < 1)
		return -1;

	pkg_id = widget_service_get_package_id(argv[2]);
	printf("pkg id: %s", pkg_id);

	return 0;
}

test_func_t test_func[] = {
	{"get_pkg_id", get_pkg_id, "<widget_id>"}
};

static void print_usage(char *pname)
{
	int i = 0;
	printf("[usage] %s <cmd> ...\n", pname);
	printf(" - available cmd list \n");

	for (i = 0; i < sizeof(test_func) / sizeof(test_func_t); i++)
		printf("%s %s %s\n\n", pname, test_func[i].cmd, test_func[i].usage);
	printf(" - end cmd list\n");
}

int main(int argc, char **argv)
{
	int i = 0;

	if (argc < 2) {
		print_usage(argv[0]);
		exit(0);
	}

	for (i = 0; i < (sizeof(test_func) / sizeof(test_func_t)); i++) {
		if (strcmp(test_func[i].cmd, argv[1]) == 0)
			return test_func[i].fn(argc, argv);
	}

	print_usage(argv[0]);

	return 0;
}
