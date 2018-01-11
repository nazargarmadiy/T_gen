#include <stdio.h>
#include <string.h>
#include "common_src/msg_item.h"

int main(){
    printf("test message\n");

    Msg_item item;
    item.Set_id(7);
    item.Set_seq_id(1);
    item.Set_seq(true);
    item.Set_dst("Destination");
    item.Set_src("Source");
    item.Set_name("Name");
    
    Msg_item_data item_data;
    item_data.type = "String";
    std::string str("Additional info for item;");
    item_data.size = str.length() + 1;
    const char * cstr = str.c_str();
    item_data.data = nullptr;
    item_data.Alloc();
    memcpy(item_data.data, cstr, item_data.size);

    item.Set_data(&item_data);
    item_data.Free();

    Msg_item_pkd pkd = Msg_item::Serialize(item);

    Msg_item unpacked = Msg_item::Deserialize(&pkd);
    Msg_item_data data = unpacked.Get_data_cpy();
    printf("Id %i, seq_id %u, dst: %s, src: %s, name: %s, data type: %s, data %s\n",
           unpacked.Get_id(), unpacked.Get_seq_id(), unpacked.Get_dst().c_str(),
           unpacked.Get_src().c_str(), unpacked.Get_name().c_str(),
           data.type.c_str(), (char*)data.data);
           data.Free();
    getchar();
    pkd.Free();
    Msg_item unpacked_1 = unpacked;
    printf("\n Exiting...\n");
    return 0;
}
