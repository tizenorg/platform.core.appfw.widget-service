extern double util_timestamp(void);
extern const char *util_basename(const char *name);
extern unsigned long util_free_space(const char *path);
extern char *util_replace_string(const char *src, const char *pattern, const char *replace);
extern const char *util_uri_to_path(const char *uri);
extern int util_validate_livebox_package(const char *pkgname);
extern char *util_conf_get_libexec(const char *pkgname);

#define SCHEMA_FILE	"file://"
#define SCHEMA_PIXMAP	"pixmap://"
#define SCHEMA_SHM	"shm://"

/* End of a file */
