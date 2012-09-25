enum livebox_service_slave_state {
	/*!
	 * Launch the slave but not yet receives "hello" packet
	 */
	SLAVE_REQUEST_TO_LAUNCH,

	/*!
	 * \note
	 * Terminate the slave but not yet receives dead signal
	 */
	SLAVE_REQUEST_TO_TERMINATE,

	/*!
	 * \note
	 * No slave process exists, just slave object created
	 */
	SLAVE_TERMINATED,

	/*!
	 * \note
	 * State change request is sent,
	 */
	SLAVE_REQUEST_TO_PAUSE,
	SLAVE_REQUEST_TO_RESUME,

	/*!
	 * \note
	 * SLAVE_ACTIVATED = { SLAVE_PAUSED, SLAVE_RESUMED }
	 */
	SLAVE_PAUSED,
	SLAVE_RESUMED,

	SLAVE_ERROR = 0xFF, /* Explicitly define the size of this enum type */
};

struct livebox_service_slave_info;

extern int livebox_service_get_slave_list(int (*cb)(struct livebox_service_slave_info *info, void *data), void *data);

/* End of a file */
