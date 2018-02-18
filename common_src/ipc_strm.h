#include <string.h>
#include "common_defs.h"

#pragma once

class Ipc_strm {
  private:
	std::string name;
	ipc_state_t state;
  public:
	virtual void Connect() = 0;
	virtual void Send(unsigned int size, void *buff) = 0;
	virtual void Receive (unsigned int size, void *buff) = 0;
	virtual void Disconect() = 0;
	virtual ipc_state_t Get_state() = 0;
	virtual ~Ipc_strm() {};
};
