#ifndef __OPER_H__
#define __OPER_H__
#include <string>
#include <map>
#include "ace/ace_header_full.h"
#include "Alog.h"

#define CHENGDU_USER_CODE "C280010006|user"

//********************parameter code define*************************

typedef enum _parameter_code
{
    PARAM_CODE_MASTERCTRL_BITSTR,           //控制主控器的开关量，字符串0000100等
    PARAM_CODE_CURTAIN_CTRL_OFFSET,
    PARAM_CODE_UFO_POWER_LED,
    PARAM_CODE_SPEAKER_NUM,
    PARAM_CODE_ZIGBEE_RGB,
    PARAM_CODE_ZIGBEE_SOCKET
}ENUM_PARAM_CODE;

//***********************operate string*******************************

#define OPER_STR_MC_OPEN "open"
#define OPER_STR_MC_CLOSE "clos"

#define OPER_STR_CURTAIN_POLLUP        "0001"
#define OPER_STR_CURTAIN_STOP            "0003"
#define OPER_STR_CURTAIN_PUSHDOWN "0002"
#define OPER_STR_CURTAIN_OFFSET         "0000"

#define OPER_STR_UFO_GETENV                "GetEnvment"
#define OPER_STR_UFO_LED_CTR                "powerLED"

#define OPER_STR_POWERAMPLIFIFER_SINGLESLIENCE      "mutesingle"
#define OPER_STR_POWERAMPLIFIFER_UNSINGLESLIENCE "unmutesingle"
#define OPER_STR_POWERAMPLIFIFER_PLAYNEXTSONG     "nextusb1sd2"
#define OPER_STR_POWERAMPLIFIFER_SWITCHCARD_SD   "selectsd1"
#define OPER_STR_POWERAMPLIFIFER_SWITCHCARD_USB "selectusb1"
#define OPER_STR_POWERAMPLIFIFER_PAUSE                    "pauseusb1sd2"
#define OPER_STR_POWERAMPLIFIFER_PLAY                      "playusb1sd2"

#define OPER_STR_ZIGBEE_COLORLIGHT_MOVETOCOLOR        "movetocolor"
#define OPER_STR_ZIGBEE_COLORLIGHT_MOVECOLOR             "movecolor"
#define OPER_STR_ZIGBEE_COLORLIGHT_STEPCOLOR               "stepcolor"
#define OPER_STR_ZIGBEE_COLORLIGHT_MOVETOHUE             "movetohue"
#define OPER_STR_ZIGBEE_COLORLIGHT_MOVEHUE                 "movehue"
#define OPER_STR_ZIGBEE_COLORLIGHT_STEPHUE                    "stephue"
#define OPER_STR_ZIGBEE_COLORLIGHT_MOVETOSAT              "movetosat"
#define OPER_STR_ZIGBEE_COLORLIGHT_MOVESAT                   "movesat"
#define OPER_STR_ZIGBEE_COLORLIGHT_STEPSAT                     "stepsat"
#define OPER_STR_ZIGBEE_COLORLIGHT_MOVETOHUEANDSAT "movetohueandsat"
#define OPER_STR_ZIGBEE_COLORLIGHT_MOVETOCOLORTEMP "movetocolortemp"


#define OPER_STR_ZIGBEE_COLORLIGHT_MOVETOLEVEL       "mv-to-level"
#define OPER_STR_ZIGBEE_COLORLIGHT_MOVELEVEL            "move"
#define OPER_STR_ZIGBEE_COLORLIGHT_STOPLEVEL              "stop"
#define OPER_STR_ZIGBEE_COLORLIGHT_ONOFFLEVEL           "o-mv-to-level"
#define OPER_STR_ZIGBEE_COLORLIGHT_STEPONOFFLEVEL    "o-step"
#define OPER_STR_ZIGBEE_COLORLIGHT_STEPLEVEL                "step"

#define OPER_STR_INSPECT_MASTERCTRL         "0001"
#define OPER_STR_INSPECT_BODYSENSCE        "0002"
#define OPER_STR_INSPECT_TFT                        "0003"
#define OPER_STR_INSPECT_POWERAMPLIFIER "0004"
#define OPER_STR_INSPECT_UFO                       "0005"
#define OPER_STR_INSPECT_LOCK                     "0006"
#define OPER_STR_INSPECT_COLOR_LIGHT        "0008"
#define OPER_STR_INSPECT_CURTAIN                "000A"

//*************************device type string**************************
#define TYPE_INSPECT                     "00"
#define TYPE_MASTR_CTRL              "20"
#define TYPE_COMMON_LIGTH       "20"
#define TYPE_SOCKET                      "20"
#define TYPE_CURTAIN                    "0A"
#define TYPE_POWER_AMPLIFIER     "40"
#define TYPE_ZIGBEE_ZCL_COLOR_LIGHT                "zcl-clolorlight"
#define TYPE_ZIGBEE_ZCL_SOCKET                "zcl-socket"
#define TYPE_MODEL                        "model"

//*****************************message type*****************************
#define MSG_TYPE_REQUEST_WEB          0
#define MSG_TYPE_REQUEST_MOBILE     1
#define MSG_TYPE_RESPONSE_RES         2
#define MSG_TYPE_RESPONSE_DATA      3
#define MSG_TYPE_REPORT                    4

int swapInt32(int value)
{
    return ((value & 0x000000FF) << 24) |
        ((value & 0x0000FF00) << 8) |
        ((value & 0x00FF0000) >> 8) |
        ((value & 0xFF000000) >> 24) ;
}

class Oper
{
public:
	Oper(){}
	~Oper(){}
protected:
	
    static void* FireRequst(void* param)
    {
        if (NULL == param)
            return NULL;
        Oper* pThis = reinterpret_cast<Oper*>(param);

        ACE_SOCK_Connector GlobalConnector;
        ACE_INET_Addr svr(8890, "192.168.60.226");
        ACE_SOCK_Stream peer;
        if (-1 == GlobalConnector.connect(peer, svr))
        {
            LOG_ERROR(" connect server bus failed ");
            return NULL;
        }
        char recvBuf[512]={0};
        ACE_Time_Value timeout = ACE_OS::gettimeofday()+ACE_Time_Value(4);
        peer.send_n(pThis->_cmdtoServer, pThis->_cmdLen);
        unsigned long startTime = GetCurrentTime();
        while (pThis->_modleCmdTimes--)
        {
            peer.recv(pThis->_recvedfromSvr, 512, &timeout);
            if (pThis->_modleCmdTimes!=0)
            {
                LOG_DEBUG("%s,", pThis->_recvedfromSvr+8);
            }
        }
        peer.close();

        unsigned long endTime = GetCurrentTime();
        pThis->_wasteTime = endTime - startTime;
        
        Sleep(100);
        ACE_OS::snprintf(recvBuf, 512, ",%d, %s, rsp:%s ,%u ,%u, %u",pThis->_operSequence, (pThis->_cmdtoServer+8) ,  pThis->_recvedfromSvr+8, pThis->_wasteTime,
            startTime, endTime);
        LOG_DEBUG("%s", recvBuf);

        return NULL;
    }

public:
    void MakeRequest()
    {
        //if (_operDeviceType.compare(TYPE_MODEL)!=0)
        //{
        //    memset(_cmdtoServer, 0, sizeof(_cmdtoServer));
        //    memset(_recvedfromSvr, 0, sizeof(_recvedfromSvr));
        //}

        if (_operDeviceType.compare(TYPE_MASTR_CTRL)==0)
        {
            _MakeMasterCtrl();
        }
        else if (_operDeviceType.compare(TYPE_CURTAIN)==0)
        {
            _MakeCurtainCtrl();
        }
        else if (_operDeviceType.compare(TYPE_INSPECT)==0)
        {
            _MakeInspect();
        }
        else if (_operDeviceType.compare(TYPE_POWER_AMPLIFIER) == 0)
        {
            _MakePowerAmplifier();
        }
        else if (_operDeviceType.compare(TYPE_ZIGBEE_ZCL_COLOR_LIGHT) == 0)
        {
            _MakeZigbeeColorLight();
        }
        else if (_operDeviceType.compare(TYPE_ZIGBEE_ZCL_SOCKET)==0)
        {
            _MakeZigbeeSocket();
        }
        else if (_operDeviceType.compare(TYPE_MODEL)==0)
        {
            _MakeModle();
        }

        int len = swapInt32(_cmdLen - 8);
        int client = MSG_TYPE_REQUEST_WEB;
        ACE_OS::memcpy(_cmdtoServer,&client, 4);
        ACE_OS::memcpy(_cmdtoServer+4, &len, 4);

        ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)FireRequst, this);
    }

private:
    void _MakeModle()
    {
        _cmdLen = strlen(_cmdtoServer+8)+8;
    }

    void _MakeColorLight()
    {
        
    }

    void _MakeZigbeeColorLight()
    {
        int iRet = ACE_OS::snprintf(_cmdtoServer+8, 500, "000e010000000002,zcl colorcontrol movetohueandsat %s 0xfe 0x01,send 0xF67F 0x01 0x01,zcl level-control mv-to-level 0xff 0x01,send 0xF67F 0x01 0x01",
                       _cmdParameters[PARAM_CODE_ZIGBEE_RGB].c_str());
         _cmdLen = iRet+8;
    }

    void _MakeZigbeeSocket()
    {
        int iRet = ACE_OS::snprintf(_cmdtoServer+8, 500,
            "000e010000000002,zcl on-off %s,send 0x526F 0x01 0x01",
            _cmdParameters[PARAM_CODE_ZIGBEE_SOCKET].c_str());
         _cmdLen = iRet+8;
    }

    //user40mutesingle037:3
    void _MakePowerAmplifier()
    {
        int iRet = ACE_OS::snprintf(_cmdtoServer+8, 500, "%s%s%s%s:%s", 
            CHENGDU_USER_CODE, TYPE_POWER_AMPLIFIER, _operateCmd.c_str(), _485addr.c_str(),
            _cmdParameters[PARAM_CODE_SPEAKER_NUM].c_str());
        _cmdLen = iRet+8;
    }

    void _MakeInspect()
    {
        int iRet = ACE_OS::snprintf(_cmdtoServer+8, 500, "%s%s%s%s:c%s", 
            CHENGDU_USER_CODE, "00", _operateCmd.c_str(), _485addr.c_str());
        _cmdLen = iRet+8;
    }

    void _MakeMasterCtrl()
    {
        int iRet = ACE_OS::snprintf(_cmdtoServer+8, 500, "%s%s%s%s:c%s", 
            CHENGDU_USER_CODE, "20", _operateCmd.c_str(), _485addr.c_str(), 
            _cmdParameters[PARAM_CODE_MASTERCTRL_BITSTR].c_str());
        _cmdLen = iRet+8;
    }
    
    void _MakeUFOGetEnv()
    {
        int iRet = 0;
        if (_cmdParameters[PARAM_CODE_UFO_POWER_LED].length() == 0)
        {
            iRet = ACE_OS::snprintf(_cmdtoServer+8, 500, "%s%s%s%s", 
                CHENGDU_USER_CODE, "50", _operateCmd.c_str(), _485addr.c_str());
        }else
        {
            iRet = ACE_OS::snprintf(_cmdtoServer+8, 500, "%s%s%s%s", 
                CHENGDU_USER_CODE, "50", _operateCmd.c_str(), _485addr.c_str(), 
                _cmdParameters[PARAM_CODE_UFO_POWER_LED].c_str());
        }

        _cmdLen = iRet+8;
    }

    void _MakeCurtainCtrl()
    {
        int iRet = 0;
        
        if (_operateCmd.compare("0002")==0 || _operateCmd.compare("0003")==0 || 0==_operateCmd.compare("0001"))
        {//push down, stop, pull up
            iRet = ACE_OS::snprintf(_cmdtoServer+8, 500, "%s%sCurtainControl%s%s", 
                CHENGDU_USER_CODE, "0A",  _485addr.c_str(), _operateCmd.c_str());
        }
        else
        {//poll offset
            iRet = ACE_OS::snprintf(_cmdtoServer+8, 500, "%s%sCurtainOffset%s%s", 
                CHENGDU_USER_CODE, "0A",  _485addr.c_str(), 
                _cmdParameters[PARAM_CODE_CURTAIN_CTRL_OFFSET].c_str());
        }
        _cmdLen = iRet+8;
    }

public:

    char _cmdtoServer[512];               //待发送命令缓冲区
    char _recvedfromSvr[512];            //从服务器返回的缓冲区
    int _cmdLen;                                //待发送命令长度


    std::string _username;                   //系统用户名
    std::string _operDeviceType;         //设备类型
    std::string _485addr;                     //设备485地址
    std::string _operateCmd;              //操作命令

    unsigned long _wasteTime;  //本次花销

    int _modleCmdTimes;                //模式包含的单个命令数
    int _operSequence;                    //运行次数
    int _intervalTime;                       //每次的时间间隔

    std::map<int, std::string>      _cmdParameters;       //操作需要传入的参数
};

#endif