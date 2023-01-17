#include "zftp_handmessage.h"
zftp_handleMessage::zftp_handleMessage()
{
    m_mapFunc[zlkMsg::rm] = std::bind(&zftp_handleMessage::hand_rm, this, std::placeholders::_1, std::placeholders::_2);
};
void zftp_handleMessage::hand_ls(const zftp_message::Pakcet &packet, zftp_message::response_msg &zlk_res){

};
void zftp_handleMessage::hand_rm(const zftp_message::Pakcet &packet, zftp_message::response_msg &zlk_res)
{
}
void zftp_handleMessage::hand_pull(const zftp_message::Pakcet &packet, zftp_message::response_msg &zlk_res)
{
}
void zftp_handleMessage::hand_push(const zftp_message::Pakcet &packet, zftp_message::response_msg &zlk_res)
{
}