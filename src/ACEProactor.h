#ifndef __ACE_PROACTOR_H__
#define __ACE_PROACTOR_H__
#include "ace/ace_header_full.h"

class TPTCPAsynchServerImpl : public ACE_Service_Handler
{
public:
    TPTCPAsynchServerImpl(void);
    ~TPTCPAsynchServerImpl();

    void start_run();

    //虚拟方法继承自上级的ACE_Service_Handler类
    virtual void open(ACE_HANDLE handle, ACE_Message_Block& message_block);
    virtual void handle_read_stream(const ACE_Asynch_Read_Stream::Result &result);
    virtual void handle_write_stream(const ACE_Asynch_Write_Stream::Result &result);
    virtual void handle_time_out(const ACE_Time_Value &tv, const void *act  = 0 );

private:
    //初始化读数据流
    int initiate_read_stream(const ACE_Asynch_Read_Stream::Result &asynch_read_result);

    //发送数据
    int send_data(int clientID, const char *data, int dataLen, unsigned int &id);

    //异步读数据流
    ACE_Asynch_Read_Stream m_rs;

    //异步写数据流
    ACE_Asynch_Write_Stream m_ws;

    //缓冲区大小
    size_t m_bufSize;
};


#endif