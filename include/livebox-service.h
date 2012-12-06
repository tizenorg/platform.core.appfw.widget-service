#ifndef __LIVEBOX_SERVICE_H
#define __LIVEBOX_SERVICE_H

#ifdef __cplusplus
extern "C" {
#endif

#define NR_OF_SIZE_LIST 7

enum livebox_size_type {
	LB_SIZE_TYPE_1x1 = 0x01,
	LB_SIZE_TYPE_2x1 = 0x02,
	LB_SIZE_TYPE_2x2 = 0x04,
	LB_SIZE_TYPE_4x1 = 0x08,
	LB_SIZE_TYPE_4x2 = 0x10,
	LB_SIZE_TYPE_4x3 = 0x20,
	LB_SIZE_TYPE_4x4 = 0x40,
	LB_SIZE_TYPE_UNKNOWN = 0xFF,
};

/*!
 * \param[in] type
 * \param[out] width
 * \param[out] height
 */
extern int livebox_service_get_size(int type, int *width, int *height);

/*!
 * \param[in] width
 * \param[in] height
 * \return Type of a livebox
 */
extern int livebox_service_size_type(int width, int height);

/*!
 */
extern int livebox_service_trigger_update(const char *pkgname, const char *cluster, const char *category);

/*!
 * \brief Synchronous package list getter
 *        callback (appid, lbid, is_prime)
 *        appid == Application Package ID (pkgname)
 *        lbid = Livebox Package ID
 *        is_prime = 1 if the livebox is default one for associated application package
 * \param[in] cb Callback function
 * \return Count of livebox packages or errno (-EIO, -EINVAL)
 */
extern int livebox_service_get_pkglist(int (*cb)(const char *appid, const char *lbid, int is_prime, void *data), void *data);

/*!
 * \brief Get the name(appid or pkgname) of a primary livebox.
 * \param[in] appid
 * \return pkgname String which is allocated on the heap
 */
extern char *livebox_service_pkgname(const char *appid);

/*!
 *\brief Get the application pkgname(appid) of given livebox package name
 * \param[in] pkgname
 * \return appid String which is allocated on the heap
 */
extern char *livebox_service_appid(const char *pkgname);

/*!
 * \brief Internationalized name of given livebox package.
 *        USER must has to do "free" after using the returned string.
 * \param[in] pkgid App ID of a livebox. (It must has to be a livebox package ID. not the UI-APP and the PACKAGE.
 * \param[in] lang locale(en-us, ko-kr, ...), if it is NULL, function will use the system locale automatically
 * \return name if it fails to get name, returns NULL, or allocated heap address.
 */
extern char *livebox_service_i18n_name(const char *pkgid, const char *lang);

/*!
 * \brief
 * \param[in] pkgid livebox ID. NOT the UI-APP ID and PACKAGE ID
 * \param[in] size_type
 * \return path preview image path
 */
extern char *livebox_service_preview(const char *pkgid, int size_type);

/*!
 * \brief Get the default content string of the given livebox
 * \param[in] pkgid Livebox ID. Not the UI-APP ID and PACKAGE ID
 * \return content content string
 */
extern char *livebox_service_content(const char *pkgid);

/*!
 * \brief Internationalized icon path of given livebox package.
 *        USER must has to do "free" after using the returned string.
 * \param[in] pkgid App ID of a livebox. (It must has to be a livebox package ID. not the UI-APP and the PACKAGE.
 * \param[in] lang locale(en-us, ko-kr, ...), if it is NULL, function will use the system locale automatically
 * \return name if it fails to get path of an icon, returns NULL, or allocated heap address.
 */
extern char *livebox_service_i18n_icon(const char *pkgid, const char *lang);

/*!
 * \brief Get the path of the plug-in module
 * \param[in] appid Package name of a livebox
 * \return path String which is allocated on the heap
 */
extern char *livebox_service_libexec(const char *appid);

/*!
 * \brief Get the "nodisplay" value
 * \param[in] pkgid Livebox package id.
 * \return true/false
 */
extern int livebox_service_nodisplay(const char *pkgid);

/*!
 * \brief
 * \param[in]
 * \return
 */
extern char *livebox_service_lb_script_path(const char *pkgid);

/*!
 * \brief
 * \param[in]
 * \return
 */
extern char *livebox_service_lb_script_group(const char *pkgid);

/*!
 * \brief
 * \param[in]
 * \return
 */
extern char *livebox_service_pd_script_path(const char *pkgid);

/*!
 * \brief
 * \param[in]
 * \return
 */
extern char *livebox_service_pd_script_group(const char *pkgid);

/*!
 * \brief
 * \return
 */
extern int livebox_service_get_supported_sizes(const char *pkgid, int *cnt, int *w, int *h);

/*!
 * \brief
 * \return
 */
extern int livebox_service_get_supported_size_types(const char *pkgid, int *cnt, int *types);

/*!
 */
extern int livebox_service_enumerate_category_list(const char *cluster, int (*cb)(const char *cluster, const char *category, void *data), void *data);

/*!
 */
extern int livebox_service_enumerate_cluster_list(int (*cb)(const char *cluster, void *data), void *data);
/*!
 * \brief
 * \return
 */
extern int livebox_service_init(void);

/*!
 * \brief
 * \return
 */
extern int livebox_service_fini(void);

#ifdef __cplusplus
}
#endif

#endif
/* End of a file */
