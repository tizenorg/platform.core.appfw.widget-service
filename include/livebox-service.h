#ifndef __LIVEBOX_SERVICE_H
#define __LIVEBOX_SERVICE_H

#ifdef __cplusplus
extern "C" {
#endif

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
 * \brief Get the path of the plug-in module
 * \param[in] appid Package name of a livebox
 * \return path String which is allocated on the heap
 */
extern char *livebox_service_libexec(const char *appid);

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
