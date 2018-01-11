#include <string>

#pragma once

struct Msg_item_pkd
{
    unsigned int size;
    void *data;
    void Alloc();
    void Free();
};

struct Msg_item_data
{
    unsigned int size;
    std::string type;
    void *data;
    void Alloc();
    void Free();
};

class Msg_item
{
  private:
    int id;
    bool sequense;
    int sequense_id;
    std::string destination;
    std::string source;
    std::string name;
    struct Msg_item_data data;
  public:
    Msg_item();
    Msg_item(int msg_id,
             bool msg_seq,
             int msg_seq_id,
             const std::string &msg_dst,
             const std::string &msg_src,
             const std::string &msg_name,
             const struct Msg_item_data &msg_data);
    Msg_item(const Msg_item &item);
    Msg_item & operator = (const Msg_item &item);
    virtual ~Msg_item();
    void Set_id(int msg_id);
    const int Get_id();
    void Set_data(const struct Msg_item_data *msg_data);
    struct Msg_item_data Get_data_cpy();
    void Set_seq(bool msg_seq);
    const bool Get_seq();
    void Set_seq_id(int msg_seq_id);
    const int Get_seq_id();
    void Set_dst(const std::string &msg_dst);
    const std::string Get_dst();
    void Set_src(const std::string &msg_src);
    const std::string Get_src();
    void Set_name(const std::string &msg_name);
    const std::string Get_name();
    static struct Msg_item_pkd Serialize(Msg_item &item);
    static Msg_item Deserialize(struct Msg_item_pkd *item);
};
