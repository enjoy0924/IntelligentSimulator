#include "ACEProactor.h"

//做客户端链接管理类
//typedef ACE_Map_Manager<ACE_HANDLE, ConnectionBean *, ACE_Null_Mutex> ConnectionMap;
//typedef ACE_Map_Iterator<ACE_HANDLE, ConnectionBean *, ACE_Null_Mutex> ConnectionIterator;
//typedef ACE_Map_Entry<ACE_HANDLE, ConnectionBean *> ConnectionEntry;
//class Globle
//{
//public:
//    Globle(void);
//    ~Globle(void);
//    ITPServer* server_;
//    ConnectionMap _connections;
//    unsigned int getIndex(void); 
//    long getTimerId(void);
//private:
//    unsigned int index_;
//    long timerId_;
//};
//typedef ACE_Singleton<Globle, ACE_Null_Mutex> GlobleSingleton;



TPTCPAsynchServerImpl::TPTCPAsynchServerImpl():m_bufSize(1024)
{

}

TPTCPAsynchServerImpl::~TPTCPAsynchServerImpl()
{

}

void TPTCPAsynchServerImpl::open(ACE_HANDLE handle, ACE_Message_Block& message_block)
{
    ACE_DEBUG((LM_DEBUG, "%N: %l: TPTCPAsynchServerImpl::open()...\n"));
    //构造读流
    if (m_rs.open(*this, handle) == -1)
    {
        ACE_ERROR((LM_ERROR, "%N: %l: TPTCPAsynchServerImpl::open()... construct read stream failed"));
        return;
    }

    if (m_ws.open(*this, handle) == -1)
    {
        ACE_ERROR((LM_ERROR, "%N: %l: TPTCPAsynchServerImpl::open()... construct write stream failed"));
        return;
    }

    //获取客户端链接地址和端口
    ACE_INET_Addr addr;
    ACE_SOCK_SEQPACK_ASSOCIATION ass = ACE_SOCK_SEQPACK_ASSOCIATION(handle);

    size_t addr_size = 1;
    ass.get_local_addrs(&addr, addr_size);
    
    //this->server_->onClientConnect((int)handle, addr.get_ip_address(), addr.get_port_number());

    if (message_block.length() != 0)
    {
        // 复制消息块
        ACE_Message_Block &duplicate =  *message_block.duplicate();
        // 伪造读结果，以便进行读完成回调
        ACE_Asynch_Read_Stream_Result_Impl *fake_result =
            ACE_Proactor::instance ()->create_asynch_read_stream_result (this->proxy(),
            this->handle_,
            duplicate,
            1024,
            0,
            ACE_INVALID_HANDLE,
            0,
            0);
        size_t bytes_transferred = message_block.length ();
        // Accept事件处理完成，wr_ptr指针会被向前移动，将其移动到开始位置
        duplicate.wr_ptr (duplicate.wr_ptr () - bytes_transferred);
        // 这个方法将调用回调函数
        fake_result->complete (message_block.length (), 1, 0);
        // 销毁伪造的读结果
        delete fake_result;
    }
    else
    {
        ACE_Message_Block *mb = 0;
        ACE_NEW(mb, ACE_Message_Block(m_bufSize));
        if (m_rs.read (*mb, mb->size () - 1) == -1)
        {
            delete mb;
            ACE_ERROR ((LM_ERROR, "%N:%l:open init read failed!"));
            return;
        }
    }
}

void TPTCPAsynchServerImpl::handle_time_out(const ACE_Time_Value &tv, const void *act /* = 0 */ )
{
    //超时处理
    ACE_DEBUG((LM_DEBUG, "accept a time out event!"));
    //////////////////////////////////////////////////////////////////////////
}

void TPTCPAsynchServerImpl::handle_read_stream(const ACE_Asynch_Read_Stream::Result &result)
{
    char* temp_result = result.message_block().rd_ptr();
    result.message_block().rd_ptr()[result.bytes_transferred()] = '\0';
    ACE_DEBUG ((LM_DEBUG, "********************\n"));
    ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_to_read", result.bytes_to_read()));
    ACE_DEBUG((LM_DEBUG, "information here is below:\n%s\n", temp_result));
    ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "handle", result.handle()));
    ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_transfered", result.bytes_transferred()));
    ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "act", (u_long) result.act()));
    ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "success", result.success()));
    ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "completion_key", (u_long) result.completion_key()));
    ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "error", result.error()));
    ACE_DEBUG ((LM_DEBUG, "********************\n"));
    result.message_block().release();
    if (this->initiate_read_stream(result) == -1)
    {
        ACE_ERROR((LM_ERROR, "%N:%l:read stream failed!connection closed, remove it:%d\n", result.handle()));
        //如果发现读写出错的话，直接关闭链接
        //        closeConnection(result.handle());
    } 
}

int TPTCPAsynchServerImpl::initiate_read_stream (const ACE_Asynch_Read_Stream::Result &result)
{
    ACE_DEBUG((LM_TRACE, "%N:%l:TPTCPAsynchServerImpl::initiate_read_stream()\n"));
    //创建一个消息块。这个消息块将用于从套接字中异步读 
    ACE_Message_Block *mb = new ACE_Message_Block(m_bufSize);
    if (mb == NULL)
    {
        ACE_DEBUG((LM_ERROR, "%N:%l:can't allock ACE_Message_Block.\n")); 
        return -1;
    }

    if (m_rs.read (*mb, mb->size () - 1) == -1)
    {
        delete mb;
        ACE_ERROR_RETURN ((LM_ERROR, "%N:%l:rs->read() failed, clientID=%d\n", result.handle()),  -1);
    }
    return 0;
}

int TPTCPAsynchServerImpl::send_data(int clientID, const char *data, int dataLen, unsigned int &id)
{
    ACE_DEBUG((LM_DEBUG, "TPTCPAsynchServerImpl::send_data(void)\n"));
    ACE_Message_Block *mb; 
    ACE_NEW_RETURN(mb, ACE_Message_Block(dataLen + 1), -1);
    mb->wr_ptr((char*)data);                  
    ACE_OS::memcpy(mb->base(),(char*)data, dataLen);
    
    id = 0/* GlobleSingleton::instance()->getIndex()*/;       //获取一个ID值，先赋值为0

    mb->msg_type((int)id);
    //向操作系统发送数据
    //if (connection->ws->write(*mb , dataLen ) == -1)
    //{
    //    ACE_ERROR_RETURN((LM_ERROR, "%N:%l:sendData failed! clientID=%d ", clientID),-1);
    //}
    return 0;
}

void
TPTCPAsynchServerImpl::handle_write_stream (const ACE_Asynch_Write_Stream::Result &result)
{
    ACE_DEBUG ((LM_DEBUG,"handle_write_stream called\n"));
    // Reset pointers.
    result.message_block().rd_ptr(result.message_block().rd_ptr() - result.bytes_transferred());
    ACE_DEBUG ((LM_DEBUG, "********************\n"));
    ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_to_write", result.bytes_to_write()));
    ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "handle", result.handle()));
    ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "bytes_transfered", result.bytes_transferred()));
    ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "act", (u_long) result.act()));
    ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "success", result.success()));
    ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "completion_key", (u_long) result.completion_key()));
    ACE_DEBUG ((LM_DEBUG, "%s = %d\n", "error", result.error()));
    ACE_DEBUG ((LM_DEBUG, "********************\n"));
#if 0
    ACE_DEBUG ((LM_DEBUG, "%s = %s ", "message_block", result.message_block ().rd_ptr ()));
#endif
    // Release message block.
    result.message_block().release();
}

void TPTCPAsynchServerImpl::start_run()
{
    //创建一个端口监听器
    //ACE_Asynch_Acceptor<TPTCPAsynchServerImpl> acceptor_;

    ////固定地址和端口
    //ACE_INET_Addr addr(8089, "127.0.0.1");
    //acceptor_.open(addr, 8 * 1024, 1);
}

