#include <stdio.h>
#include <stdlib.h>
#include <widget_service.h>
#include <string.h>
#include <aul.h>

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

static int list_cb(const char *pkgid, const char *widget_id, int is_prime,
		void *data)
{
	printf("pkgid [%s]\twidget_id [%s]\tis_prime [%d]\n", pkgid, widget_id,
			is_prime);
	return 0;
}

int get_widget_list(int argc, char **argv)
{
	if (widget_service_get_widget_list(list_cb, NULL) < 0) {
		printf("failed to get widget list\n");
		return -1;
	}

	return 0;
}

int get_content(int argc, char **argv)
{
	bundle *b;
	int ret;
	char *content_info = NULL;

	ret = widget_service_get_content_of_widget_instance(argv[2], argv[3], &b);
	if (b)
		bundle_get_str(b, AUL_K_WIDGET_CONTENT_INFO, &content_info);

	printf("ret:%d %s\n", ret, content_info);
	return 0;
}

test_func_t test_func[] = {
	{"get_pkg_id", get_pkg_id, "<widget_id>"},
	{"get_widget_list", get_widget_list, ""},
	{"get_content", get_content, ""},
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
