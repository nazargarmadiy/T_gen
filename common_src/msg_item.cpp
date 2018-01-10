#include <string.h>
#include <exception>
#include <stdexcept>
#include <stdlib.h>
#include "msg_item.h"

void Msg_item_data::Alloc()
{
    if (size)
    {
        if (nullptr != data)
        {
            throw std::logic_error("ptr is not null");
        }
        if (type.empty())
        {
            throw std::logic_error("type is empty");
        }
        data = calloc(1, size);
        if (nullptr == data)
        {
            throw std::bad_alloc();
        }
    }
};

void Msg_item_data::Free()
{
    if (nullptr != data)
    {
        free(data);
        size = 0;
        type.clear();
    }
};

void Msg_item_pkd::Alloc()
{
    if (size)
    {
        if (nullptr != data)
        {
            throw std::logic_error("ptr is not null");
        }
        data = calloc(1, size);
        if (nullptr == data)
        {
            throw std::bad_alloc();
        }
    }
};

void Msg_item_pkd::Free()
{
    if (nullptr != data)
    {
        free(data);
        size = 0;
    }
};

Msg_item::Msg_item() : id(0), sequense(false), sequense_id(0), destination(""), source(""),
                       name(""), data({0, "", nullptr}){};

Msg_item::Msg_item(int msg_id,
                   bool msg_seq,
                   int msg_seq_id,
                   const std::string &msg_dst,
                   const std::string &msg_src,
                   const std::string &msg_name,
                   const struct Msg_item_data &msg_data) : id(msg_id), sequense(msg_seq), sequense_id(msg_seq_id),
                                                           destination(msg_dst), source(msg_src), name(msg_name)
{
    data.size = msg_data.size;
    data.type = msg_data.type;
    data.data = nullptr;
    data.Alloc();
    memcpy(data.data, msg_data.data, data.size);
};

Msg_item::~Msg_item()
{
    data.Free();
};

void Msg_item::Set_id(int msg_id)
{
    id = msg_id;
};

const int Msg_item::Get_id()
{
    return id;
};

void Msg_item::Set_data(const struct Msg_item_data *msg_data)
{
    if (nullptr == msg_data)
    {
        throw std::invalid_argument("msg_data is null");
    }
    data.size = msg_data->size;
    data.type = msg_data->type;
    if (nullptr != msg_data->data || msg_data->size)
    {
        if ((msg_data->type).empty())
        {
            throw std::invalid_argument("data is represented, but data type empty");
        }
        else if (!msg_data->size)
        {
            throw std::invalid_argument("data is represented, but data size is 0");
        }
        else if (nullptr == msg_data->data)
        {
            throw std::invalid_argument("data size is not 0 but data is not represented");
        }
        data.Alloc();
        memcpy(data.data, msg_data->data, msg_data->size);
    }
};

struct Msg_item_data Msg_item::Get_data()
{
    Msg_item_data item_data = data;
    item_data.data = nullptr;
    if (item_data.size && nullptr != data.data)
    {
        item_data.data = malloc(item_data.size);
        if (nullptr == item_data.data)
        {
            throw std::bad_alloc();
        }
        memcpy(item_data.data, data.data, item_data.size);
    }
    return item_data;
};

void Msg_item::Set_seq(bool msg_seq)
{
    sequense = msg_seq;
};

const bool Msg_item::Get_seq()
{
    return sequense;
};

void Msg_item::Set_seq_id(int msg_seq_id)
{
    sequense_id = msg_seq_id;
};

const int Msg_item::Get_seq_id()
{
    return sequense_id;
};

void Msg_item::Set_dst(const std::string &msg_dst)
{
    if (msg_dst.empty())
    {
        throw std::invalid_argument("empty destination");
    }
    destination = msg_dst;
};

const std::string Msg_item::Get_dst()
{
    return destination;
};

void Msg_item::Set_src(const std::string &msg_src)
{
    if (msg_src.empty())
    {
        throw std::invalid_argument("empty source");
    }
    source = msg_src;
};

const std::string Msg_item::Get_src()
{
    return source;
};

void Msg_item::Set_name(const std::string &msg_name)
{
    if (msg_name.empty())
    {
        throw std::invalid_argument("empty name");
    }
    name = msg_name;
};

const std::string Msg_item::Get_name()
{
    return name;
};

struct Msg_item_pkd Msg_item::Serialize(Msg_item &item)
{
    Msg_item_data  item_data = item.Get_data();
    unsigned int   total_size;
    unsigned int   src_len = item.Get_src().length() + 1;
    unsigned int   dst_len = item.Get_dst().length() + 1;
    unsigned int   name_len = item.Get_name().length() + 1;
    unsigned int   type_len = item_data.type.length() + 1;
    const char    *src_str = item.Get_src().c_str();
    const char    *dst_str = item.Get_dst().c_str();
    const char    *name_str = item.Get_name().c_str();
    const char    *type_str = item_data.type.c_str();
    int            id = item.Get_id();
    int            seq_id = item.Get_seq_id();
    bool           seq = item.Get_seq();
    char          *ptr = nullptr;
    Msg_item_pkd   item_pkd;

    /*               id            seq          seq_id   */
    total_size = sizeof(id) + sizeof(seq) + sizeof(seq_id);
    /* append place for  dst, src, name values*/
    total_size += sizeof(src_len) + src_len;
    total_size += sizeof(dst_len) + dst_len;
    total_size += sizeof(name_len) + name_len;
    /* append place for data -  size, type, data */
    total_size += sizeof(item_data.size);
    total_size += sizeof(type_len) + item_data.type.length();
    total_size += item.data.size;

    memset(&item_pkd, 0, sizeof(item_pkd));
    item_pkd.size = total_size;
    item_pkd.Alloc();

    ptr = (char *)item_pkd.data;

    /* append id, seq, seq_id */
    memcpy(ptr, &id, sizeof(id));
    ptr += sizeof(id);
    memcpy(ptr, &seq, sizeof(seq));
    ptr += sizeof(seq);
    memcpy(ptr, &seq_id, sizeof(seq_id));
    ptr += sizeof(seq_id);

    /* append dst, src, name, and lenght for each field */
    mempcpy(ptr, &dst_len, sizeof(dst_len));
    ptr += sizeof(dst_len);
    memcpy(ptr, dst_str, dst_len);
    ptr += dst_len;

    mempcpy(ptr, &src_len, sizeof(src_len));
    ptr += sizeof(src_len);
    memcpy(ptr, src_str, src_len);
    ptr += src_len;

    mempcpy(ptr, &name_len, sizeof(name_len));
    ptr += sizeof(name_len);
    memcpy(ptr, name_str, name_len);
    ptr += name_len;

    /* append data - size, type, data, and lenght for type */
    memcpy(ptr, &(item_data.size), sizeof(item_data.size));
    ptr += sizeof(item_data.size);

    memcpy(ptr, &type_len, sizeof(type_len));
    ptr += sizeof(type_len);
    memcpy(ptr, type_str, type_len);
    ptr += type_len;

    memcpy(ptr, item_data.data, item_data.size);

    item_data.Free();
    return item_pkd;
};

Msg_item *Msg_item::Deserialize(struct Msg_item_pkd *item)
{
    Msg_item_data  item_data;
    unsigned int   total_size;
    unsigned int   src_len = 0;
    unsigned int   dst_len = 0;
    unsigned int   name_len = 0;
    unsigned int   type_len = 0;
    char          *src_str = nullptr;
    char          *dst_str = nullptr;
    char          *name_str = nullptr;
    char          *type_str = nullptr;
    char          *ptr = nullptr;
    int            id = 0;
    int            seq_id = 0;
    bool           seq = false;

    if (nullptr == item)
    {
        throw std::invalid_argument("item is nullptr");
    }

    memset(&item_data, 0, sizeof(item_data));
    ptr = (char *)item->data;

    /* get id, seq, seq_id */
    id = *((int *)ptr);
    ptr += sizeof(id);
    seq = *((bool *)ptr);
    ptr += sizeof(bool);
    seq_id = *((int *)ptr);
    ptr += sizeof(seq_id);

    /* get dst, src, name */
    dst_len = *((unsigned int *)ptr);
    ptr += sizeof(unsigned int);
    dst_str = (char *)malloc(dst_len);
    if (nullptr == dst_str)
    {
        throw std::bad_alloc();
    }
    memcpy(dst_str, ptr, dst_len);
    ptr += dst_len;

    src_len = *((unsigned int *)ptr);
    ptr += sizeof(unsigned int);
    src_str = (char *)malloc(src_len);
    if (nullptr == src_str)
    {
        throw std::bad_alloc();
    }
    memcpy(src_str, ptr, src_len);
    ptr += src_len;

    name_len = *((unsigned int *)ptr);
    ptr += sizeof(unsigned int);
    name_str = (char *)malloc(name_len);
    if (nullptr == name_str)
    {
        throw std::bad_alloc();
    }
    memcpy(name_str, ptr, name_len);
    ptr += name_len;

    /* get data - size, type, data */
    item_data.size = *((unsigned int *)ptr);
    ptr += sizeof(unsigned int);

    type_len = *((unsigned int *)ptr);
    ptr += sizeof(unsigned int);
    type_str = (char *)malloc(type_len);
    if (nullptr == type_str)
    {
        throw std::bad_alloc();
    }
    memcpy(type_str, ptr, type_len);
    item_data.type = type_str;
    ptr += type_len;

    item_data.Alloc();
    memcpy(item_data.data, ptr, item_data.size);

    Msg_item *ret_item = new Msg_item(id, seq, seq_id, dst_str, src_str, name_str, item_data);

    item_data.Free();
    free(dst_str);
    free(src_str);
    free(name_str);
    free(type_str);

    return ret_item;
};