#include "msg_item.h"

#pragma once

/* state of ipc_strm instance */
typedef enum ipc_state {
	INITIALIZED,
	READY,
	CONNECTED,
	DISCONNECTED,
	ERROR,
} ipc_state_t;

/* generic callback for  ipc messages*/
typedef void (*msg_clbck_func_ptr)(Msg_item &item);
