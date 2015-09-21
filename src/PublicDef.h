#ifndef __PUBLIC_DEF_H__
#define __PUBLIC_DEF_H__

#include <string>
#include <vector>

#define SAVE_FILENAME "result.xls"
#define DEVICE_NAME_LEN 120

typedef struct _NETWORK_NODE_
{
    char s_devicename[DEVICE_NAME_LEN];
    std::string s_node_ip;
    std::string s_indexcode;
    int n_port;
    std::string s_username;
    std::string s_password;
    int n_status;
    _NETWORK_NODE_():s_node_ip(""),
        s_indexcode(""),
        n_port(0),
        n_status(0),
        s_password("12345"),
        s_username("admin")
    {
        memset((void*)s_devicename, NULL, sizeof(s_devicename));
    }
}NETWORK_NODE;


typedef enum _ENUM_IPC_STATUS
{
    STATUS_PORT_OFF,                             //端口未开启，可能是设备离线
    STATUS_PORT_OK,                               //端口开启，登录失败
    STATUS_IPC_LOGIN_OK,                     //登录成功，获取设备信息失败
    STATUS_IPC_DEV_OK                          //登录成功，获取信息成功
}ENUM_IPC_STATUS;

#define DEFAULT_DEV_PORT 8000

typedef struct _DVR_IPC_SET
{
    std::string s_username;
    std::string s_password;
    std::string s_ip;                 //device ip
    int n_port;                         //device port
    int n_device_type;               //device type
    int n_chan_num;                 //device channel number
    std::string s_device_type; //device type
    std::string s_name;          //device name
    std::string s_version;       //device version
    std::string s_serial_num;
    std::vector<std::string> s_chan_name_list;    //设备通道名称列表
    ENUM_IPC_STATUS enum_status;
    _DVR_IPC_SET():s_username(""),
        s_password(""),
        s_ip(""),
        s_name("unknown"),
        s_version(""),
        s_serial_num(""),
        s_device_type(""),
        n_port(DEFAULT_DEV_PORT),
        n_device_type(0),
        n_chan_num(0),
        enum_status(STATUS_PORT_OFF)
    {}
}DVR_IPC_SET;

#endif