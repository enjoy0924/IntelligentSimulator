#include "Markup.h"
#include "Oper.h"
#include "Alog.h"

#define XML_HEADER        "<?xml version=\"1.0\"?>\r\n"

void BuildXml(std::string &sPrepareToSend)
{
    CMarkup xml;
    xml.SetDoc(XML_HEADER);
    xml.AddElem("Request");
    xml.IntoElem();
    xml.AddElem("ProxyId", 41);        //代理ID
    xml.AddElem("CommandId", 4001);
    xml.AddElem("Input");
    xml.IntoElem();
    xml.AddElem("AreaInfo");
    xml.IntoElem();
    xml.AddElem("AreaID", 1701);
    xml.AddElem("AreaCameraID", 32789);
    xml.OutOfElem();
    xml.AddElem("PlayType", 0);
    xml.OutOfElem();
    xml.OutOfElem();
    sPrepareToSend = xml.GetDoc();
}

void TESTLightOn(int iTimes = 10, int iFrequence = 3000)
{
    //socket/light on
    //int iTimes = 100;
    //int iFrequence = 3000;      //3 seconds
    LOG_DEBUG("======comon light on, time interval 3 second, 100 times======");
    while(iTimes--)
    {
        Oper* comonlighOpen = new Oper();
        comonlighOpen->_modleCmdTimes = 1;
        comonlighOpen->_operSequence = 101 - iTimes;
        comonlighOpen->_operDeviceType = TYPE_COMMON_LIGTH;
        comonlighOpen->_operateCmd = OPER_STR_MC_OPEN;
        comonlighOpen->_485addr = "003";
        comonlighOpen->_cmdParameters[PARAM_CODE_MASTERCTRL_BITSTR] = "00000010";
        comonlighOpen->MakeRequest();

        if (iFrequence > 0)
        {
            Sleep(iFrequence);
        }
    }
}

void TESTPollUpCurtain()
{
    int iTimes = 100;
    int iFrequence = 3000;     //3 seconds
    LOG_DEBUG("======curtain poll up, time interval 3 second, 100 times======");
    while(iTimes--)
    {
        Oper curtainOper;
        curtainOper._operSequence = 101 - iTimes;
        curtainOper._operDeviceType = TYPE_CURTAIN;
        curtainOper._operateCmd = OPER_STR_CURTAIN_POLLUP;
        curtainOper._485addr = "039";
        //curtainOper._cmdParameters[PARAM_CODE_MASTERCTRL_BITSTR] = "00010100";
        curtainOper.MakeRequest();

        Sleep(iFrequence);
    }
}

void TESTStopCurtain()
{
    int iTimes = 100;
    int iFrequence = 3000;     //3 seconds
    LOG_DEBUG("======curtain stop, time interval 3 second, 100 times======");
    while(iTimes--)
    {
        Oper curtainOper;
        curtainOper._operSequence = 101 - iTimes;
        curtainOper._operDeviceType = TYPE_CURTAIN;
        curtainOper._operateCmd = OPER_STR_CURTAIN_STOP;
        curtainOper._485addr = "039";
        //curtainOper._cmdParameters[PARAM_CODE_MASTERCTRL_BITSTR] = "00010100";
        curtainOper.MakeRequest();

        Sleep(iFrequence);
    }
}

void TESTPushDownCurtain()
{
    int iTimes = 100;
    int iFrequence = 3000;     //3 seconds
    LOG_DEBUG("======curtain push down, time interval 3 second, 100 times======");
    while(iTimes--)
    {
        Oper curtainOper;
        curtainOper._operSequence = 101 - iTimes;
        curtainOper._operDeviceType = TYPE_CURTAIN;
        curtainOper._operateCmd = OPER_STR_CURTAIN_PUSHDOWN;
        curtainOper._485addr = "240";
        curtainOper._modleCmdTimes = 1;
        //curtainOper._cmdParameters[PARAM_CODE_MASTERCTRL_BITSTR] = "00010100";
        curtainOper.MakeRequest();

        Sleep(iFrequence);
    }
}

void TESTCurtainOffset(int iTimes = 10, int iFrequence=3000)
{
    LOG_DEBUG("======curtain push down, time interval 3 second, 100 times======");
    while(iTimes--)
    {
        int offset = iTimes%3;
        std::string stroffset ;
        Oper* curtainOper = new Oper();
        curtainOper->_modleCmdTimes = 1;
        curtainOper->_operSequence = 101 - iTimes;
        curtainOper->_operDeviceType = TYPE_CURTAIN;
        curtainOper->_operateCmd = OPER_STR_CURTAIN_OFFSET;
        curtainOper->_485addr = "039";
        if (offset == 0)
        {
            stroffset = "075";
        }else if (offset == 1)
        {
            stroffset = "033";
        }else if (offset == 2)
        {
            stroffset = "050";
        }
        curtainOper->_cmdParameters[PARAM_CODE_CURTAIN_CTRL_OFFSET] = stroffset;
        curtainOper->MakeRequest();
        if (iFrequence > 0)
        {
            Sleep(iFrequence);
        }
    }
}

//user40mutesingle037:3

void TESTSilenceSpeaker()
{
    int iTimes = 100;
    int iFrequence = 3000;     //3 seconds
    LOG_DEBUG("======silence speaker, time interval 3 second, 100 times======");
    while(iTimes--)
    {
        int offset = (iTimes-1)%8;
        std::string stroffset ;
        Oper curtainOper;
        curtainOper._operSequence = 100 - iTimes;
        curtainOper._operDeviceType = TYPE_POWER_AMPLIFIER;
        curtainOper._operateCmd = OPER_STR_POWERAMPLIFIFER_SINGLESLIENCE;
        curtainOper._485addr = "037";
        if (offset == 0)
        {
            stroffset = "8";
        }else if (offset == 1)
        {
            stroffset = "1";
        }else if (offset == 2)
        {
            stroffset = "2";
        }else if (offset == 3)
        {
            stroffset = "3";
        }else if (offset == 4)
        {
            stroffset = "4";
        }else if (offset == 5)
        {
            stroffset = "5";
        }else if (offset == 6)
        {
            stroffset = "6";
        }else if (offset == 7)
        {
            stroffset = "7";
        }
        curtainOper._cmdParameters[PARAM_CODE_SPEAKER_NUM] = stroffset;
        curtainOper.MakeRequest();

        Sleep(iFrequence);
    }
}

void TESTOpenSpeaker()
{
    int iTimes = 100;
    int iFrequence = 3000;     //3 seconds
    LOG_DEBUG("======silence speaker, time interval 3 second, 100 times======");
    while(iTimes--)
    {
        int offset = (iTimes-1)%8;
        std::string stroffset = "8";
        Oper curtainOper;
        curtainOper._operSequence = 100 - iTimes;
        curtainOper._operDeviceType = TYPE_POWER_AMPLIFIER;
        curtainOper._operateCmd = OPER_STR_POWERAMPLIFIFER_UNSINGLESLIENCE;
        curtainOper._485addr = "037";
        if (offset == 0)
        {
            stroffset = "8";
        }else if (offset == 1)
        {
            stroffset = "1";
        }else if (offset == 2)
        {
            stroffset = "2";
        }else if (offset == 3)
        {
            stroffset = "3";
        }else if (offset == 4)
        {
            stroffset = "4";
        }else if (offset == 5)
        {
            stroffset = "5";
        }else if (offset == 6)
        {
            stroffset = "6";
        }else if (offset == 7)
        {
            stroffset = "7";
        }

        curtainOper._cmdParameters[PARAM_CODE_SPEAKER_NUM] = stroffset;
        curtainOper.MakeRequest();

        Sleep(iFrequence);
    }
}

void PlayNextSound()
{
    int iTimes = 100;
    int iFrequence = 3000;     //3 seconds
    LOG_DEBUG("======play next song, time interval 3 second, 100 times======");
    while(iTimes--)
    {
        int offset = (iTimes-1)%8;
        std::string stroffset = "8";
        Oper curtainOper;
        curtainOper._operSequence = 100 - iTimes;
        curtainOper._operDeviceType = TYPE_POWER_AMPLIFIER;
        curtainOper._operateCmd = OPER_STR_POWERAMPLIFIFER_PLAYNEXTSONG;
        curtainOper._485addr = "037";
        curtainOper.MakeRequest();

        Sleep(iFrequence);
    }
}

void TESTSwitchCard()
{
    int iTimes = 100;
    int iFrequence = 3000;     //3 seconds
    LOG_DEBUG("======switch card, time interval 3 second, 100 times======");
    while(iTimes--)
    {
        int offset = (iTimes-1)%2;
        std::string stroffset = "8";
        Oper curtainOper;
        curtainOper._operSequence = 100 - iTimes;
        curtainOper._operDeviceType = TYPE_POWER_AMPLIFIER;

        if (offset==0)
        {
            curtainOper._operateCmd = OPER_STR_POWERAMPLIFIFER_SWITCHCARD_SD;
        }
        else
        {
            curtainOper._operateCmd = OPER_STR_POWERAMPLIFIFER_SWITCHCARD_USB;
        }
        
        curtainOper._485addr = "037";
        curtainOper.MakeRequest();

        Sleep(iFrequence);
    }
}

void PlayPause()
{
    int iTimes = 100;
    int iFrequence = 3000;     //3 seconds
    LOG_DEBUG("======play pause, time interval 3 second, 100 times======");
    while(iTimes--)
    {
        int offset = (iTimes-1)%2;
        std::string stroffset = "8";
        Oper curtainOper;
        curtainOper._operSequence = 100 - iTimes;
        curtainOper._operDeviceType = TYPE_POWER_AMPLIFIER;
        if (offset==0)
        {
            curtainOper._operateCmd = OPER_STR_POWERAMPLIFIFER_PAUSE;
        }
        else
        {
            curtainOper._operateCmd = OPER_STR_POWERAMPLIFIFER_PLAY;
        }

        curtainOper._485addr = "037";
        curtainOper.MakeRequest();

        Sleep(iFrequence);
    }
}

void TESTZigbeeSocket()
{
    int iTimes = 100;
    int iFrequence = 3000;     //3 seconds
    LOG_DEBUG("======zigbee socket, time interval 3 second, 100 times======");
    while(iTimes--)
    {
        int offset = (iTimes-1)%2;
        std::string stroffset = "on";
        Oper curtainOper;
        curtainOper._operSequence = 100 - iTimes;
        curtainOper._operDeviceType = TYPE_ZIGBEE_ZCL_SOCKET;
        if (offset==0)
        {
            stroffset = "off";
            //curtainOper._operateCmd = OPER_STR_POWERAMPLIFIFER;
        }
        curtainOper._cmdParameters[PARAM_CODE_ZIGBEE_SOCKET] = stroffset;
        curtainOper.MakeRequest();

        Sleep(iFrequence);
    }
}

void TESTZigbeeColor()
{
    int iTimes = 100;
    int iFrequence = 3000;     //3 seconds
    LOG_DEBUG("======zigbee color, time interval 3 second, 100 times======");
    while(iTimes--)
    {
        int offset = (iTimes-1)%5;
        std::string stroffset = "0x22";
        Oper curtainOper;
        curtainOper._operSequence = 100 - iTimes;
        curtainOper._operDeviceType = TYPE_ZIGBEE_ZCL_COLOR_LIGHT;
        if (offset==0)
        {
            stroffset = "0xf8";
            //curtainOper._operateCmd = OPER_STR_POWERAMPLIFIFER;
        }else if (offset==1)
        {
            stroffset = "0xa8";
        }else if (offset==2)
        {
            stroffset = "0x58";
        }
        else if (offset==3)
        {
            stroffset = "0x08";
        }
        else if (offset==4)
        {
            stroffset = "0xd2";
        }
        curtainOper._cmdParameters[PARAM_CODE_ZIGBEE_RGB] = stroffset;
        curtainOper.MakeRequest();

        Sleep(iFrequence);
    }
}

void _TESTTEMPLATE()
{
    int iTimes = 1;
    int iFrequence = 3000;     //3 seconds
    LOG_DEBUG("======play pause, time interval 3 second, 100 times======");
    while(iTimes--)
    {
        int offset = (iTimes-1)%2;
        std::string stroffset = "8";
        Oper curtainOper;
        curtainOper._operSequence = 100 - iTimes;
        curtainOper._operDeviceType = TYPE_POWER_AMPLIFIER;
        curtainOper._operateCmd = OPER_STR_POWERAMPLIFIFER_SWITCHCARD_SD;
        curtainOper._485addr = "037";
        curtainOper.MakeRequest();

        //system("PAUSE");

        Oper pauseOper;
        pauseOper._operSequence = 100 - iTimes;
        pauseOper._operDeviceType = TYPE_POWER_AMPLIFIER;
        pauseOper._operateCmd = OPER_STR_POWERAMPLIFIFER_PAUSE;
        pauseOper._485addr = "037";
        pauseOper.MakeRequest();

        Sleep(iFrequence);
    }
}

#define LIGHT_ALL_ON            1
#define LIGHT_AND_CURTAIN 2

void TEST_MODLE(int iModel)
{
    Oper* ModleOper = new Oper();
    memset(ModleOper->_cmdtoServer, NULL, 512);
    memset(ModleOper->_recvedfromSvr, NULL, 512);
    switch(iModel)
    {
    case LIGHT_ALL_ON:
        ModleOper->_operDeviceType = TYPE_MODEL;
        ModleOper->_modleCmdTimes = 4;
        strcpy(ModleOper->_cmdtoServer+8, "C280010002|user20open003:c11111010|user20open004:c11111111|user20open006:c00000011|user20open005:c11000111");
        break;
    case LIGHT_AND_CURTAIN:
        break;
    default:
        break;
    }
    ModleOper->MakeRequest();
}

int ACE_TMAIN(int argc, ACE_TCHAR** argv)
{
    AlogInit("log4cplus.properties");

    ACE::init();
    //*************************************打开照明灯*************************************
    //TEST_MODLE(LIGHT_ALL_ON);
    //int i = 10;
    //while (i--)
    //{
    //    TESTLightOn(1, 0);
    //    TESTCurtainOffset(1, 0);
    //    Sleep(15*1000);
    //}
    //TESTPushDownCurtain();
    //TESTCurtainOffset(100);

    //*************************************************************************************
    //TESTSilenceSpeaker();
    //TESTOpenSpeaker();
    //PlayNextSound();
    //TESTSwitchCard();
    //PlayPause();
    //TESTZigbeeSocket();
    //TESTZigbeeColor();
    //_TESTTEMPLATE();
    //***********************************拉起窗帘*****************************************

    //**********************************关闭窗帘*********************************************

    //**********************************拉下窗帘*********************************************

    //**********************************偏移拉窗帘*********************************************
    //TESTCurtainOffset();
    
    system("pause");

    ACE::fini();
    return 0;
}