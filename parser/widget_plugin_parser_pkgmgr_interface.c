#include <glib.h>
#include <libxml/tree.h>

#include <dlog.h>

#include "widget_plugin_parser_internal.h"

API int PKGMGR_PARSER_PLUGIN_PRE_INSTALL(const char *pkgid)
{
	return widget_plugin_parser_init();
}

API int PKGMGR_PARSER_PLUGIN_INSTALL(xmlDocPtr doc, const char *pkgid)
{
	GList *result;

	result = widget_plugin_parser_parse_manifest(doc);
	if (result == NULL) {
		LOGE("parse failed");
		return -1;
	}

	if (widget_parser_db_insert_widget_class(pkgid, result))
		return -1;

	g_list_free_full(result, _free_widget_class);

	return 0;
}

API int PKGMGR_PARSER_PLUGIN_POST_INSTALL(const char *pkgid)
{
	return widget_plugin_parser_fini();
}

API int PKGMGR_PARSER_PLUGIN_PRE_UPGRADE(const char *pkgid)
{
	return widget_plugin_parser_init();
}

API int PKGMGR_PARSER_PLUGIN_UPGRADE(xmlDocPtr doc, const char *pkgid)
{
	GList *result;

	result = widget_plugin_parser_parse_manifest(doc);
	if (result == NULL) {
		LOGE("parse failed");
		return -1;
	}

	if (widget_parser_db_remove_widget_class(pkgid))
		return -1;

	if (widget_parser_db_insert_widget_class(pkgid, result))
		return -1;

	g_list_free_full(result, _free_widget_class);

	return 0;
}

API int PKGMGR_PARSER_PLUGIN_POST_UPGRADE(const char *pkgid)
{
	return widget_plugin_parser_fini();
}

API int PKGMGR_PARSER_PLUGIN_PRE_UNINSTALL(const char *pkgid)
{
	return widget_plugin_parser_init();
}

API int PKGMGR_PARSER_PLUGIN_UNINSTALL(xmlDocPtr doc, const char *pkgid)
{
	return widget_parser_db_remove_widget_class(pkgid);
}

API int PKGMGR_PARSER_PLUGIN_POST_UNINSTALL(const char *pkgid)
{
	return widget_plugin_parser_fini();
}
