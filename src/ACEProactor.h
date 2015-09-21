#ifndef __ACE_PROACTOR_H__
#define __ACE_PROACTOR_H__
#include "ace/ace_header_full.h"

class TPTCPAsynchServerImpl : public ACE_Service_Handler
{
public:
    TPTCPAsynchServerImpl(void);
    ~TPTCPAsynchServerImpl();

    void start_run();

    //���ⷽ���̳����ϼ���ACE_Service_Handler��
    virtual void open(ACE_HANDLE handle, ACE_Message_Block& message_block);
    virtual void handle_read_stream(const ACE_Asynch_Read_Stream::Result &result);
    virtual void handle_write_stream(const ACE_Asynch_Write_Stream::Result &result);
    virtual void handle_time_out(const ACE_Time_Value &tv, const void *act  = 0 );

private:
    //��ʼ����������
    int initiate_read_stream(const ACE_Asynch_Read_Stream::Result &asynch_read_result);

    //��������
    int send_data(int clientID, const char *data, int dataLen, unsigned int &id);

    //�첽��������
    ACE_Asynch_Read_Stream m_rs;

    //�첽д������
    ACE_Asynch_Write_Stream m_ws;

    //��������С
    size_t m_bufSize;
};


#endif