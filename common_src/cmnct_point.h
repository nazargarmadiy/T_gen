#include <map>
#include "common_defs.h"
#include "ipc_strm.h"

#pragma once

class Cmnct_point {
  private:
    std::string                                name;
    Ipc_strm                                  *int_strm;
    Ipc_strm                                  *ext_strm;
    std::map<std::string, msg_clbck_func_ptr>  cbck_map;
    ipc_state_t                                state;
  public:
    Cmnct_point();
    Cmnct_point(std::string                               &nm,
				Ipc_strm                                  *int_s,
				Ipc_strm                                  *ext_s,
				std::map<std::string, msg_clbck_func_ptr> &cbcks);
    Cmnct_point(const Cmnct_point &item);
    virtual ~Cmnct_point();
    ipc_state_t Get_state();
    std::string Get_name();
    void Set_name(std::string);
    Ipc_strm* Get_int_strm();
    void Set_int_strm(Ipc_strm *strm);
    Ipc_strm* Get_ext_strm();
    void Set_ext_strm(Ipc_strm *strm);
};
