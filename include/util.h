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

struct supported_size_list {
    int w;
    int h;
};

struct service_info {
    sqlite3 *handle;
    const char *dbfile;
    const char *conf_file;
    int init_count;

    const char *iso3lang;
    char country[ULOC_COUNTRY_CAPACITY];
    char *syslang;
    int country_len;

    int base_w;
    int base_h;

    int base_parse;
    widget_error_e last_status;
};

extern double util_timestamp(void);
extern const char *util_basename(const char *name);
extern unsigned long util_free_space(const char *path);
extern char *util_id_to_uri(const char *id);    /* For FILENAME id */
extern int util_update_resolution(struct service_info *info, struct supported_size_list *SIZE_LIST);
extern int util_screen_size_get(unsigned int *width, unsigned int *height);

#define DEFAULT_TIMEOUT 2.0
#define SCHEMA_FILE    "file://"
#define SCHEMA_PIXMAP    "pixmap://"
#define SCHEMA_SHM    "shm://"

#define MAX_COLUMN 80

/* End of a file */
