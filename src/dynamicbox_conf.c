/*
 * Copyright 2013  Samsung Electronics Co., Ltd
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
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // access
#include <sqlite3.h>

#include <dlog.h>
#include <dynamicbox_errno.h>
#include <unicode/uloc.h>

#include "dynamicbox_conf.h"
#include "util.h"
#include "debug.h"

#define BASE_SHARE_DIR "/opt/usr/share/live_magazine/"
#define DEFAULT_INPUT_NODE "/dev/input/event2"

#define CR 13
#define LF 10

#define MAX_ABI		256
#define MAX_PKGNAME	512

static const char *CONF_DEFAULT_SERVICES = "[dynamicbox],[shortcut],[notification],[badge],[utility],[file]";
static const char *CONF_DEFAULT_EMERGENCY_DISK = "source=tmpfs;type=tmpfs;option=size=6M";
static const char *CONF_DEFAULT_PATH_CONF = "/opt/usr/live/%s/etc/%s.conf";
static const char *CONF_DEFAULT_PATH_IMAGE = BASE_SHARE_DIR;
static const char *CONF_DEFAULT_PATH_LOG = BASE_SHARE_DIR"log";
static const char *CONF_DEFAULT_PATH_READER = BASE_SHARE_DIR"reader";
static const char *CONF_DEFAULT_PATH_ALWAYS = BASE_SHARE_DIR"always";
static const char *CONF_DEFAULT_PATH_SCRIPT = "/opt/usr/live/%s/res/script/%s.edj";
static const char *CONF_DEFAULT_PATH_ROOT = "/opt/usr/live/";
static const char *CONF_DEFAULT_PATH_SCRIPT_PORT = "/usr/share/data-provider-master/plugin-script/";
static const char *CONF_DEFAULT_PATH_DB = "/opt/dbspace/.dynamicbox.db";
static const char *CONF_DEFAULT_PATH_INPUT = DEFAULT_INPUT_NODE;
static const char *CONF_DEFAULT_SCRIPT_TYPE = "edje";
static const char *CONF_DEFAULT_ABI = "c";
static const char *CONF_DEFAULT_GBAR_GROUP = "disclosure";
static const char *CONF_DEFAULT_LAUNCH_BUNDLE_NAME = "name";
static const char *CONF_DEFAULT_LAUNCH_BUNDLE_SECURED = "secured";
static const char *CONF_DEFAULT_LAUNCH_BUNDLE_ABI = "abi";
static const char *CONF_DEFAULT_CONTENT = "default";
static const char *CONF_DEFAULT_TITLE = "";
static const char *CONF_DEFAULT_EMPTY_CONTENT = "";
static const char *CONF_DEFAULT_EMPTY_TITLE = "";
static const char *CONF_DEFAULT_REPLACE_TAG = "/APPID/";
static const char *CONF_DEFAULT_PROVIDER_METHOD = "pixmap";
static const int CONF_DEFAULT_WIDTH = 0;
static const int CONF_DEFAULT_HEIGHT = 0;
static const int CONF_DEFAULT_BASE_WIDTH = 720;
static const int CONF_DEFAULT_BASE_HEIGHT = 1280;
static const double CONF_DEFAULT_MINIMUM_PERIOD = 1.0f;
static const double CONF_DEFAULT_PERIOD = -1.0f;
static const double CONF_DEFAULT_PACKET_TIME = 0.0001f;
static const unsigned long CONF_DEFAULT_MINIMUM_SPACE = 5242880;
static const double CONF_DEFAULT_SLAVE_TTL = 30.0f;
static const double CONF_DEFAULT_SLAVE_ACTIVATE_TIME = 30.0f;
static const double CONF_DEFAULT_SLAVE_RELAUNCH_TIME = 3.0f;
static const int CONF_DEFAULT_SLAVE_RELAUNCH_COUNT = 3;
static const int CONF_DEFAULT_MAX_LOG_LINE = 1000;
static const int CONF_DEFAULT_MAX_LOG_FILE = 3;
static const int CONF_DEFAULT_SQLITE_FLUSH_MAX = 1048576;
static const double CONF_DEFAULT_PING_TIME = 240.0f;
static const int CONF_DEFAULT_SLAVE_MAX_LOAD = 30;
static const int CONF_DEFAULT_USE_SW_BACKEND = 0;
static const int CONF_DEFAULT_DEBUG_MODE = 0;
static const int CONF_DEFAULT_OVERWRITE_CONTENT = 0;
static const int CONF_DEFAULT_COM_CORE_THREAD = 1;
static const int CONF_DEFAULT_USE_XMONITOR = 0;
static const int CONF_DEFAULT_PREMULTIPLIED = 1;
static const double CONF_DEFAULT_SCALE_WIDTH_FACTOR = 1.0f;
static const double CONF_DEFAULT_SCALE_HEIGHT_FACTOR = 1.0f;
static const double CONF_DEFAULT_GBAR_REQUEST_TIMEOUT = 5.0f;
static const int CONF_DEFAULT_PIXELS = sizeof(int);
static const int CONF_DEFAULT_AUTO_ALIGN = 1;
static const int CONF_DEFAULT_USE_EVENT_TIME = 1;
static const int CONF_DEFAULT_CHECK_LCD = 1;

#define CONF_PATH_FORMAT "/usr/share/data-provider-master/%dx%d/conf.ini"

int errno;

struct dynamicbox_conf {
	unsigned int width;
	unsigned int height;

	unsigned int base_width;
	unsigned int base_height;
	double minimum_period;

	struct {
		char *script;
		char *abi;
		char *gbar_group;
		double period;
		int pixels;
	} default_conf;

	struct {
		char *name;
		char *secured;
		char *abi;
	} launch_key;

	double default_packet_time;

	char *empty_content;
	char *empty_title;

	char *default_content;
	char *default_title;

	unsigned long minimum_space;

	char *replace_tag;

	double slave_ttl;
	double slave_activate_time;
	double slave_relaunch_time;
	int slave_relaunch_count;

	int max_log_line;
	int max_log_file;

	unsigned long sqlite_flush_max;

	struct {
		char *conf;
		char *image;
		char *script;
		char *root;
		char *script_port;
		char *slave_log;
		char *reader;
		char *always;
		char *db;
		char *input;
	} path;

	int max_size_type;

	int slave_max_load;

	double ping_time;

	char *vconf_sys_cluster;
	int max_pended_ctx_events;

	int use_sw_backend;
	char *provider_method;
	int debug_mode;
	int overwrite_content;
	int com_core_thread;
	int use_xmonitor;
	int premultiplied;

	double scale_width_factor;
	double scale_height_factor;

	double gbar_request_timeout;

	char *emergency_disk;
	char *services;
	int auto_align;
	int use_event_time;
	int check_lcd;
};

static struct dynamicbox_conf s_conf;

static struct info {
	int conf_loaded;
} s_info = {
	.conf_loaded = 0,
};

static void use_xmonitor(char *buffer)
{
	s_conf.use_xmonitor = !strcasecmp(buffer, "true");
}

static void emergency_disk_handler(char *buffer)
{
	s_conf.emergency_disk = strdup(buffer);
	if (!s_conf.emergency_disk) {
		ErrPrint("Heap: %s\n", strerror(errno));
	}
}

static void check_lcd_handler(char *buffer)
{
	s_conf.check_lcd = !strcasecmp(buffer, "true");
}

static void use_event_time_handler(char *buffer)
{
	s_conf.use_event_time = !strcasecmp(buffer, "true");
}

static void auto_align_handler(char *buffer)
{
	s_conf.auto_align = !strcasecmp(buffer, "true");
}

static void services_handler(char *buffer)
{
	s_conf.services = strdup(buffer);
	if (!s_conf.services) {
		ErrPrint("Heap: %s\n", strerror(errno));
	}
}

static void use_sw_backend_handler(char *buffer)
{
	s_conf.use_sw_backend = !strcasecmp(buffer, "true");
}

static void provider_method_handler(char *buffer)
{
	s_conf.provider_method = strdup(buffer);
	if (!s_conf.provider_method) {
		ErrPrint("Heap: %s\n", strerror(errno));
	}
}

static void debug_mode_handler(char *buffer)
{
	s_conf.debug_mode = !strcasecmp(buffer, "true");
}

static void overwrite_content_handler(char *buffer)
{
	s_conf.overwrite_content = !strcasecmp(buffer, "true");
}

static void com_core_thread_handler(char *buffer)
{
	s_conf.com_core_thread = !strcasecmp(buffer, "true");
}

static void base_width_handler(char *buffer)
{
	if (sscanf(buffer, "%d", &s_conf.base_width) != 1) {
		if (!strncasecmp(buffer, "screen", strlen("screen"))) {
			s_conf.base_width = s_conf.width;
			DbgPrint("Base width: %u (screen)\n", s_conf.base_width);
		} else {
			ErrPrint("Failed to parse the base_width\n");
		}
	}
}

static void base_height_handler(char *buffer)
{
	if (sscanf(buffer, "%d", &s_conf.base_height) != 1) {
		if (!strncasecmp(buffer, "screen", strlen("screen"))) {
			s_conf.base_height = s_conf.height;
			DbgPrint("Base height: %u (screen)\n", s_conf.base_height);
		} else {
			ErrPrint("Failed to parse the base_height\n");
		}
	}
}

static void minimum_period_handler(char *buffer)
{
	if (sscanf(buffer, "%lf", &s_conf.minimum_period) != 1) {
		ErrPrint("Failed to parse the minimum_period\n");
	}
	DbgPrint("Minimum period: %lf\n", s_conf.minimum_period);
}

static void pixels_handler(char *buffer)
{
	if (sscanf(buffer, "%d", &s_conf.default_conf.pixels) != 1) {
		ErrPrint("Failed to parse the minimum_period\n");
	}
	DbgPrint("Default pixels: %lf\n", s_conf.default_conf.pixels);
}

static void script_handler(char *buffer)
{
	s_conf.default_conf.script = strdup(buffer);
	if (!s_conf.default_conf.script) {
		ErrPrint("Heap: %s\n", strerror(errno));
	}
}

static void default_abi_handler(char *buffer)
{
	s_conf.default_conf.abi = strdup(buffer);
	if (!s_conf.default_conf.abi) {
		ErrPrint("Heap: %s\n", strerror(errno));
	}
}

static void default_group_handler(char *buffer)
{
	s_conf.default_conf.gbar_group = strdup(buffer);
	if (!s_conf.default_conf.gbar_group) {
		ErrPrint("Heap: %s\n", strerror(errno));
	}
}

static void default_period_handler(char *buffer)
{
	if (sscanf(buffer, "%lf", &s_conf.default_conf.period) != 1) {
		ErrPrint("Failed to parse the default_period\n");
	}
	DbgPrint("Default Period: %lf\n", s_conf.default_conf.period);
}

static void default_packet_time_handler(char *buffer)
{
	if (sscanf(buffer, "%lf", &s_conf.default_packet_time) != 1) {
		ErrPrint("Failed to parse the default_packet_time\n");
	}
	DbgPrint("Default packet time: %lf\n", s_conf.default_packet_time);
}

static void default_content_handler(char *buffer)
{
	s_conf.default_content = strdup(buffer);
	if (!s_conf.default_content) {
		ErrPrint("Heap: %s\n", strerror(errno));
	}
}

static void default_title_handler(char *buffer)
{
	s_conf.default_title = strdup(buffer);
	if (!s_conf.default_title) {
		ErrPrint("Heap: %s\n", strerror(errno));
	}
}

static void minimum_space_handler(char *buffer)
{
	if (sscanf(buffer, "%lu", &s_conf.minimum_space) != 1) {
		ErrPrint("Failed to parse the minimum_space\n");
	}
}

static void replace_tag_handler(char *buffer)
{
	s_conf.replace_tag = strdup(buffer);
	if (!s_conf.replace_tag) {
		ErrPrint("Heap: %s\n", strerror(errno));
	}
}

static void slave_ttl_handler(char *buffer)
{
	if (sscanf(buffer, "%lf", &s_conf.slave_ttl) != 1) {
		ErrPrint("Failed to parse the slave_ttl\n");
	}
	DbgPrint("Slave TTL: %lf\n", s_conf.slave_ttl);
}

static void slave_activate_time_handler(char *buffer)
{
	if (sscanf(buffer, "%lf", &s_conf.slave_activate_time) != 1) {
		ErrPrint("Failed to parse the slave_activate_time\n");
	}
	DbgPrint("Slave activate time: %lf\n", s_conf.slave_activate_time);
}

static void slave_relaunch_time_handler(char *buffer)
{
	if (sscanf(buffer, "%lf", &s_conf.slave_relaunch_time) != 1) {
		ErrPrint("Failed to parse the slave_activate_time\n");
	}
	DbgPrint("Slave relaunch time: %lf\n", s_conf.slave_relaunch_time);
}

static void slave_relaunch_count_handler(char *buffer)
{
	if (sscanf(buffer, "%d", &s_conf.slave_relaunch_count) != 1) {
		ErrPrint("Failed to parse the max_log_line\n");
	}
}

static void max_log_line_handler(char *buffer)
{
	if (sscanf(buffer, "%d", &s_conf.max_log_line) != 1) {
		ErrPrint("Failed to parse the max_log_line\n");
	}
}

static void max_log_file_handler(char *buffer)
{
	if (sscanf(buffer, "%d", &s_conf.max_log_file) != 1) {
		ErrPrint("Failed to parse the max_log_file\n");
	}
}

static void sqlite_flush_max_handler(char *buffer)
{
	if (sscanf(buffer, "%lu", &s_conf.sqlite_flush_max) != 1) {
		ErrPrint("Failed to parse the sqlite_flush_max\n");
	}
}

static void db_path_handler(char *buffer)
{
	s_conf.path.db = strdup(buffer);
	if (!s_conf.path.db) {
		ErrPrint("Heap: %s\n", strerror(errno));
	}
}

static void reader_path_handler(char *buffer)
{
	s_conf.path.reader = strdup(buffer);
	if (!s_conf.path.reader) {
		ErrPrint("Heap: %s\n", strerror(errno));
	}
}

static void always_path_handler(char *buffer)
{
	s_conf.path.always = strdup(buffer);
	if (!s_conf.path.always) {
		ErrPrint("Heap: %s\n", strerror(errno));
	}
}

static void log_path_handler(char *buffer)
{
	s_conf.path.slave_log = strdup(buffer);
	if (!s_conf.path.slave_log) {
		ErrPrint("Heap: %s\n", strerror(errno));
	}
}

static void script_port_path_handler(char *buffer)
{
	s_conf.path.script_port = strdup(buffer);
	if (!s_conf.path.script_port) {
		ErrPrint("Heap: %s\n", strerror(errno));
	}
}

static void share_path_handler(char *buffer)
{
	s_conf.path.image = strdup(buffer);
	if (!s_conf.path.image) {
		ErrPrint("Heap: %s\n", strerror(errno));
	}
}

static void input_path_handler(char *buffer)
{
	s_conf.path.input = strdup(buffer);
	if (!s_conf.path.input) {
		ErrPrint("Heap: %s\n", strerror(errno));
	}
}

static void ping_time_handler(char *buffer)
{
	if (sscanf(buffer, "%lf", &s_conf.ping_time) != 1) {
		ErrPrint("Failed to parse the ping_time\n");
	}
	DbgPrint("Default ping time: %lf\n", s_conf.ping_time);
}

static void slave_max_loader(char *buffer)
{
	if (sscanf(buffer, "%d", &s_conf.slave_max_load) != 1) {
		ErrPrint("Failed to parse the slave_max_load\n");
	}
}

static void premultiplied_handler(char *buffer)
{
	if (sscanf(buffer, "%d", &s_conf.premultiplied) != 1) {
		ErrPrint("Failed to parse the premultiplied color\n");
	}

	DbgPrint("Premultiplied: %d\n", s_conf.premultiplied);
}

static void gbar_request_timeout_handler(char *buffer)
{
	if (sscanf(buffer, "%lf", &s_conf.gbar_request_timeout) != 1) {
		ErrPrint("Failed to parse the request_timeout\n");
	}
	DbgPrint("Default GBAR request timeout: %lf\n", s_conf.gbar_request_timeout);
}

EAPI void dynamicbox_conf_init(void)
{
	if (s_info.conf_loaded) {
		DbgPrint("Already initialized.\n");
		return;
	}

	s_conf.width = CONF_DEFAULT_WIDTH;
	s_conf.height = CONF_DEFAULT_HEIGHT;
	s_conf.base_width = CONF_DEFAULT_BASE_WIDTH;
	s_conf.base_height = CONF_DEFAULT_BASE_HEIGHT;
	s_conf.minimum_period = CONF_DEFAULT_MINIMUM_PERIOD;
	s_conf.default_conf.period = CONF_DEFAULT_PERIOD;
	s_conf.default_conf.pixels = CONF_DEFAULT_PIXELS;
	s_conf.minimum_space = CONF_DEFAULT_MINIMUM_SPACE;
	s_conf.default_packet_time = CONF_DEFAULT_PACKET_TIME;
	s_conf.slave_ttl = CONF_DEFAULT_SLAVE_TTL;
	s_conf.slave_activate_time = CONF_DEFAULT_SLAVE_ACTIVATE_TIME;
	s_conf.slave_relaunch_time = CONF_DEFAULT_SLAVE_RELAUNCH_TIME;
	s_conf.slave_relaunch_count = CONF_DEFAULT_SLAVE_RELAUNCH_COUNT;
	s_conf.max_log_line = CONF_DEFAULT_MAX_LOG_LINE;
	s_conf.max_log_file = CONF_DEFAULT_MAX_LOG_FILE;
	s_conf.sqlite_flush_max = CONF_DEFAULT_SQLITE_FLUSH_MAX;
	s_conf.ping_time = CONF_DEFAULT_PING_TIME;
	s_conf.slave_max_load = CONF_DEFAULT_SLAVE_MAX_LOAD;
	s_conf.use_sw_backend = CONF_DEFAULT_USE_SW_BACKEND;
	s_conf.debug_mode = CONF_DEFAULT_DEBUG_MODE;
	s_conf.overwrite_content = CONF_DEFAULT_OVERWRITE_CONTENT;
	s_conf.com_core_thread = CONF_DEFAULT_COM_CORE_THREAD;
	s_conf.use_xmonitor = CONF_DEFAULT_USE_XMONITOR;
	s_conf.scale_width_factor = CONF_DEFAULT_SCALE_WIDTH_FACTOR;
	s_conf.scale_height_factor = CONF_DEFAULT_SCALE_HEIGHT_FACTOR;
	s_conf.gbar_request_timeout = CONF_DEFAULT_GBAR_REQUEST_TIMEOUT;
	s_conf.premultiplied = CONF_DEFAULT_PREMULTIPLIED;
	s_conf.default_conf.script = (char *)CONF_DEFAULT_SCRIPT_TYPE;
	s_conf.default_conf.abi = (char *)CONF_DEFAULT_ABI;
	s_conf.default_conf.gbar_group = (char *)CONF_DEFAULT_GBAR_GROUP;
	s_conf.launch_key.name = (char *)CONF_DEFAULT_LAUNCH_BUNDLE_NAME;
	s_conf.launch_key.secured = (char *)CONF_DEFAULT_LAUNCH_BUNDLE_SECURED;
	s_conf.launch_key.abi = (char *)CONF_DEFAULT_LAUNCH_BUNDLE_ABI;
	s_conf.empty_content = (char *)CONF_DEFAULT_EMPTY_CONTENT;
	s_conf.empty_title = (char *)CONF_DEFAULT_EMPTY_TITLE;
	s_conf.default_content = (char *)CONF_DEFAULT_CONTENT;
	s_conf.default_title = (char *)CONF_DEFAULT_TITLE;
	s_conf.replace_tag = (char *)CONF_DEFAULT_REPLACE_TAG;
	s_conf.path.conf = (char *)CONF_DEFAULT_PATH_CONF;
	s_conf.path.image = (char *)CONF_DEFAULT_PATH_IMAGE;
	s_conf.path.slave_log = (char *)CONF_DEFAULT_PATH_LOG;
	s_conf.path.reader = (char *)CONF_DEFAULT_PATH_READER;
	s_conf.path.always = (char *)CONF_DEFAULT_PATH_ALWAYS;
	s_conf.path.script = (char *)CONF_DEFAULT_PATH_SCRIPT;
	s_conf.path.root = (char *)CONF_DEFAULT_PATH_ROOT;
	s_conf.path.script_port = (char *)CONF_DEFAULT_PATH_SCRIPT_PORT;
	s_conf.path.db = (char *)CONF_DEFAULT_PATH_DB;
	s_conf.path.input = (char *)CONF_DEFAULT_PATH_INPUT;
	s_conf.provider_method = (char *)CONF_DEFAULT_PROVIDER_METHOD;
	s_conf.emergency_disk = (char *)CONF_DEFAULT_EMERGENCY_DISK;
	s_conf.services = (char *)CONF_DEFAULT_SERVICES;
	s_conf.auto_align = CONF_DEFAULT_AUTO_ALIGN;
	s_conf.use_event_time = CONF_DEFAULT_USE_EVENT_TIME;
	s_conf.check_lcd = CONF_DEFAULT_CHECK_LCD;
}

/*
 * Find proper configuration and install(link) it to conf path.
 */
static char *conf_path(void)
{
	char *path;
	int length;

	length = strlen(CONF_PATH_FORMAT) + 12;	// 12 == RESERVED SPACE
	path = calloc(1, length);
	if (!path) {
		ErrPrint("calloc: %s\n", strerror(errno));
		return NULL;
	}

	snprintf(path, length, CONF_PATH_FORMAT, s_conf.width, s_conf.height);
	DbgPrint("Selected conf file: %s\n", path);
	if (access(path, F_OK) != 0) {
		ErrPrint("Fallback to default, access: %s\n", strerror(errno));
		strncpy(path, DEFAULT_MASTER_CONF, length);
		if (access(path, F_OK) != 0) {
			ErrPrint("Serious error - there is no conf file, use default setting: %s\n", strerror(errno));
			free(path);
			path = NULL;
		}
	}

	return path;
}

EAPI int dynamicbox_conf_load(void)
{
	char *conf_file;
	FILE *fp;
	int c;
	enum state {
		START,
		SPACE,
		TOKEN,
		VALUE,
		ERROR,
		COMMENT,
		END
	} state;
	int ch_idx;
	int token_idx;
	int buffer_idx;
	int quote;
	int linelen;
	char buffer[256];
	static const struct token_parser {
		const char *name;
		void (*handler)(char *buffer);
	} token_handler[] = {
		{
			.name = "base_width",
			.handler = base_width_handler,
		},
		{
			.name = "base_height",
			.handler = base_height_handler,
		},
		{
			.name = "minimum_period",
			.handler = minimum_period_handler,
		},
		{
			.name = "script",
			.handler = script_handler,
		},
		{
			.name = "pixels",
			.handler = pixels_handler,
		},
		{
			.name = "default_abi",
			.handler = default_abi_handler,
		},
		{
			.name = "default_group",
			.handler = default_group_handler,
		},
		{
			.name = "default_period",
			.handler = default_period_handler,
		},
		{
			.name = "default_packet_time",
			.handler = default_packet_time_handler,
		},
		{
			.name = "default_content",
			.handler = default_content_handler,
		},
		{
			.name = "default_title",
			.handler = default_title_handler,
		},
		{
			.name = "minimum_space",
			.handler = minimum_space_handler,
		},
		{
			.name = "replace_tag",
			.handler = replace_tag_handler,
		},
		{
			.name = "slave_ttl",
			.handler = slave_ttl_handler,
		},
		{
			.name = "slave_activate_time",
			.handler = slave_activate_time_handler,
		},
		{
			.name = "slave_relaunch_time",
			.handler = slave_relaunch_time_handler,
		},
		{
			.name = "slave_relaunch_count",
			.handler = slave_relaunch_count_handler,
		},
		{
			.name = "max_log_line",
			.handler = max_log_line_handler,
		},
		{
			.name = "max_log_file",
			.handler = max_log_file_handler,
		},
		{
			.name = "sqilte_flush_max",
			.handler = sqlite_flush_max_handler,
		},
		{
			.name = "db_path",
			.handler = db_path_handler,
		},
		{
			.name = "log_path",
			.handler = log_path_handler,
		},
		{
			.name = "reader_path",
			.handler = reader_path_handler,
		},
		{
			.name = "always_path",
			.handler = always_path_handler,
		},
		{
			.name = "share_path",
			.handler = share_path_handler,
		},
		{
			.name = "script_port_path",
			.handler = script_port_path_handler,
		},
		{
			.name = "ping_interval",
			.handler = ping_time_handler,
		},
		{
			.name = "slave_max_load",
			.handler = slave_max_loader,
		},
		{
			.name = "use_sw_backend",
			.handler = use_sw_backend_handler,
		},
		{
			.name = "emergency_disk",
			.handler = emergency_disk_handler,
		},
		{
			.name = "services",
			.handler = services_handler,
		},
		{
			.name = "auto_align",
			.handler = auto_align_handler,
		},
		{
			.name = "use_event_time",
			.handler = use_event_time_handler,
		},
		{
			.name = "check_lcd",
			.handler = check_lcd_handler,
		},
		{
			.name = "use_xmonitor",
			.handler = use_xmonitor,
		},
		{
			.name = "provider_method",
			.handler = provider_method_handler,
		},
		{
			.name = "debug_mode",
			.handler = debug_mode_handler,
		},
		{
			.name = "overwrite_content",
			.handler = overwrite_content_handler,
		},
		{
			.name = "com_core_thread",
			.handler = com_core_thread_handler,
		},
		{
			.name = "input",
			.handler = input_path_handler,
		},
		{
			.name = "gbar_request_timeout",
			.handler = gbar_request_timeout_handler,
		},
		{
			.name = "premultiplied",
			.handler = premultiplied_handler,
		},
		{
			.name = NULL,
			.handler = NULL,
		},
	};

	if (s_info.conf_loaded) {
		ErrPrint("Already loaded\n");
		return DBOX_STATUS_ERROR_ALREADY;
	}

	util_screen_size_get(&s_conf.width, &s_conf.height);

	conf_file = conf_path();
	if (!conf_file) {
		return DBOX_STATUS_ERROR_IO_ERROR;
	}

	fp = fopen(conf_file, "rt");
	free(conf_file);
	if (!fp) {
		ErrPrint("Error: %s\n", strerror(errno));
		return DBOX_STATUS_ERROR_IO_ERROR;
	}

	state = START;
	ch_idx = 0;
	token_idx = -1;
	buffer_idx = 0;
	quote = 0;
	linelen = 0;
	do {
		c = getc(fp);
		if ((c == EOF) && (state == VALUE)) {
			DbgPrint("[%s:%d] VALUE state EOF\n", __func__, __LINE__);
			state = END;
		}

		switch (state) {
		case COMMENT:
			if (c == CR || c == LF || c == EOF) {
				buffer[buffer_idx] = '\0';

				state = START;
				token_idx = -1;
				ch_idx = 0;
				buffer_idx = 0;
				linelen = -1; /* Will be ZERO by follwing increment code */
				quote = 0;
			} else {
				buffer[buffer_idx++] = c;
				if (buffer_idx == (sizeof(buffer) - 1)) {
					buffer[buffer_idx] = '\0';
					buffer_idx = 0;
				}
			}
			break;
		case START:
			if (linelen == 0 && c == '#') {
				state = COMMENT;
			} else if (isspace(c)) {
				/* Ignore empty space */
			} else {
				state = TOKEN;
				ungetc(c, fp);
			}
			break;
		case SPACE:
			if (c == '=') {
				state = VALUE;
			} else if (!isspace(c)) {
				state = ERROR;
			}
			break;
		case VALUE:
			if (c == '"') {
				if (quote == 1) {
					buffer[buffer_idx] = '\0';
					state = END;
				} else if (buffer_idx != 0) {
					buffer[buffer_idx++] = c;
					if (buffer_idx >= sizeof(buffer)) {
						state = ERROR;
					}
				} else {
					quote = 1;
				}
			} else if (isspace(c)) {
				if (buffer_idx == 0) {
					/* Ignore */
				} else if (quote == 1) {
					buffer[buffer_idx++] = c;
					if (buffer_idx >= sizeof(buffer)) {
						state = ERROR;
					}
				} else {
					buffer[buffer_idx] = '\0';
					ungetc(c, fp);
					state = END;
				}
			} else {
				buffer[buffer_idx++] = c;
				if (buffer_idx >= sizeof(buffer)) {
					state = ERROR;
				}
			}
			break;
		case TOKEN:
			if (c == '=') {
				if (token_idx < 0) {
					state = ERROR;
				} else {
					state = VALUE;
				}
			} else if (isspace(c)) {
				if (token_idx < 0) {
					break;
				}

				if (token_handler[token_idx].name[ch_idx] != '\0') {
					state = ERROR;
				} else {
					state = SPACE;
				}
			} else  {
				if (token_idx < 0) {
					/* Now start to find a token! */
					token_idx = 0;
				}

				if (token_handler[token_idx].name[ch_idx] == c) {
					ch_idx++;
				} else {
					ungetc(c, fp);
					while (ch_idx-- > 0)
						ungetc(token_handler[token_idx].name[ch_idx], fp);

					token_idx++;

					if (token_handler[token_idx].name == NULL) {
						state = ERROR;
					} else {
						ch_idx = 0;
					}
				}
			}
			break;
		case ERROR:
			if (c == CR || c == LF || c == EOF) {
				state = START;
				token_idx = -1;
				buffer_idx = 0;
				ch_idx = 0;
				linelen = -1;
				quote = 0;
			}
			break;
		case END:
			if (c == LF || c == CR || c == EOF) {
				state = START;

				if (token_idx >= 0 && token_handler[token_idx].handler) {
					buffer[buffer_idx] = '\0';
					token_handler[token_idx].handler(buffer);
				}

				token_idx = -1;
				ch_idx = 0;
				buffer_idx = 0;
				linelen = -1;
				quote = 0;
				/* Finish */
			} else if (isspace(c)) {
				/* ignore */
			} else {
				state = ERROR;
			}
			break;
		default:
			/* ?? */
			break;
		}

		linelen++;
	 } while (c != EOF);

	if (fclose(fp) != 0) {
		ErrPrint("fclose: %s\n", strerror(errno));
	}

	s_conf.scale_width_factor = (double)s_conf.width / (double)DYNAMICBOX_CONF_BASE_W;
	s_conf.scale_height_factor = (double)s_conf.height / (double)DYNAMICBOX_CONF_BASE_H;
	s_info.conf_loaded = 1;

	return DBOX_STATUS_ERROR_NONE;
}

EAPI void dynamicbox_conf_reset(void)
{
	s_conf.width = CONF_DEFAULT_WIDTH;
	s_conf.height = CONF_DEFAULT_HEIGHT;
	s_conf.base_width = CONF_DEFAULT_BASE_WIDTH;
	s_conf.base_height = CONF_DEFAULT_BASE_HEIGHT;
	s_conf.minimum_period = CONF_DEFAULT_MINIMUM_PERIOD;
	s_conf.default_conf.period = CONF_DEFAULT_PERIOD;
	s_conf.minimum_space = CONF_DEFAULT_MINIMUM_SPACE;
	s_conf.default_packet_time = CONF_DEFAULT_PACKET_TIME;
	s_conf.slave_ttl = CONF_DEFAULT_SLAVE_TTL;
	s_conf.slave_activate_time = CONF_DEFAULT_SLAVE_ACTIVATE_TIME;
	s_conf.slave_relaunch_time = CONF_DEFAULT_SLAVE_RELAUNCH_TIME;
	s_conf.slave_relaunch_count = CONF_DEFAULT_SLAVE_RELAUNCH_COUNT;
	s_conf.max_log_line = CONF_DEFAULT_MAX_LOG_LINE;
	s_conf.max_log_file = CONF_DEFAULT_MAX_LOG_FILE;
	s_conf.sqlite_flush_max = CONF_DEFAULT_SQLITE_FLUSH_MAX;
	s_conf.ping_time = CONF_DEFAULT_PING_TIME;
	s_conf.slave_max_load = CONF_DEFAULT_SLAVE_MAX_LOAD;
	s_conf.use_sw_backend = CONF_DEFAULT_USE_SW_BACKEND;
	s_conf.debug_mode = CONF_DEFAULT_DEBUG_MODE;
	s_conf.overwrite_content = CONF_DEFAULT_OVERWRITE_CONTENT;
	s_conf.com_core_thread = CONF_DEFAULT_COM_CORE_THREAD;
	s_conf.use_xmonitor = CONF_DEFAULT_USE_XMONITOR;
	s_conf.scale_width_factor = CONF_DEFAULT_SCALE_WIDTH_FACTOR;
	s_conf.scale_height_factor = CONF_DEFAULT_SCALE_HEIGHT_FACTOR;
	s_conf.gbar_request_timeout = CONF_DEFAULT_GBAR_REQUEST_TIMEOUT;
	s_conf.premultiplied = CONF_DEFAULT_PREMULTIPLIED;
	s_conf.default_conf.pixels = CONF_DEFAULT_PIXELS;
	s_conf.auto_align = CONF_DEFAULT_AUTO_ALIGN;
	s_conf.use_event_time = CONF_DEFAULT_USE_EVENT_TIME;
	s_conf.check_lcd = CONF_DEFAULT_CHECK_LCD;

	if (s_conf.default_conf.script != CONF_DEFAULT_SCRIPT_TYPE) {
		free(s_conf.default_conf.script);
		s_conf.default_conf.script = (char *)CONF_DEFAULT_SCRIPT_TYPE;
	}

	if (s_conf.default_conf.abi != CONF_DEFAULT_ABI) {
		free(s_conf.default_conf.abi);
		s_conf.default_conf.abi = (char *)CONF_DEFAULT_ABI;
	}

	if (s_conf.default_conf.gbar_group != CONF_DEFAULT_GBAR_GROUP) {
		free(s_conf.default_conf.gbar_group);
		s_conf.default_conf.gbar_group = (char *)CONF_DEFAULT_GBAR_GROUP;
	}

	if (s_conf.launch_key.name != CONF_DEFAULT_LAUNCH_BUNDLE_NAME) {
		free(s_conf.launch_key.name);
		s_conf.launch_key.name = (char *)CONF_DEFAULT_LAUNCH_BUNDLE_NAME;
	}

	if (s_conf.launch_key.secured != CONF_DEFAULT_LAUNCH_BUNDLE_SECURED) {
		free(s_conf.launch_key.secured);
		s_conf.launch_key.secured = (char *)CONF_DEFAULT_LAUNCH_BUNDLE_SECURED;
	}

	if (s_conf.launch_key.abi != CONF_DEFAULT_LAUNCH_BUNDLE_ABI) {
		free(s_conf.launch_key.abi);
		s_conf.launch_key.abi = (char *)CONF_DEFAULT_LAUNCH_BUNDLE_ABI;
	}

	if (s_conf.empty_content != CONF_DEFAULT_EMPTY_CONTENT) {
		free(s_conf.empty_content);
		s_conf.empty_content = (char *)CONF_DEFAULT_EMPTY_CONTENT;
	}

	if (s_conf.empty_title != CONF_DEFAULT_EMPTY_TITLE) {
		free(s_conf.empty_title);
		s_conf.empty_title = (char *)CONF_DEFAULT_EMPTY_TITLE;
	}

	if (s_conf.default_content != CONF_DEFAULT_CONTENT) {
		free(s_conf.default_content);
		s_conf.default_content = (char *)CONF_DEFAULT_CONTENT;
	}

	if (s_conf.default_title != CONF_DEFAULT_TITLE) {
		free(s_conf.default_title);
		s_conf.default_title = (char *)CONF_DEFAULT_TITLE;
	}

	if (s_conf.replace_tag != CONF_DEFAULT_REPLACE_TAG) {
		free(s_conf.replace_tag);
		s_conf.replace_tag = (char *)CONF_DEFAULT_REPLACE_TAG;
	}

	if (s_conf.path.conf != CONF_DEFAULT_PATH_CONF) {
		free(s_conf.path.conf);
		s_conf.path.conf = (char *)CONF_DEFAULT_PATH_CONF;
	}

	if (s_conf.path.image != CONF_DEFAULT_PATH_IMAGE) {
		free(s_conf.path.image);
		s_conf.path.image = (char *)CONF_DEFAULT_PATH_IMAGE;
	}

	if (s_conf.path.slave_log != CONF_DEFAULT_PATH_LOG) {
		free(s_conf.path.slave_log);
		s_conf.path.slave_log = (char *)CONF_DEFAULT_PATH_LOG;
	}

	if (s_conf.path.reader != CONF_DEFAULT_PATH_READER) {
		free(s_conf.path.reader);
		s_conf.path.reader = (char *)CONF_DEFAULT_PATH_READER;
	}

	if (s_conf.path.always != CONF_DEFAULT_PATH_ALWAYS) {
		free(s_conf.path.always);
		s_conf.path.always = (char *)CONF_DEFAULT_PATH_ALWAYS;
	}

	if (s_conf.path.script != CONF_DEFAULT_PATH_SCRIPT) {
		free(s_conf.path.script);
		s_conf.path.script = (char *)CONF_DEFAULT_PATH_SCRIPT;
	}

	if (s_conf.path.root != CONF_DEFAULT_PATH_ROOT) {
		free(s_conf.path.root);
		s_conf.path.root = (char *)CONF_DEFAULT_PATH_ROOT;
	}

	if (s_conf.path.script_port != CONF_DEFAULT_PATH_SCRIPT_PORT) {
		free(s_conf.path.script_port);
		s_conf.path.script_port = (char *)CONF_DEFAULT_PATH_SCRIPT_PORT;
	}

	if (s_conf.path.db != CONF_DEFAULT_PATH_DB) {
		free(s_conf.path.db);
		s_conf.path.db = (char *)CONF_DEFAULT_PATH_DB;
	}

	if (s_conf.path.input != CONF_DEFAULT_PATH_INPUT) {
		free(s_conf.path.input);
		s_conf.path.input = (char *)CONF_DEFAULT_PATH_INPUT;
	}

	if (s_conf.provider_method != CONF_DEFAULT_PROVIDER_METHOD) {
		free(s_conf.provider_method);
		s_conf.provider_method = (char *)CONF_DEFAULT_PROVIDER_METHOD;
	}

	if (s_conf.emergency_disk != CONF_DEFAULT_EMERGENCY_DISK) {
		free(s_conf.emergency_disk);
		s_conf.emergency_disk = (char *)CONF_DEFAULT_EMERGENCY_DISK;
	}

	if (s_conf.services != CONF_DEFAULT_SERVICES) {
		free(s_conf.services);
		s_conf.services = (char *)CONF_DEFAULT_SERVICES;
	}

	s_info.conf_loaded = 0;
}

EAPI const int const dynamicbox_conf_use_xmonitor(void)
{
	return s_conf.use_xmonitor;
}

EAPI const char * const dynamicbox_conf_emergency_disk(void)
{
	return s_conf.emergency_disk;
}

EAPI const int const dynamicbox_conf_check_lcd(void)
{
	return s_conf.check_lcd;
}

EAPI const int const dynamicbox_conf_use_event_time(void)
{
	return s_conf.use_event_time;
}

EAPI const int const dynamicbox_conf_auto_align(void)
{
	return s_conf.auto_align;
}

EAPI const char * const dynamicbox_conf_services(void)
{
	return s_conf.services;
}

EAPI const int const dynamicbox_conf_use_sw_backend(void)
{
	return s_conf.use_sw_backend;
}

EAPI const char * const dynamicbox_conf_provider_method(void)
{
	return s_conf.provider_method;
}

EAPI const int const dynamicbox_conf_debug_mode(void)
{
	return s_conf.debug_mode;
}

EAPI const int const dynamicbox_conf_overwrite_content(void)
{
	return s_conf.overwrite_content;
}

EAPI const int const dynamicbox_conf_com_core_thread(void)
{
	return s_conf.com_core_thread;
}

EAPI const unsigned int const dynamicbox_conf_base_width(void)
{
	return s_conf.base_width;
}

EAPI const unsigned int const dynamicbox_conf_base_height(void)
{
	return s_conf.base_height;
}

EAPI const double const dynamicbox_conf_minimum_period(void)
{
	return s_conf.minimum_period;
}

EAPI const int const dynamicbox_conf_default_pixels(void)
{
	return s_conf.default_conf.pixels;
}

EAPI const char * const dynamicbox_conf_default_script(void)
{
	return s_conf.default_conf.script;
}

EAPI const char * const dynamicbox_conf_default_abi(void)
{
	return s_conf.default_conf.abi;
}

EAPI const char * const dynamicbox_conf_default_gbar_group(void)
{
	return s_conf.default_conf.gbar_group;
}

EAPI const double const dynamicbox_conf_default_period(void)
{
	return s_conf.default_conf.period;
}

EAPI const double const dynamicbox_conf_default_packet_time(void)
{
	return s_conf.default_packet_time;
}

EAPI const char * const dynamicbox_conf_default_content(void)
{
	return s_conf.default_content;
}

EAPI const char * const dynamicbox_conf_default_title(void)
{
	return s_conf.default_title;
}

EAPI const unsigned long const dynamicbox_conf_minimum_space(void)
{
	return s_conf.minimum_space;
}

EAPI const char * const dynamicbox_conf_replace_tag(void)
{
	return s_conf.replace_tag;
}

EAPI const double const dynamicbox_conf_slave_ttl(void)
{
	return s_conf.slave_ttl;
}

EAPI const double const dynamicbox_conf_slave_activate_time(void)
{
	return s_conf.slave_activate_time;
}

EAPI const double const dynamicbox_conf_slave_relaunch_time(void)
{
	return s_conf.slave_relaunch_time;
}

EAPI const int const dynamicbox_conf_slave_relaunch_count(void)
{
	return s_conf.slave_relaunch_count;
}

EAPI const int const dynamicbox_conf_max_log_line(void)
{
	return s_conf.max_log_line;
}

EAPI const int const dynamicbox_conf_max_log_file(void)
{
	return s_conf.max_log_file;
}

EAPI const unsigned long const dynamicbox_conf_sqlite_flush_max(void)
{
	return s_conf.sqlite_flush_max;
}

EAPI const char * const dynamicbox_conf_db_path(void)
{
	return s_conf.path.db;
}

EAPI const char * const dynamicbox_conf_reader_path(void)
{
	return s_conf.path.reader;
}

EAPI const char * const dynamicbox_conf_always_path(void)
{
	return s_conf.path.always;
}

EAPI const char * const dynamicbox_conf_log_path(void)
{
	return s_conf.path.slave_log;
}

EAPI const char * const dynamicbox_conf_script_port(void)
{
	return s_conf.path.script_port;
}

EAPI const char * const dynamicbox_conf_script_path(void)
{
	return s_conf.path.script;
}

EAPI const char * const dynamicbox_conf_share_path(void)
{
	return s_conf.path.image;
}

EAPI const char * const dynamicbox_conf_input_path(void)
{
	return s_conf.path.input;
}

EAPI const double const dynamicbox_conf_ping_time(void)
{
	return s_conf.ping_time;
}

EAPI const int const dynamicbox_conf_slave_max_load(void)
{
	return s_conf.slave_max_load;
}

EAPI const int const dynamicbox_conf_premultiplied_alpha(void)
{
	return s_conf.premultiplied;
}

EAPI const double const dynamicbox_conf_gbar_request_timeout(void)
{
	return s_conf.gbar_request_timeout;
}

EAPI const double const dynamicbox_conf_scale_width_factor(void)
{
	return s_conf.scale_width_factor;
}

EAPI const double const dynamicbox_conf_scale_height_factor(void)
{
	return s_conf.scale_height_factor;
}

EAPI const char * const dynamicbox_conf_launch_key_name(void)
{
	return s_conf.launch_key.name;
}

EAPI const char * const dynamicbox_conf_launch_key_secured(void)
{
	return s_conf.launch_key.secured;
}

EAPI const char * const dynamicbox_conf_launch_key_abi(void)
{
	return s_conf.launch_key.abi;
}

EAPI const char * const dynamicbox_conf_empty_content(void)
{
	return s_conf.empty_content;
}

EAPI const char * const dynamicbox_conf_empty_title(void)
{
	return s_conf.empty_title;
}

EAPI const char * const dynamicbox_conf_path(void)
{
	return s_conf.path.conf;
}

EAPI const char * const dynamicbox_conf_root_path(void)
{
	return s_conf.path.root;
}

EAPI const int const dynamicbox_conf_is_loaded(void)
{
	return s_info.conf_loaded;
}

/* End of a file */

