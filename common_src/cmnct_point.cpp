#include "cmnct_point.h"

Cmnct_point::Cmnct_point(): name(""), int_strm(nullptr), ext_strm(nullptr),
                            cbck_map(), state(INITIALIZED) {};

Cmnct_point::Cmnct_point(std::string                               &nm,
                         Ipc_strm                                  *int_s,
						 Ipc_strm                                  *ext_s,
						 std::map<std::string, msg_clbck_func_ptr> &cbcks):
name(nm), int_strm(int_s), ext_strm(ext_s), cbck_map(cbcks), state(INITIALIZED) {};

Cmnct_point::Cmnct_point(const Cmnct_point &item){
	name = item.name;
	int_strm = item.int_strm;
	ext_strm = item.ext_strm;
	cbck_map = item.cbck_map;
	state = item.state;
}

Cmnct_point::~Cmnct_point(){};

ipc_state_t Cmnct_point::Get_state(){
	return state;
};

std::string Cmnct_point::Get_name(){
	return name;
};

void Cmnct_point::Set_name(std::string nm){
	if(nm.empty()){
		throw std::invalid_argument("empty name");
	}
	name = nm;
};

Ipc_strm* Cmnct_point::Get_int_strm(){
	return int_strm;
};

void Cmnct_point::Set_int_strm(Ipc_strm *strm){
	if(nullptr == strm){
		throw std::invalid_argument("strm is null");
	}
	/*do we need to check state ?*/
	int_strm = strm;
};

Ipc_strm* Cmnct_point::Get_ext_strm(){
	return ext_strm;
};

void Cmnct_point::Set_ext_strm(Ipc_strm *strm){
	if(nullptr == strm){
		throw std::invalid_argument("strm is null");
	}
	/*do we need to check state ?*/
	ext_strm = strm;
};
