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

#ifdef __cplusplus
}
#endif

#endif
/* End of a file */
