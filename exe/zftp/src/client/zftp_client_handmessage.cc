#include "zftp_client_handmessage.h"
#include "zlk_log.h"
#include "fstream"
zftp_client_handleMessage::zftp_client_handleMessage()
{
    m_mapFunc[zlkMsg::rm] = std::bind(&zftp_client_handleMessage::hand_rm, this, std::placeholders::_1, std::placeholders::_2);
    m_mapFunc[zlkMsg::ls] = std::bind(&zftp_client_handleMessage::hand_ls, this, std::placeholders::_1, std::placeholders::_2);
    m_mapFunc[zlkMsg::pull] = std::bind(&zftp_client_handleMessage::hand_pull, this, std::placeholders::_1, std::placeholders::_2);
    m_mapFunc[zlkMsg::push] = std::bind(&zftp_client_handleMessage::hand_push, this, std::placeholders::_1, std::placeholders::_2);
    m_mapFunc[zlkMsg::pwd] = std::bind(&zftp_client_handleMessage::hand_pwd, this, std::placeholders::_1, std::placeholders::_2);
    m_mapFunc[zlkMsg::mkdir] = std::bind(&zftp_client_handleMessage::hand_mkdir, this, std::placeholders::_1, std::placeholders::_2);
    m_mapFunc[zlkMsg::cd] = std::bind(&zftp_client_handleMessage::hand_cd, this, std::placeholders::_1, std::placeholders::_2);
};
void zftp_client_handleMessage::hand_ls(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res)
{
    zftp_message::ls_response res;
    if (res.ParseFromString(packet.msgbody()))
    {
        if (res.ok())
        {
            printf("list:\n");

            for (int i = 0; i < res.list_size(); i++)
            {
                printf("id = %-3d size = %-12d  is_dir = %d  filename = %s  \n", res.list(i).id(), res.list(i).size(), res.list(i).type(), res.list(i).name().data());
            }
        }
        else
        {
            printf("ls error %s\n", res.error().data());
        }
    }
    else
    {
        ERR("ParseFromString error");
    }
};
void zftp_client_handleMessage::hand_rm(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res)
{
    zftp_message::response_msg res;
    if (res.ParseFromString(packet.msgbody()))
    {
        if (res.ok())
        {
            printf("rm ok\n");
        }
        else
        {
            printf("rm failed %s\n", res.error().data());
        }
    }
    else
    {
        ERR("ParseFromString error");
    }
}
void zftp_client_handleMessage::hand_pull(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res)
{
    zftp_message::pull_respon_msg res;
    if (res.ParseFromString(packet.msgbody()))
    {
        if (res.ok())
        {
            auto data = res.data();
            auto filename = res.save_name();

            int idx = filename.find_last_of('/');
            if (idx != -1)
            {
                string dir = filename.substr(0, idx);
                if (access(dir.data(), F_OK) != 0)
                {
                    string shell = "mkdir -p " + dir;
                    system(shell.data());
                }
            }
            ofstream ofs(filename, ios::binary);
            if (ofs.is_open())
            {
                ofs.write(data.data(), data.length());
                printf("pull success to %s\n", filename.data());
                ofs.close();
            }
            else
            {
                printf("save failed %s\n", filename.data());
            }
        }
        else
        {
            printf("pull failed %s\n", res.data().data());
        }
    }
    else
    {
        ERR("ParseFromString error");
    }
}
void zftp_client_handleMessage::hand_push(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res)
{
    zftp_message::response_msg res;
    if (res.ParseFromString(packet.msgbody()))
    {
        if (res.ok())
        {
            printf("push ok\n");
        }
        else
        {
            printf("push failed %s\n", res.error().data());
        }
    }
    else
    {
        ERR("ParseFromString error");
    }
}

void zftp_client_handleMessage::hand_pwd(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res)
{
    zftp_message::response_msg res;
    if (res.ParseFromString(packet.msgbody()))
    {
        printf("%s\n", res.error().data());
    }
    else
    {
        ERR("ParseFromString error");
    }
}

void zftp_client_handleMessage::hand_cd(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res)
{
    zftp_message::response_msg res;
    if (res.ParseFromString(packet.msgbody()))
    {
        if (res.ok())
        {
            printf("cd ok\n");
        }
        else
        {
            printf("cd failed %s\n", res.error().data());
        }
    }
    else
    {
        ERR("ParseFromString error");
    }
}

void zftp_client_handleMessage::hand_mkdir(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res)
{
    zftp_message::response_msg res;
    if (res.ParseFromString(packet.msgbody()))
    {
        if (res.ok())
        {
            printf("mkdir ok\n");
        }
        else
        {
            printf("mkdir failed %s\n", res.error().data());
        }
    }
    else
    {
        ERR("ParseFromString error");
    }
}
