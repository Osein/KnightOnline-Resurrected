#ifndef __PACKET_DEF_H_
#define __PACKET_DEF_H_

///////////////////////////////////////////////////////////////
// Packet Define 
///////////////////////////////////////////////////////////////
#define VERSION_REQ				0x01	// Send 
#define DOWNLOAD_INFO_REQ		0x02	// Send - s1(�������) | Recv FTP
#define LOGIN_REQ				0x03	// Send - s1(ID����) str1(ID���ڿ�:20����Ʈ����) s1(PW����) str1(PW���ڿ�:12����Ʈ����) | Recv - b1(0:���� 1:���� 2:ID���� 3:PWƲ�� 4:����������)
#define MGAME_LOGIN_REQ			0x04	// !!! LOGIN_REQ !!! �� ����
#define SERVER_LIST				0x05	// Recv - b1(ServerCount) loop(	s1(IP����), str1(IP���ڿ�), s1(�����̸�����), str1(�����̸����ڿ�), s1(���������ο�) )
///////////////////////////////////////////////////////////////

#endif // end of #ifndef __PACKET_DEF_H_
