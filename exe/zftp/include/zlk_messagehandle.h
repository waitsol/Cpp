#ifndef __ZLK_MESSAGEHANDLE
#define __ZLK_MESSAGEHANDLE
#include <map>
#include "zlk_message.pb.h"
#include <functional>
enum class zlkMsg
{
    rm,
    ls,
    pull,
    push
};
using zlk_msg_func = std::function<void(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res)>;

class zlk_messagehandle
{
public:
    std::map<zlkMsg, zlk_msg_func> m_mapFunc;
};
#endif