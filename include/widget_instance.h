#ifndef __WIDGET_INSTANCE_H__
#define __WIDGET_INSTANCE_H__

#include <bundle.h>
#include <aul.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WIDGET_K_CLASS		AUL_K_WIDGET_CLASS_ID
#define WIDGET_K_INSTANCE	AUL_K_WIDGET_INSTANCE_ID
#define WIDGET_K_STATUS		AUL_K_WIDGET_INSTANCE_STATUS
#define WIDGET_K_EXTRA		AUL_K_WIDGET_INTERNAL_STATUS

#define WIDGET_K_CALLER		"__WIDGET_CALLER_PID__"
#define WIDGET_K_OPERATION	"__WIDGET_OP__"
#define WIDGET_K_WIDTH		"__WIDGET_WIDTH__"
#define WIDGET_K_HEIGHT		"__WIDGET_HEIGHT__"
#define WIDGET_K_REASON		"__WIDGET_REASON__"

typedef enum widget_instance_event {
	WIDGET_INSTANCE_EVENT_CREATE = 0,
	WIDGET_INSTANCE_EVENT_DESTROY = 1,
	WIDGET_INSTANCE_EVENT_TERMINATE = 2,
	WIDGET_INSTANCE_EVENT_PAUSE = 3,
	WIDGET_INSTANCE_EVENT_RESUME = 4,
	WIDGET_INSTANCE_EVENT_UPDATE = 5,
	WIDGET_INSTANCE_EVENT_PERIOD_CHANGED = 6,
	WIDGET_INSTANCE_EVENT_SIZE_CHANGED = 7,
	WIDGET_INSTANCE_EVENT_MAX,
} widget_instance_event_e;

typedef struct _widget_instance* widget_instance_h;
typedef int (*widget_instance_foreach_cb)(widget_instance_h instance, void *data);
extern int widget_instance_foreach(const char *widget_id, widget_instance_foreach_cb cb, void *data);

extern int widget_instance_get_id(widget_instance_h instance, char **id);
extern int widget_instance_get_content(widget_instance_h instance, bundle **content);
extern int widget_instance_get_width(widget_instance_h instance, int *w);
extern int widget_instance_get_height(widget_instance_h instance, int *h);
extern int widget_instance_get_period(widget_instance_h instance, double *period);

extern int widget_instance_create(const char *widget_id, char **instance_id);
extern int widget_instance_launch(const char *widget_id, const char *instance_id, bundle *content_info, int w, int h);
extern int widget_instance_terminate(const char *widget_id, const char *instance_id);
extern int widget_instance_destroy(const char *widget_id, const char *instance_id);
extern int widget_instance_init(const char *viewer_id);
extern int widget_instance_fini();

#ifdef __cplusplus
}
#endif

#endif

