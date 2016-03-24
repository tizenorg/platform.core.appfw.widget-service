/*
 * Copyright 2015  Samsung Electronics Co., Ltd
 *
 * Licensed under the Flora License, Version 1.1 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://floralicense.org/license/
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>

#include <libxml/tree.h>
#include <glib.h>

#include <dlog.h>

#include "widget_plugin_parser_internal.h"

enum widget_tag {
	TAG_WIDGET_APPLICATION = 1,
	TAG_WIDGET_CLASS,
	TAG_LABEL,
	TAG_ICON,
	TAG_SUPPORT_SIZE,
};

struct tag_map {
	char *name;
	enum widget_tag tag;
};

struct tag_map map[] = {
	{ "widget-application", TAG_WIDGET_APPLICATION },
	{ "widget-class", TAG_WIDGET_CLASS },
	{ "label", TAG_LABEL },
	{ "icon", TAG_ICON },
	{ "support-size", TAG_SUPPORT_SIZE }
};
static GHashTable *tag_table;

int widget_plugin_parser_init(void)
{
	int i;

	if (tag_table)
		return 0;

	tag_table = g_hash_table_new(g_int_hash, g_str_equal);
	if (tag_table == NULL)
		return -1;

	for (i = 0; i < (sizeof(map) / sizeof(struct tag_map)); i++)
		g_hash_table_insert(tag_table, map[i].name,
				(gpointer)map[i].tag);

	return 0;
}

int widget_plugin_parser_fini(void)
{
	if (!tag_table)
		return 0;

	g_hash_table_destroy(tag_table);
	tag_table = NULL;

	return 0;
}

static char *_get_attribute(xmlNode *node, const char *name)
{
	xmlChar *val;
	char *attr = NULL;

	val = xmlGetProp(node, (const xmlChar *)name);
	if (val) {
		attr = strdup((char *)val);
		xmlFree(val);
	}

	return attr;
}

static enum widget_tag _get_tag(xmlNode *node)
{
	return (enum widget_tag)g_hash_table_lookup(tag_table, node->name);
}

static int _parse_support_size(xmlNode *node, GList **sizes)
{
	char *val;
	struct support_size *size;
	char *tok;
	char *ptr;

	if (node->children == NULL || node->children->content == NULL)
		return -1;

	size = calloc(1, sizeof(struct support_size));
	if (size == NULL)
		return -1;

	val = strdup((char *)node->children->content);
	tok = strtok_r(val, "xX", &ptr);
	if (tok == NULL) {
		free(size);
		return -1;
	}
	size->width = atoi(tok);
	tok = strtok_r(NULL, "xX", &ptr);
	if (tok == NULL) {
		free(size);
		return -1;
	}
	size->height = atoi(tok);
	free(val);

	size->preview = _get_attribute(node, "preview");
	val = _get_attribute(node, "frame");
	if (val && !strcasecmp(val, "true")) {
		size->frame = true;
		free(val);
	}

	*sizes = g_list_append(*sizes, size);

	return 0;
}

static int _parse_icon(xmlNode *node, GList **icons)
{
	struct icon *icon;

	if (node->children == NULL || node->children->content == NULL)
		return -1;

	icon = calloc(1, sizeof(struct icon));
	if (icon == NULL)
		return -1;

	icon->icon = strdup((char *)node->children->content);
	icon->lang = _get_attribute(node, "lang");

	*icons = g_list_append(*icons, icon);

	return 0;
}

static int _parse_label(xmlNode *node, GList **labels)
{
	struct label *label;

	if (node->children == NULL || node->children->content == NULL)
		return -1;

	label = calloc(1, sizeof(struct label));
	if (label == NULL)
		return -1;

	label->label = strdup((char *)node->children->content);
	label->lang = _get_attribute(node, "lang");

	*labels = g_list_append(*labels, label);

	return 0;
}

static int _parse_widget_class(xmlNode *node, const char *appid, GList **apps)
{
	char *val;
	xmlNode *tmp;
	struct widget_class *wc;
	char buf[128];

	wc = calloc(1, sizeof(struct widget_class));
	if (wc == NULL)
		return -1;
	wc->appid = strdup(appid);

	val = _get_attribute(node, "classid");
	if (val == NULL) {
		free(wc);
		return -1;
	}
	snprintf(buf, sizeof(buf), "%s@%s", val, appid);
	free(val);
	wc->classid = strdup(buf);

	val = _get_attribute(node, "update-period");
	if (val)
		wc->update_period = atoi(val);
	free(val);

	wc->setup_appid = _get_attribute(node, "setup-appid");

	for (tmp = node->children; tmp; tmp = tmp->next) {
		switch (_get_tag(tmp)) {
		case TAG_SUPPORT_SIZE:
			if (_parse_support_size(tmp, &wc->support_size)) {
				_free_widget_class((gpointer)wc);
				return -1;
			}
			break;
		case TAG_ICON:
			if (_parse_icon(tmp, &wc->icon)) {
				_free_widget_class((gpointer)wc);
				return -1;
			}
			break;
		case TAG_LABEL:
			if (_parse_label(tmp, &wc->label)) {
				_free_widget_class((gpointer)wc);
				return -1;
			}
			break;
		default:
			/* unexpected tag */
			continue;
		}
	}

	*apps = g_list_append(*apps, wc);

	return 0;
}

static int _parse_widget_application(xmlNode *node, GList **list)
{
	char *val;
	xmlNode *tmp;
	struct widget_class *wc;

	wc = calloc(1, sizeof(struct widget_class));
	if (wc == NULL)
		return -1;

	val = _get_attribute(node, "appid");
	if (val == NULL) {
		free(wc);
		return -1;
	}
	wc->appid = val;
	wc->classid = strdup(wc->appid);

	val = _get_attribute(node, "update-period");
	if (val)
		wc->update_period = atoi(val);

	val = _get_attribute(node, "nodisplay");
	if (val && strncmp(val, "true", strlen("true")) == 0)
		wc->nodisplay = 1;
	else
		wc->nodisplay = 0;

	wc->setup_appid = _get_attribute(node, "setup-appid");

	for (tmp = node->children; tmp; tmp = tmp->next) {
		switch (_get_tag(tmp)) {
		case TAG_SUPPORT_SIZE:
			if (_parse_support_size(tmp, &wc->support_size)) {
				_free_widget_class((gpointer)wc);
				return -1;
			}
			break;
		case TAG_ICON:
			if (_parse_icon(tmp, &wc->icon)) {
				_free_widget_class((gpointer)wc);
				return -1;
			}
			break;
		case TAG_LABEL:
			if (_parse_label(tmp, &wc->label)) {
				_free_widget_class((gpointer)wc);
				return -1;
			}
			break;
		case TAG_WIDGET_CLASS:
			if (_parse_widget_class(tmp, wc->appid, list)) {
				_free_widget_class((gpointer)wc);
				return -1;
			}
		default:
			continue;
		}
	}

	*list = g_list_append(*list, wc);

	return 0;
}

GList *widget_plugin_parser_parse_manifest(xmlDocPtr doc)
{
	xmlNode *root;
	xmlNode *tmp;
	GList *list = NULL;

	if (!tag_table) {
		LOGE("parser is not initialized");
		return NULL;
	}

	if (doc == NULL) {
		LOGE("invalid parameter");
		return NULL;
	}

	root = xmlDocGetRootElement(doc);
	if (root == NULL) {
		LOGE("failed to get root element");
		return NULL;
	}

	for (tmp = root->children; tmp; tmp = tmp->next) {
		if (_get_tag(tmp) != TAG_WIDGET_APPLICATION)
			continue;

		if (_parse_widget_application(tmp, &list)) {
			LOGE("parse failed");
			return NULL;
		}
	}

	return list;
}
