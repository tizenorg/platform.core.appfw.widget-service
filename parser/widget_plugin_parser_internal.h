#ifndef __WIDGET_PLUGIN_PARSER_INTERNAL_H__
#define __WIDGET_PLUGIN_PARSER_INTERNAL_H__

#include <stdbool.h>
#include <sys/types.h>

#include <libxml/tree.h>
#include <sqlite3.h>
#include <glib.h>

#ifndef API
#define API __attribute__ ((visibility("default")))
#endif

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "WIDGET_PLUGIN_PARSER"
#endif

struct support_size {
	char *preview;
	bool frame;
	int width;
	int height;
};

struct label {
	char *label;
	char *lang;
};

struct icon {
	char *icon;
	char *lang;
};

struct widget_class {
	char *classid;
	int update_period;
	char *setup_appid;
	char *appid;
	GList *support_size;
	GList *label;
	GList *icon;
};

int widget_plugin_parser_init(void);
int widget_plugin_parser_fini(void);
GList *widget_plugin_parser_parse_manifest(xmlDocPtr doc);


int widget_parser_db_insert_widget_class(const char *pkgid, GList *widget_list);
int widget_parser_db_remove_widget_class(const char *pkgid);

void _free_widget_class(gpointer data);

sqlite3 *_open_db(uid_t uid, bool readonly);
void _close_db(sqlite3 *db);

int widget_info_get_widget_class(const char *classid, struct widget_class **wc);

#endif
