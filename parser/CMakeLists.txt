CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(widget-plugin-parser C)

SET(SRCS
	widget_plugin_parser.c
	widget_plugin_parser_db.c
	widget_plugin_parser_internal.c
	widget_plugin_parser_pkgmgr_interface.c
	)

pkg_check_modules(PKGS REQUIRED
	glib-2.0
	sqlite3
	libxml-2.0
	dlog
	libtzplatform-config
	)
FOREACH(FLAGS ${PKGS_CFLAGS})
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAGS}")
ENDFOREACH(FLAGS)

INCLUDE_DIRECTORIES(../include)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden -fPIC")

ADD_LIBRARY(widget-application MODULE ${SRCS})

INSTALL(TARGETS widget-application DESTINATION ${SYSCONF_INSTALL_DIR}/package-manager/parserlib/)
