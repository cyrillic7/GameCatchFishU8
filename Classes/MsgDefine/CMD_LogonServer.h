#ifndef CMD_LONGON_HEAD_FILE
#define CMD_LONGON_HEAD_FILE


#include "GameGlobalSetting.h"
#include "GolbalDefine/Define.h"
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////////////

#define	 MAX_CHAMPION_TEXT		 64										//�ھ���ʾ
#define  LEN_MATCHTYPE			 32
#define  LEN_REMARK				 32
#define  LEN_AWARD				 32
#define  LEN_MATCHITEMAWARD		 10
#define  LEN_MATCHGAMECOUNT		 10
#define  LEN_MATCHGAMETYPE		 5
#define  LEN_MATCHAWARD			 50	
#define  LEN_MATCHINFO			 30	
#define  LEN_MATCHID			 30	
#define  TCP_MAX_CONNECT		 10000
#define  LEN_ADDR				 16
#define  MAX_MATCHSERVERID		 200									//��������id
#define  MAX_ROOM_NUM			 200									//�����������
#define  MAX_ROOM_NUM			 200									//�����������
#define  MAX_ROOM_TYPE_NUM		 20										//һ��Ϸ������

//��¼����
#define MDM_GP_LOGON				1									//�㳡��¼

//��¼ģʽ
#define SUB_GP_LOGON_GAMEID			1									//I D ��¼
#define SUB_GP_LOGON_ACCOUNTS		2									//�ʺŵ�¼
#define SUB_GP_REGISTER_ACCOUNTS	3									//ע���ʺ�
#define SUB_GP_LOGON_LOGOUT_GAMEID 	4									//�û��˳�
#define SUB_GP_LOBBY_IP 			5									//���������ip

//��¼���
#define SUB_GP_LOGON_SUCCESS		100									//��¼�ɹ�
#define SUB_GP_LOGON_FAILURE		101									//��¼ʧ��
#define SUB_GP_LOGON_FINISH			102									//��¼���
#define SUB_GP_VALIDATE_MBCARD      103                                 //��¼ʧ��

#define SUB_GP_GET_CAPTCHA			106									//��ȡ��֤��
#define SUB_GP_SEND_CAPTCHA			107									//������֤��



//����ʾ
#define SUB_GP_UPDATE_NOTIFY		200									//����ʾ

//////////////////////////////////////////////////////////////////////////////////
//

#define MB_VALIDATE_FLAGS           0x01                                //Ч���ܱ�
#define LOW_VER_VALIDATE_FLAGS      0x02                                //Ч��Ͱ汾

//shop 
#define SUB_GP_GIFT 				108									//��Ʒ��
#define SUB_GP_PROPERTY				109									//����
#define SUB_GP_BUYGIFT				110									//������Ʒ��,����
#define SUB_GP_KNAPSACK				111									//����
#define SUB_GP_KNAPSACKLOG			112									//���������ʾ
#define SUB_GP_USE_KNAPSACKLOG		113									//ʹ�ñ���

//���������
#define SUB_GP_AUCTION				114									//����Ʒ
#define SUB_GP_SELL_AUCTION			115									//��������Ʒ
#define SUB_GP_BUY_AUCTION			116									//��������Ʒ
#define SUB_GP_CANCEL_AUCTION		117									//��������Ʒ
#define SUB_GP_AUCTION_RECORD		118									//������¼
#define SUB_GP_MYAUCTION_RECORD		119									//�ҵ�����
#define SUB_GP_AUCTION_HISTORY_RECORD	120								//������ʷ��¼
#define SUB_GP_QUERY_AUCTION		121									//��ѯ������¼
#define SUB_GP_AUCTIONLOG			122									//��ȡ����Ʒ
#define SUB_GP_TREASURE_RANK		123									//�Ƹ������
#define	SUB_GP_VIP_POWER			124									//vip��Ȩ
#define	SUB_GP_VIP_AWARD			125									//vip����
#define	SUB_GP_GET_SIGNIN_TASK		126									//��ȡǩ���б�
#define	SUB_GP_SIGNIN				127									//ǩ��
//������������---��ȡ��֤��
#define SUB_GP_SET_INSUREPASS_GET_CAPTCHA			128					//��ȡ��֤��
#define SUB_GP_SET_INSUREPASS_SEND_CAPTCHA			129					//������֤��
#define SUB_GP_RECHARGE_ORDER		131									//��ֵ����
#define SUB_GP_VIPSHOP				132									//vip��
#define SUB_GP_GIFT_CHANGE			133									//������һ�


#define SUB_GP_CHECK_ACCOUNT		140									//�˶��˺�
#define SUB_GP_GET_CAPTCHA_BY_ID	141									//���ID��ȡ��֤��
#define SUB_GP_SET_LOGIN_PASS		142									//�����µ�¼����
#define SUB_GP_UN_MOOR_MACHINE		143									//�������

#define SUB_GP_UNREAD_MSG_COUNT			418								//δ������Ϣ
#define SUB_GP_TASK_FINISH_COUNT		419								//��ɵ�����


//�޸�ͷ��
#define SUB_GP_USER_FACE_INFO		200									//ͷ����Ϣ
#define SUB_GP_SYSTEM_FACE_INFO		201									//ϵͳͷ��
#define SUB_GP_CUSTOM_FACE_INFO		202									//�Զ�ͷ��

//��������
#define SUB_GP_USER_INDIVIDUAL		301									//��������
#define	SUB_GP_QUERY_INDIVIDUAL		302									//��ѯ��Ϣ
#define SUB_GP_MODIFY_INDIVIDUAL	303									//�޸�����

//���з���
#define SUB_GP_USER_SAVE_SCORE		400									//������
#define SUB_GP_USER_TAKE_SCORE		401									//ȡ�����
#define SUB_GP_USER_TRANSFER_SCORE	402									//ת�˲���
#define SUB_GP_USER_INSURE_INFO		403									//��������
#define SUB_GP_QUERY_INSURE_INFO	404									//��ѯ����
#define SUB_GP_USER_INSURE_SUCCESS	405									//���гɹ�
#define SUB_GP_USER_INSURE_FAILURE	406									//����ʧ��
#define SUB_GP_QUERY_USER_INFO_REQUEST	407								//��ѯ�û�
#define SUB_GP_QUERY_USER_INFO_RESULT	408								//�û���Ϣ
#define SUB_GP_GET_ALMS             409                                 //��ȡ�ȼý�     
#define SUB_GP_INSURE_RECORD        410                                 //���м�¼   
#define SUB_GP_EXCHANGE_INGOT       411                                 //Ԫ���һ� 
//�Ƹ�
#define SUB_GP_TREASURE					413                             //�Ƹ���ϸ 
#define SUB_GP_CONVERSION_AUCTIONSCORE  414                             //������öһ� 

#define SUB_GP_MORE_GAME			415									//�����Ϸ
#define SUB_GP_FEEDBACK				416									//����

#define SUB_GP_DOWNLOAD_RESURL			417                            //������Դ
//�������
#define SUB_GP_OPERATE_SUCCESS		900									//�����ɹ�
#define SUB_GP_OPERATE_FAILURE		901									//����ʧ��


#define  GIFT_NAME_LEN		16
#define  GIFT_IMGNAME		32
#define  NAME_LEN			16
#define  BUY_PRICE_COUNT	3
#define  AWARD_COUNT		6
#define	 DESPICT_LEN		32
#define  GIFT_COUNT			30
#define  NOTE_LEN			64


//I D ��¼
struct CMD_GP_LogonGameID
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		 //���֤�ţ����֤��ʹ�ã�
	TCHAR							szPhoneVerifyID[LEN_PHONE_VERIFY_ID];//�ֻ���֤�ţ��ֻ��ʹ�ã�
	//��¼��Ϣ
	DWORD							dwGameID;							//��Ϸ I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	BYTE							cbValidateFlags;			        //У���ʶ
};

//�ʺŵ�¼
struct CMD_GP_LogonAccounts
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		 //���֤�ţ����֤��ʹ�ã�
	TCHAR							szPhoneVerifyID[LEN_PHONE_VERIFY_ID];//�ֻ���֤�ţ��ֻ��ʹ�ã�
	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	BYTE							cbValidateFlags;			        //У���ʶ
};

//�����¼
struct CMD_MB_AccessToken
{
	DWORD							dwSessionID;						//�ƹ�ID
	TCHAR							szUMId[LEN_ACCOUNTS * 2];				//�û�Ψһ��ʾ
	DWORD							dwSex;								//�Ա�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szAccessToken[256];					//Token
	DWORD							dwSubSessionID;						//推广ID 不同的登录方式
};

//ע���ʺ�
struct CMD_GP_RegisterAccounts
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szSpreader[LEN_ACCOUNTS];			//�Ƽ��ʺ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����
	BYTE							cbValidateFlags;			        //У���ʶ
};

//��¼�ɹ�
struct CMD_GP_LogonSuccess
{
	//��������
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwGroupID;							//���ű�ʶ
	DWORD							dwCustomID;							//�Զ���ʶ
	DWORD							dwUserMedal;						//�û�����
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����

	//�û��ɼ�
	SCORE                           lIngot;								//Ԫ��
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//�û�����

	//�û���Ϣ
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMoorMachine;						//�����

	BYTE							cbMoorPassPortID;					//�����֤
	BYTE							cbMoorPhone;						//���ֻ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//���֤�ţ����֤��ʹ�ã�
	TCHAR							szPhone[LEN_MOBILE_PHONE];			//�ƶ��绰

	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������

	//������Ϣ
	BYTE                            cbShowServerStatus;                 //��ʾ������״̬
};

//��¼ʧ��
struct CMD_GP_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��½���
struct CMD_GP_LogonFinish
{
	WORD							wIntermitTime;						//�ж�ʱ��
	WORD							wOnLineCountTime;					//����ʱ��
};

//��¼ʧ��
struct CMD_GP_ValidateMBCard
{
	unsigned int								uMBCardID;						//��������
};

//����ʾ
struct CMD_GP_UpdateNotify
{
	BYTE							cbMustUpdate;						//ǿ����
	BYTE							cbAdviceUpdate;						//������
	DWORD							dwCurrentVersion;					//��ǰ�汾
};

//////////////////////////////////////////////////////////////////////////////////
//Я����Ϣ CMD_GP_LogonSuccess

#define DTP_GP_GROUP_INFO			1									//������Ϣ
#define DTP_GP_MEMBER_INFO			2									//��Ա��Ϣ
#define	DTP_GP_UNDER_WRITE			3									//����ǩ��
#define DTP_GP_STATION_URL			4									//��ҳ��Ϣ

//������Ϣ
struct DTP_GP_GroupInfo
{
	DWORD							dwGroupID;							//��������
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������
};

//��Ա��Ϣ
struct DTP_GP_MemberInfo
{
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	//SYSTEMTIME						MemberOverDate;						//����ʱ��
};

//////////////////////////////////////////////////////////////////////////////////
//�б�����

#define MDM_GP_SERVER_LIST			2									//�б���Ϣ

//��ȡ����
#define SUB_GP_GET_LIST				1									//��ȡ�б�
#define SUB_GP_GET_SERVER			2									//��ȡ����
#define SUB_GP_GET_ONLINE			3									//��ȡ����
#define SUB_GP_GET_COLLECTION		4									//��ȡ�ղ�

//�б���Ϣ
#define SUB_GP_LIST_TYPE			100									//�����б�
#define SUB_GP_LIST_KIND			101									//�����б�
#define SUB_GP_LIST_NODE			102									//�ڵ��б�
#define SUB_GP_LIST_PAGE			103									//�����б�
#define SUB_GP_LIST_SERVER			104									//�����б�
#define SUB_GP_VIDEO_OPTION			105									//��Ƶ����

//�����Ϣ
#define SUB_GP_LIST_FINISH			200									//�������
#define SUB_GP_SERVER_FINISH		201									//�������

//������Ϣ
#define SUB_GR_KINE_ONLINE			300									//��������
#define SUB_GR_SERVER_ONLINE		301									//��������

//////////////////////////////////////////////////////////////////////////////////

//��ȡ����
struct CMD_GP_GetOnline
{
	WORD							wServerCount;						//������Ŀ
	WORD							wOnLineServerID[MAX_SERVER];		//�����ʶ
};

//��������
struct CMD_GP_KindOnline
{
	WORD							wKindCount;							//������Ŀ
//	tagOnLineInfoKind				OnLineInfoKind[MAX_KIND];			//��������
};

//��������
struct CMD_GP_ServerOnline
{
	WORD							wServerCount;						//������Ŀ
//	tagOnLineInfoServer				OnLineInfoServer[MAX_SERVER];		//��������
};

//////////////////////////////////////////////////////////////////////////////////
//��������

#define MDM_GP_USER_SERVICE			3									//�û�����

//�˺ŷ���
#define SUB_GP_MODIFY_MACHINE		100									//�޸Ļ���
#define SUB_GP_MODIFY_LOGON_PASS	101									//�޸�����
#define SUB_GP_MODIFY_INSURE_PASS	102									//�޸�����
#define SUB_GP_MODIFY_UNDER_WRITE	103									//�޸�ǩ��
#define SUB_GP_MODIFY_PASSPORT_ID	104									//�����֤
#define SUB_GP_VERIFY_INSURE_PASS	105	                                //��֤���չ�����

//////////////////////////////////////////////////////////////////////////////////

//�޸�����
struct CMD_GP_ModifyLogonPass
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�����
struct CMD_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�ǩ��
struct CMD_GP_ModifyUnderWrite
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��
};

//////////////////////////////////////////////////////////////////////////////////

//�û�ͷ��
struct CMD_GP_UserFaceInfo
{
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwCustomID;							//�Զ���ʶ
};

//�޸�ͷ��
struct CMD_GP_SystemFaceInfo
{
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�ͷ��
struct CMD_GP_CustomFaceInfo
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	DWORD							dwCustomFace[FACE_CX*FACE_CY];		//ͼƬ��Ϣ
};

//////////////////////////////////////////////////////////////////////////////////

//�󶨻���
struct CMD_GP_ModifyMachine
{
	BYTE							cbBind;								//�󶨱�־
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};


//�����֤
struct CMD_GP_ModifyPassPortID
{
	BYTE							cbBind;								//�󶨱�־
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szName[LEN_COMPELLATION];			//��ʵ����		
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//���֤�ţ����֤��ʹ�ã�
};

//////////////////////////////////////////////////////////////////////////////////

//��������
struct CMD_GP_UserIndividual
{
	DWORD							dwUserID;							//�û� I D
};

//��ѯ��Ϣ
struct CMD_GP_QueryIndividual
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
};

//�޸�����
struct CMD_GP_ModifyIndividual
{
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
};

//////////////////////////////////////////////////////////////////////////////////
//Я����Ϣ CMD_GP_UserIndividual

#define DTP_GP_UI_NICKNAME			1									//�û��ǳ�
#define DTP_GP_UI_USER_NOTE			2									//�û�˵��
#define DTP_GP_UI_UNDER_WRITE		3									//����ǩ��
#define DTP_GP_UI_QQ				4									//Q Q ����
#define DTP_GP_UI_EMAIL				5									//�����ʼ�
#define DTP_GP_UI_SEAT_PHONE		6									//�̶��绰
#define DTP_GP_UI_MOBILE_PHONE		7									//�ƶ��绰
#define DTP_GP_UI_COMPELLATION		8									//��ʵ����
#define DTP_GP_UI_DWELLING_PLACE	9									//��ϵ��ַ

//////////////////////////////////////////////////////////////////////////////////

//��������
struct CMD_GP_UserInsureInfo
{
	WORD							wRevenueTake;						//˰�ձ���
	WORD							wRevenueTransfer;					//˰�ձ���
	WORD							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
	SCORE							lTransferPrerequisite;				//ת������
};

//������
struct CMD_GP_UserSaveScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lSaveScore;							//������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ȡ���
struct CMD_GP_UserTakeScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lTakeScore;							//��ȡ���
	TCHAR							szPassword[LEN_MD5];				//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ת�˽��
struct CMD_GP_UserTransferScore
{
	DWORD							dwUserID;							//�û� I D
	BYTE                            cbByNickName;                       //�ǳ�����
	SCORE							lTransferScore;						//ת�˽��
	TCHAR							szPassword[LEN_MD5];				//��������
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//���гɹ�
struct CMD_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
	SCORE							lSwapScore;							//������
	DWORD							dwGameID;
	TCHAR							szNickName[LEN_NICKNAME];
	DWORD							dwTargetGameID;
	TCHAR							szTargetNickName[LEN_NICKNAME];
	DWORD							dwRecordID;
	TCHAR							szDate[32];							//����
	TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct CMD_GP_UserInsureFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��ȡ���
struct CMD_GP_UserTakeResult
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
};

//��ѯ����
struct CMD_GP_QueryInsureInfo
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��������
};

//��ѯ�û�
struct CMD_GP_QueryUserInfoRequest
{
	BYTE                            cbByNickName;                       //�ǳ�����
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
};

//�û���Ϣ
struct CMD_GP_UserTransferUserInfo
{
	DWORD							dwTargetGameID;						//Ŀ���û�
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
};

//�Ƹ�
struct CMD_GP_UserTreasure
{
	CMD_GP_UserTreasure()
	{
		//memset(this, 0, sizeof(CMD_GP_UserTreasure));
	}
	SCORE		lScore;													
	SCORE       lInsureScore;											
	SCORE		lIngotScore;											//Ԫ��
	SCORE		lAuctionScore;											//�������
	SCORE		lLottery;												//����
};
//�������ת��
struct CMD_GP_ConversionAuctionScore
{
	CMD_GP_ConversionAuctionScore()
	{
		//memset(this, 0, sizeof(CMD_GP_ConversionAuctionScore));
	}
	long							lResultCode;						//��������
	SCORE							lInsureScore;						//���н��
	TCHAR							szDescribeString[128];				//������Ϣ
};

//////////////////////////////////////////////////////////////////////////////////


//��ȡ�ȼý�
struct CMD_GP_GetAlms
{
	DWORD							dwUserID;						    //�û�
	TCHAR							szPassword[LEN_MD5];			    //�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ȡ�ȼý�
struct GP_GetAlmsSuccess
{
	SCORE	 lUserInsure;												//���չ���
	TCHAR	 szDescribeString[128];										//�ɹ���Ϣ
};
//��ȡ�ȼý�
struct GP_GetAlmsFailure
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//////////////////////////////////////////////////////////////////////////////////
//Զ�̷���

#define MDM_GP_REMOTE_SERVICE		4									//Զ�̷���

//���ҷ���
#define SUB_GP_C_SEARCH_DATABASE	100									//��ݲ���
#define SUB_GP_C_SEARCH_CORRESPOND	101									//Э������

//���ҷ���
#define SUB_GP_S_SEARCH_DATABASE	200									//��ݲ���
#define SUB_GP_S_SEARCH_CORRESPOND	201									//Э������

//////////////////////////////////////////////////////////////////////////////////

//Э������
struct CMD_GP_C_SearchCorrespond
{
	DWORD							dwGameID;							//��Ϸ��ʶ
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
};

//Э������
struct CMD_GP_S_SearchCorrespond
{
	WORD							wUserCount;							//�û���Ŀ
//	tagUserRemoteInfo				UserRemoteInfo[16];					//�û���Ϣ
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//��¼����
#define MDM_MB_LOGON				100									//�㳡��¼

//��¼ģʽ
#define SUB_MB_LOGON_GAMEID			1									//I D ��¼
#define SUB_MB_LOGON_ACCOUNTS		2									//�ʺŵ�¼
#define SUB_MB_REGISTER_ACCOUNTS	3									//ע���ʺ�
#define SUB_MB_QUICK_LOGIN			4									       //���ٵ�¼(�ο����)
#define SUB_MB_ACCESSTOKEN			5									//�����¼

//��¼���
#define SUB_MB_LOGON_SUCCESS		100									//��¼�ɹ�
#define SUB_MB_LOGON_FAILURE		101									//��¼ʧ��

//����ʾ
#define SUB_MB_UPDATE_NOTIFY		200									//����ʾ

//////////////////////////////////////////////////////////////////////////////////

//I D ��¼
struct CMD_MB_LogonGameID
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE                            cbDeviceType;                       //�豸����

	//��¼��Ϣ
	DWORD							dwGameID;							//��Ϸ I D
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		 //���֤�ţ����֤��ʹ�ã�
	TCHAR							szPhoneVerifyID[LEN_PHONE_VERIFY_ID];//�ֻ���֤�ţ��ֻ��ʹ�ã�
};

//�ʺŵ�¼
struct CMD_MB_LogonAccounts
{
	//ϵͳ��Ϣ
	WORD							wModuleID[10];							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE                            cbDeviceType;                       //�豸����

	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		 //���֤�ţ����֤��ʹ�ã�
	TCHAR							szPhoneVerifyID[LEN_PHONE_VERIFY_ID];//�ֻ���֤�ţ��ֻ��ʹ�ã�
};

//ע���ʺ�
struct CMD_MB_RegisterAccounts
{
	//ϵͳ��Ϣ
	WORD							wModuleID[10];							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE                            cbDeviceType;                       //�豸����

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//��¼�ɹ�
struct CMD_MB_LogonSuccess
{
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbInsurePwd;						//��������
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwExperience;						//������ֵ
	SCORE							dwLoveLiness;						//�û�����
	DWORD                           dwVipLevel;							//��ҵȼ�
	SCORE                           lIngot;								//Ԫ��
	SCORE							lUserScore;							//�û���Ϸ��
	SCORE							lIngotScore;						//Ԫ��
	SCORE							lUserInsure;						//�û�����
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szPhone[LEN_MOBILE_PHONE];			//�绰����
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		 //���֤�ţ����֤��ʹ�ã�

	DWORD							dwTaskCount;						//�������	
	DWORD							dwUnReadCount;						//δ����Ϣ	
	DWORD							dwFirstOnLineOrder;					//�׳�
};

//��¼ʧ��
struct CMD_MB_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//���ٵ�¼
//struct CMD_MB_Quick_Logon
//{
//	DWORD							dwOpTerminal;						//�����նˣ�0������ 1��pc, 2���ֻ�ţţ 3���ֻ��㣩
//	TCHAR							    szMachineID[LEN_MACHINE_ID];		//������ʶ
//};

//���ٵ�¼
struct CMD_MB_Quick_Logon
{
	DWORD							dwOpTerminal;						//�����նˣ�0������ 1��pc, 2���ֻ�ţţ 3���ֻ��㣩
	DWORD							dwSessionID;						//�ƹ�
	TCHAR							    szStatisCode[LEN_MACHINE_ID];		//�ƹ�	
	TCHAR							    szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//���ٵ�¼����
struct CMD_MB_Quick_Logon_Success
{
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szLogonPass[LEN_MD5];				   //��¼����
	LONG							lResultCode;						               //�������
	TCHAR							szDescribeString[128];		               //������Ϣ	
};

//����ʾ
struct CMD_MB_UpdateNotify
{
	BYTE							cbMustUpdate;						//ǿ����
	BYTE							cbAdviceUpdate;						//������
	DWORD							dwCurrentVersion;					//��ǰ�汾
};
//��֤���չ�����
struct CMD_GP_VerifyInsurePass
{
	DWORD	 dwUserID;				
	TCHAR	 szInsurePass[LEN_MD5];			 //���չ�����
	TCHAR	 szMachineID[LEN_MACHINE_ID];	 //��������
};
//////////////////////////////////////////////////////////////////////////////////
//�б�����

#define MDM_MB_SERVER_LIST			101									//�б���Ϣ

//�б���Ϣ
#define SUB_MB_LIST_KIND			100									//�����б�
#define SUB_MB_LIST_SERVER			101									//�����б�
#define SUB_MB_LIST_FINISH			200									//�б����


//�̵�////////////////////////////////////////��Ʒ/////////////////////////////
//ͨ��ʲô;������
struct CMD_GP_Buy_Price
{
	CMD_GP_Buy_Price()
	{
		//memset(this, 0, sizeof(CMD_GP_Buy_Price));
	}
	DWORD	dwCount;
	TCHAR   szName[NAME_LEN];
};
//��Ʒ

struct CMD_GP_GetGift
{
	CMD_GP_GetGift()
	{
		memset(this, 0, sizeof(CMD_GP_GetGift));
	}
	DWORD	dwUserID;
	DWORD	dwOpTerminal;												//�����նˣ�0������ 1��pc, 2���ֻ�ţţ 3���ֻ��㣩
};

struct CMD_GP_Gift
{
	CMD_GP_Gift()
	{
		//memset(this, 0, sizeof(CMD_GP_Gift));
	}
	DWORD				dwID;
	DWORD				dwShowType;
	TCHAR				szName[GIFT_NAME_LEN];
	TCHAR				szImgName[GIFT_IMGNAME];
	CMD_GP_Buy_Price	price[BUY_PRICE_COUNT];
	DWORD				dwDiscount;								//vip�ۿۣ�1-9��
	TCHAR				szOpenDespict[DESPICT_LEN * 2];			//�����Ʒ����
	TCHAR				szDespict[DESPICT_LEN];					//���� 

};
struct CMD_GP_BuyGift
{
	CMD_GP_BuyGift()
	{
		//memset(this, 0, sizeof(CMD_GP_BuyGift));
	}
	DWORD			    dwUserID;								//id
	DWORD				dwOpTerminal;							//�����նˣ�1��pc, 2���ֻ�
	TCHAR				szLogonPass[LEN_MD5];					//��¼����
	DWORD				dwType;									//���1:���2:����
	DWORD				dwID;									//�������id
	DWORD				dwNum;									//����
	DWORD				dwBuyMethod;							//���򷽷���1����ң�2��Ԫ����3�����4����ȯ
	//TCHAR				szNote[NOTE_LEN];						//������Ϣ
	TCHAR				szMachineID[LEN_MACHINE_ID];			//��������
};

struct CMD_GP_BuyGiftLog
{
	CMD_GP_BuyGiftLog()
	{
		//memset(this, 0, sizeof(CMD_GP_BuyGiftLog));
	}
	DWORD							dwRet;								//0���ɹ���1��ʧ��
	TCHAR							szDescribeString[128];				//������Ϣ
};
///////////////////////////////////////��Ʒ///////////////////////////////////////
//��ѯ������Ϣ
struct CMD_GP_UserID
{
	CMD_GP_UserID()
	{
		//memset(this, 0, sizeof(CMD_GP_UserID));
	}
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
};
struct CMD_GP_Knapsack
{
	CMD_GP_Knapsack()
	{
		//memset(this, 0, sizeof(CMD_GP_Knapsack));
	}
	DWORD				dwID;
	DWORD				dwExchangeType;//�һ���ʽ1������ʹ�� 2��ʹ�� 3���һ�(�鿴�һ�����)
	DWORD				dwUseType;								//ʹ������5������  6��Q��
	DWORD				dwPackType;								//���1:���2:����
	DWORD				dwPropID;								//�������id
	DWORD				dwNum;									//����
	DWORD				dwSortID;								//˳��
	TCHAR				szName[GIFT_NAME_LEN];					//���
	TCHAR				szImgName[GIFT_IMGNAME];				//ͼƬ
	TCHAR				szRemark[GIFT_IMGNAME];					//�һ�����
	TCHAR				szOpenDespict[64];
	TCHAR				szDespict[64];
};


struct CMD_GP_KnapsackLog
{
	CMD_GP_KnapsackLog()
	{
		//memset(this, 0, sizeof(CMD_GP_KnapsackLog));
	}
	DWORD							dwRet;								//0���ɹ���1��ʧ��
	TCHAR							szDescribeString[128];				//������Ϣ
};


//ʹ�ñ���
struct CMD_GP_UseKnapsack
{
	CMD_GP_UseKnapsack()
	{
		//memset(this, 0, sizeof(CMD_GP_UseKnapsack));
	}
	DWORD							dwUserID;							//�û� I D
	DWORD							dwOpTerminal;						//�����նˣ�1��pc, 2���ֻ�
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	DWORD							dwID;								//����id
	DWORD							dwNum;								//����
	TCHAR							szNote[NOTE_LEN];					//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};
struct CMD_GP_UseKnapsackLog
{
	CMD_GP_UseKnapsackLog()
	{
		//memset(this, 0, sizeof(CMD_GP_UseKnapsackLog));
	}
	DWORD							dwRet;								//0���ɹ���1��ʧ��
	DWORD							dwID;								//����id
	DWORD							dwNum;								//ʹ�õ�����
	TCHAR							szDescribeString[128];				//������Ϣ
};

//////////////////////////////////////////////////////////////////////////
struct CMD_GP_Auction
{
	CMD_GP_Auction()
	{
		//memset(this, 0, sizeof(CMD_GP_Auction));
	}
	DWORD				dwID;
	DWORD				dwPackType;								//���1:���2:����
	DWORD				dwPropID;								//�������id
	DWORD				dwNum;									//����
	DWORD				dwSortID;								//˳��
	TCHAR				szName[GIFT_NAME_LEN];					//���
	TCHAR				szImgName[GIFT_IMGNAME];				//ͼƬ
};
//��ȡ�ҵ��ϼ���Ʒ
struct  CMD_GP_AuctionLog
{
	CMD_GP_AuctionLog()
	{
		//memset(this, 0, sizeof(CMD_GP_AuctionLog));
	}
	DWORD							dwRet;								//0���ɹ���1��ʧ��
	TCHAR							szDescribeString[128];				//������Ϣ
};
//����Ʒ�ϼ�
struct CMD_GP_Sell_Auction
{
	CMD_GP_Sell_Auction()
	{
		//memset(this, 0, sizeof(CMD_GP_Sell_Auction));
	}
	DWORD			    dwUserID;								//id
	DWORD				dwOpTerminal;							//�����նˣ�1��pc, 2���ֻ�
	TCHAR				szLogonPass[LEN_MD5];					//��¼����
	DWORD				dwID;									//�������id
	SCORE				lGold;									//�۸�
	DWORD				dwNum;									//����
	TCHAR				szMachineID[LEN_MACHINE_ID];			//��������
};
//��������Ʒ
struct CMD_GP_Buy_Auction
{
	CMD_GP_Buy_Auction()
	{
		//memset(this, 0, sizeof(CMD_GP_Buy_Auction));
	}
	DWORD			    dwUserID;								//id
	DWORD				dwOpTerminal;							//�����նˣ�1��pc, 2���ֻ�
	TCHAR				szLogonPass[LEN_MD5];					//��¼����
	DWORD				dwID;									//�������id
	DWORD				dwNum;									//����
	TCHAR				szMachineID[LEN_MACHINE_ID];			//��������
};
//����Ʒ����
struct CMD_GP_Cancel_Auction
{
	CMD_GP_Cancel_Auction()
	{
		//memset(this, 0, sizeof(CMD_GP_Cancel_Auction));
	}
	DWORD			    dwUserID;								//id
	DWORD				dwOpTerminal;							//�����նˣ�1��pc, 2���ֻ�
	TCHAR				szLogonPass[LEN_MD5];					//��¼����
	DWORD				dwID;									//�������id
	TCHAR				szMachineID[LEN_MACHINE_ID];			//��������
};
//����������¼
struct CMD_GP_Query_Auction
{
	CMD_GP_Query_Auction()
	{
		//memset(this, 0, sizeof(CMD_GP_Query_Auction));
	}
	TCHAR								szID[LEN_ACCOUNTS];			    //GameID �� �ǳ�
	DWORD								dwPage;							//��ѯ�ڼ�ҳ
	DWORD								dwPageSize;						//ÿҳ��С��ȱʡΪ10
	DWORD								dwLastDay;						//ʱ��
};
//��ȡ������¼
struct CMD_GP_GetAuctionRecord
{
	DWORD								dwUserID;
	DWORD								dwPage;							//��ѯ�ڼ�ҳ
	DWORD								dwPageSize;						//ÿҳ��С��ȱʡΪ10
	DWORD								dwLastDay;						//ʱ��
};
//������¼
struct CMD_GP_AuctionRecordItem
{
	DWORD								dwIndex;						//����
	DWORD								dwGameID;
	TCHAR								szImgName[GIFT_IMGNAME];	    //ͼƬ
	TCHAR								szNickName[LEN_ACCOUNTS];		//�ǳ�
	TCHAR								szAuctionName[NAME_LEN];		//����Ʒ���
	DWORD								dwPropNum;
	SCORE								lGold;							//�۸�
};
//�ҵ�����
struct CMD_GP_MyAuctionRecordItem
{
	DWORD								dwIndex;						//����
	TCHAR								szAuctionName[NAME_LEN];		//����Ʒ���
	TCHAR								szImgName[GIFT_IMGNAME];		//ͼƬ
	DWORD								dwPropNum;
	SCORE								lGold;							//�۸�
};
//������¼
struct CMD_GP_HistoryAuctionRecordItem
{
	DWORD								dwIndex;						//����
	DWORD								dwGameID;
	TCHAR								szNickName[LEN_ACCOUNTS];		//�ǳ�
	TCHAR								szAuctionName[NAME_LEN];		//����Ʒ���
	TCHAR								szImgName[GIFT_IMGNAME];		//ͼƬ
	DWORD								dwType;							//����1:��2:��
	DWORD								dwPropNum;
	SCORE								lGold;							//�۸�
	SYSTEMTIME							tOpDate;
};
//��¼
template <class T>
struct CMD_GP_AuctionRecord
{
	DWORD								dwPageCount;				//ҳ������
	DWORD								dwPageIndex;					//��ѯ�ڼ�ҳ
	DWORD								dwIndex;							//����
	DWORD								dwAllIndex;						//������
	T											RecordItem[20];
};

struct CMD_GP_Sell_AuctionLog
{
	CMD_GP_Sell_AuctionLog()
	{
		//memset(this, 0, sizeof(CMD_GP_Sell_AuctionLog));
	}
	DWORD							dwRet;								//0���ɹ���1��ʧ��
	DWORD							dwID;								//�������id
	SCORE							lGold;								//�۸�
	DWORD							dwNum;								//����							
	TCHAR							szDescribeString[128];				//������Ϣ
};
struct CMD_GP_Buy_AuctionLog
{
	CMD_GP_Buy_AuctionLog()
	{
		//memset(this, 0, sizeof(CMD_GP_Buy_AuctionLog));
	}
	DWORD							dwRet;								//0���ɹ�����0��ʧ��
	DWORD							dwID;								//�������id
	SCORE							lGold;								//�۸�					
	TCHAR							szDescribeString[128];				//������Ϣ
};
struct CMD_GP_Cancel_AuctionLog
{
	CMD_GP_Cancel_AuctionLog()
	{
		//memset(this, 0, sizeof(CMD_GP_Cancel_AuctionLog));
	}
	DWORD							dwRet;								//0���ɹ���1��ʧ��
	DWORD							dwID;
	DWORD							dwNum;								//����
	DWORD							dwSortID;							//˳��
	TCHAR							szName[GIFT_NAME_LEN];				//���
	TCHAR							szImgName[GIFT_IMGNAME];			//ͼƬ		
	TCHAR							szDescribeString[128];				//������Ϣ
};



//////////////////////////////////////////////////////////////////////////
//vip��Ȩ
struct CMD_GP_VipUserID
{
	CMD_GP_VipUserID()
	{
		//memset(this, 0, sizeof(CMD_GP_VipUserID));
	}
	DWORD	dwUserID;													//�û� I D
	DWORD	dwOpTerminal;												//�����նˣ�1��pc, 2���ֻ�
};
//vip��Ȩ��dwRedPaper�� dwRedPieces ��0�Ǹ����ã�

struct CMD_GP_VipPowerInfo
{
	CMD_GP_VipPowerInfo()
	{
		memset(this, 0, sizeof(CMD_GP_VipPowerInfo));
	}
	DWORD	dwVipID;													//1---7(vip1-vip7)
	SCORE   lLoginScore;												//ÿ���¼�Ľ���
	DWORD	dwShopping;													//����
	DWORD	dwRedPaper;													//���
	DWORD	dwRedPieces;												//�����Ƭ
};


struct CMD_GP_VipPower
{
	CMD_GP_VipPower()
	{
		memset(this, 0, sizeof(CMD_GP_VipPower));
	}
	CMD_GP_VipPowerInfo				VipPowerInfo[7];					//����vip��Ϣ
	DWORD	dwVipID;													//1---7(vip1-vip7)
	DWORD	dwIngot;													//��dwIngot��
	DWORD	dwAllIngot;													//��һ����Ԫ��
	SCORE   lLoginScore;												//ÿ���¼�Ľ���
	DWORD	dwShopping;													//����
	DWORD	dwRedPaper;													//���
	DWORD	dwRedPieces;												//�����Ƭ
	DWORD	dwLoginScoreStatus;											//��¼���� �Ƿ���ȡ   0:��Ч 1��δ��ȡ 2������ȡ
	DWORD	dwRedPaperStatus;											//�������Ƭ �Ƿ���ȡ 0:��Ч 1��δ��ȡ 2������ȡ
};


//vip������ȡ
struct CMD_GP_VipAward
{
	CMD_GP_VipAward()
	{
		//memset(this, 0, sizeof(CMD_GP_VipAward));
	}
	DWORD	dwUserID;													//id
	DWORD	dwOpTerminal;												//�����նˣ�1��pc, 2���ֻ�
	TCHAR	szLogonPass[LEN_MD5];										//��¼����
	DWORD	dwAwardType;												//��������1����¼������ 2�� ��������Ƭ��
};
//vip������ȡ
struct CMD_GP_VipAwardLog
{
	CMD_GP_VipAwardLog()
	{
		//memset(this, 0, sizeof(CMD_GP_VipAwardLog));
	}
	DWORD	dwRet;														//0���ɹ���1��ʧ��
	TCHAR	szDescribeString[128];										//������Ϣ
};
//////////////////////////////////////////////////////////////////////////////////
//����ʧ��
struct CMD_GP_OperateFailure
{
	long							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�����ɹ�
struct CMD_GP_OperateSuccess
{
	long							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//�ɹ���Ϣ
};

//////////////////////////////////////////////////////////////////////////
//����
#define MDM_GL_C_DATA					1								//����--�ͻ������
#define SUB_GL_C_LOGON_ACCOUNTS	 		100								//�ʺŵ�¼
#define SUB_GL_C_MATCH_GAME		 		101								//�������Ϸ
#define SUB_GL_C_MATCH_TYPE		 		102								//�������ͣ�1ÿ��ѭ����2ÿ�춨ʱ��3��ʱ��4���˿���
#define SUB_GL_C_MATCH_INFO		 		103								//�����б?��ӣ����£��Ƿ���
#define SUB_GL_C_MATCH_DELETE	 		104								//�����б�ɾ��
#define SUB_GL_C_MATCH_NUM				105								//��������
#define SUB_GL_C_MATCH_SIGNUP			106								//������
#define SUB_GL_C_MATCH_START			107								//����ʼ�����ˣ�
#define SUB_GL_C_MATCH_RANK				108								//�ھ�
#define SUB_GL_C_MATCH_WITHDRAWAL		109							//��������
#define SUB_GL_C_MATCH_COUNTDOWN		110								//����ʱ

#define  SUB_C_MATCH_START_NEXT     6                 //�ٴ���ս
#define  SUB_GL_C_UPDATE_NOTIFY       111

#define SUB_GL_SIGNUP_SUCCESS			155								//����ɹ�
#define SUB_GL_SIGNUP_FAILURE			156								//����ʧ��
//������
#define SUB_GL_WITHDRAW_SUCCESS			157								//����ɹ�
#define SUB_GL_WITHDRAW_FAILURE			158								//����ʧ��

#define	SUB_GL_C_TASK_LOAD				114								//��������
#define	SUB_GL_C_TASK_REWARD			115								//��ȡ����
#define SUB_GL_C_SYSTEM_MESSAGE		    112								//������Ϣ

//��������
#define SMT_CLOSE_ROOM				0x0100								//�رշ���
#define SMT_CLOSE_GAME				0x0200								//�ر���Ϸ
#define SMT_CLOSE_LINK				0x0400								//�ж�����
#define SMT_CLOSE_HALL				0x0800	

#define SUB_GL_MB_LOGON_ACCOUNTS	 	116								//�ֻ��¼
#define SUB_GL_LOGON_SUCCESS            150

#define  TASK_TITLE_LEN			32
#define	 TASK_AWARD				8
#define  TASK_INFO_COUNT		20
#define	 IMGNAME				32
#define  PROC_NAME				64
#define  LEN_AWARD				32


//ϵͳ��Ϣ
struct CMD_GL_SystemMessage
{
	WORD							wType;								//��Ϣ����
	WORD							wLength;							//��Ϣ����
	TCHAR							szString[1024];						//��Ϣ����
};


//�����¼
struct CMD_GL_LogonAccounts
{
	//ϵͳ��Ϣ
	//DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	//TCHAR							szPassPortID[LEN_PASS_PORT_ID];		 //���֤�ţ����֤��ʹ�ã�
	//TCHAR							szPhoneVerifyID[LEN_PHONE_VERIFY_ID];//�ֻ���֤�ţ��ֻ��ʹ�ã�
	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	//BYTE							cbValidateFlags;			        //У���ʶ
};

struct CMD_GL_GetTask
{
	CMD_GL_GetTask()
	{
		//memset(this, 0, sizeof(CMD_GL_GetTask));
	}
	DWORD				dwOpTerminal;							//�����նˣ�1��pc, 2���ֻ�
};
//���������3��
struct CMD_GL_TaskAward
{
	CMD_GL_TaskAward()
	{
		//memset(this, 0, sizeof(CMD_GL_TaskAward));
	}
	DWORD           dwAward1;
	TCHAR			szAward1[LEN_AWARD];
	DWORD           dwAward2;
	TCHAR			szAward2[LEN_AWARD];
	DWORD           dwAward3;
	TCHAR			szAward3[LEN_AWARD];
};
//����
struct CMD_GL_TaskInfo
{
	CMD_GL_TaskInfo()
	{
		//memset(this, 0, sizeof(CMD_GL_TaskInfo));
	}
	DWORD				dwTaskID;
	TCHAR				szTitle[TASK_TITLE_LEN];
	TCHAR				szImgName[IMGNAME];				//ͼƬ
	DWORD				dwSortID;
	CMD_GL_TaskAward	Award;
	TCHAR				szRemarks[TASK_TITLE_LEN];
	DWORD				dwProgress;
	DWORD				dwCurProgress;
};

//����, ����
struct CMD_GL_TaskID
{
	CMD_GL_TaskID()
	{
		//memset(this, 0, sizeof(CMD_GL_TaskID));
	}
	DWORD				dwTaskID;
	DWORD				dwUserID;
	TCHAR				szPassword[LEN_MD5];				//��¼����
};

//�����
struct CMD_GL_TaskIDLog
{
	CMD_GL_TaskIDLog()
	{
		//memset(this, 0, sizeof(CMD_GL_TaskIDLog));
	}
	long							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//�ɹ���Ϣ
};

//����/���������
struct CMD_GP_LobbyIp
{
	DWORD							dwUserID;							//�û� I D
	SCORE                           lIngot;							    //Ԫ��
	SCORE							lUserScore;							//�û����
	TCHAR							szServerIP[LEN_ADDR];
	DWORD							dwServerPort; 
};
//////////////////////////////////////////////////////////////////////////��Ϣ
#define SUB_GL_C_MESSAGE				113								//��Ϣ
#define MSG_MESSAGE_LEN		516
#define MSG_TITLE			32
#define DATE_LENGTH			64

enum MsgPositionType
{
	Position_Top = 1,		//����
	Position_Under,			//����
	Position_Right,			//���Ͻ�
};
enum MsgType
{
	Msg_Delta = 1,			//��ֵ			��ֻ��һ�Σ����ߣ�
	Msg_Rewards,			//������		��ֻ��һ�Σ����ߣ�
	Msg_Sell,				//����			��ֻ��һ�Σ����ߣ�
	Msg_Rolling,			//������Ϣ		 (����һ�Σ�ʵʱ��ʾ��
	Msg_Notice,				//����			����¼�����ߣ������ͣ�
	Msg_Building,			//��¥			 ֻ��һ�Σ����ߣ�
	Msg_Sell_Success,		//�����ɹ�
	Msg_Vip,				//vip			��ֻ��һ�Σ�����
	Msg_9,
	Msg_10,
	Msg_11,
	Msg_12,

};

struct CMD_GL_MsgNode
{
	CMD_GL_MsgNode()
	{
		memset(this, 0, sizeof(CMD_GL_MsgNode));
	}
	DWORD				dwMsgID;
	DWORD				dwUserID;
	MsgPositionType		dwPositionType;
	MsgType				dwMsgType;
	DWORD				dwIEType;
	DWORD				dwMsgVlaue1;
	DWORD				dwMsgVlaue2;
	DWORD				dwMsgVlaue3;
	DWORD				dwIsNotify;
	TCHAR				szTitle[MSG_TITLE];
	TCHAR				szMsgcontent[MSG_MESSAGE_LEN];
};

//////////////////////////////////////////////////////////////////////////
//��ȡǩ���б�
struct CMD_GP_GetSignInTask
{
	CMD_GP_GetSignInTask()
	{
		memset(this, 0, sizeof(CMD_GP_GetSignInTask));
	}
	DWORD	dwUserID;													//�û� I D
	DWORD	dwOpTerminal;												//�����նˣ�1��pc, 2���ֻ�ţţ 3���ֻ��㣩
};

//ÿ��ǩ���б�
struct CMD_GP_SignInTask
{
	CMD_GP_SignInTask()
	{
		memset(this, 0, sizeof(CMD_GP_SignInTask));
	}
	DWORD	dwDay;														//����    1��ÿ��  >1 �ۼ�   >27:����
	SCORE	lScore;														//������
};
//��ȡǩ���б�
struct CMD_GP_GetSignInTaskInfo
{
	CMD_GP_GetSignInTaskInfo()
	{
		memset(this, 0, sizeof(CMD_GP_GetSignInTaskInfo));
	}
	DWORD	dwDay;												//�ۼƼ���
	CMD_GP_SignInTask   SignInTask[7];							//�ۼƽ������6��,����һ����ÿ��
};

//ǩ��
struct CMD_GP_SignIn
{
	CMD_GP_SignIn()
	{
		memset(this, 0, sizeof(CMD_GP_SignIn));
	}
	DWORD	dwUserID;													//�û� I D
	DWORD	dwOpTerminal;												//�����նˣ�1��pc, 2���ֻ�ţţ 3���ֻ��㣩
	TCHAR	szLogonPass[LEN_MD5];										//��¼����
};

//ÿ��ǩ������
struct CMD_GP_SignInTaskLog
{
	CMD_GP_SignInTaskLog()
	{
		memset(this, 0, sizeof(CMD_GP_SignInTaskLog));
	}
	DWORD	dwRet;														//0���ɹ���1��ʧ��
	SCORE	lScore;														//ÿ�콱����
	DWORD	dwDay;														//����    1��ÿ��  >1 �ۼ�   >27:����
	SCORE	lAllScore;													//�ۼƽ��
	TCHAR	szDescribeString[128];										//������Ϣ
};

//���ֻ����

//��ȡ��֤��
struct CMD_GP_Get_Captcha
{
	CMD_GP_Get_Captcha()
	{
		memset(this, 0, sizeof(CMD_GP_Get_Captcha));
	}
	DWORD							dwUserID;							//�û� I D
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szPhone[LEN_MOBILE_PHONE];			//�ƶ��绰
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};


//������֤��
struct CMD_GP_Send_Captcha
{
	CMD_GP_Send_Captcha()
	{
		memset(this, 0, sizeof(CMD_GP_Send_Captcha));
	}
	DWORD							dwUserID;							//�û� I D
	DWORD							dwCaptcha;							//��֤��
};

//��֤��ʧ��
struct CMD_GP_CaptchaRet
{
	CMD_GP_CaptchaRet()
	{
		memset(this, 0, sizeof(CMD_GP_CaptchaRet));
	}
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//////////////////////////////////////////////////////////////////////////
//�һ��������
struct CMD_GP_Accounts
{
	CMD_GP_Accounts()
	{
		memset(this, 0, sizeof(CMD_GP_Accounts));
	}
	TCHAR							szAccounts[LEN_ACCOUNTS];			//�ʺ�
};

struct CMD_GP_AccountsRet
{
	CMD_GP_AccountsRet()
	{
		memset(this, 0, sizeof(CMD_GP_AccountsRet));
	}
	LONG							lResultCode;						//�������
	DWORD							dwUserID;
	TCHAR							szPhone[LEN_MOBILE_PHONE];			//�ƶ��绰
	TCHAR							szDescribeString[128];				//������Ϣ
};

struct CMD_GP_GetCaptchaByUserID
{
	CMD_GP_GetCaptchaByUserID()
	{
		memset(this, 0, sizeof(CMD_GP_GetCaptchaByUserID));
	}
	DWORD							dwUserID;
};


struct CMD_GP_SetPass
{
	CMD_GP_SetPass()
	{
		memset(this, 0, sizeof(CMD_GP_SetPass));
	}
	DWORD							dwUserID;							//�û� I D
	DWORD							dwCaptcha;							//��֤��
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
};
//�������뷵��
struct CMD_GP_SetPassRet
{
	CMD_GP_SetPassRet()
	{
		memset(this, 0, sizeof(CMD_GP_SetPassRet));
	}
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};


//���������������
//������������---��ȡ��֤��
struct CMD_GP_SetInsurePassGetCaptcha
{
	CMD_GP_SetInsurePassGetCaptcha()
	{
		memset(this, 0, sizeof(CMD_GP_SetInsurePassGetCaptcha));
	}
	DWORD							dwUserID;							//�û� I D
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};



//�������
struct CMD_GP_UnMoorMachine
{
	CMD_GP_UnMoorMachine()
	{
		memset(this, 0, sizeof(CMD_GP_UnMoorMachine));
	}
	DWORD							dwUserID;							//�û� I D
	DWORD							dwCaptcha;							//��֤��
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};
//��������
struct CMD_GP_UnMoorMachineRet
{
	CMD_GP_UnMoorMachineRet()
	{
		memset(this, 0, sizeof(CMD_GP_UnMoorMachineRet));
	}
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};


//////////////////////////////////////////////////////////////////////////
//Ԫ���һ�
struct CMD_GP_UserExchangeIngot
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwIngot;							//Ԫ��
	TCHAR							szPassword[LEN_MD5];				//���չ�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//Ԫ���һ��ɹ�
struct CMD_GP_ExchangeIngotSuccess
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lInsure;							//����
	SCORE							lIngot;								//Ԫ��
	TCHAR							szDescribeString[128];				//������Ϣ
};


#define MORENAME_NAME_LEN				32
#define MORENAME_ICO_LEN				32
#define MORENAME_URL_LEN				128

//��ȡ�����Ϸ
struct CMD_GP_GetMoreGame
{
	CMD_GP_GetMoreGame()
	{
		//memset(this, 0, sizeof(CMD_GP_GetMoreGame));
	}
	DWORD	dwOpTerminal;												//�����նˣ�1��pc, 2���ֻ�ţţ 3���ֻ��㣩
	WORD	wPlatformType;												//ƽ̨���ͣ�1���߲�ţƽ̨��
};

struct CMD_GP_MoreGame
{
	CMD_GP_MoreGame()
	{
		//memset(this, 0, sizeof(CMD_GP_MoreGame));
	}
	TCHAR	szPackageName[MORENAME_NAME_LEN];							//��Ϸ����
	TCHAR	szActivityName[MORENAME_NAME_LEN];							   //������
	TCHAR	szICO[MORENAME_ICO_LEN];									           //ͼƬ���ص�ַ
	TCHAR	szUrlAndroid[MORENAME_URL_LEN];								       //android���ص�ַ
	TCHAR	szUrlIos[MORENAME_URL_LEN];									           //ios���ص�ַ

	TCHAR	szGameName[MORENAME_NAME_LEN];								   //Ӧ����
	TCHAR	szDescribeString[MORENAME_URL_LEN];							   //������Ϣ
};

//����
struct CMD_GP_Feedback
{
	CMD_GP_Feedback()
	{
		memset(this, 0, sizeof(CMD_GP_Feedback));
	}
	TCHAR	szAccounts[LEN_ACCOUNTS];
	TCHAR	szTitle[32];
	TCHAR	szQQ[16];
	TCHAR	szContent[256];										//������Ϣ
};

struct CMD_GP_FeedbackLog
{
	CMD_GP_FeedbackLog()
	{
		memset(this, 0, sizeof(CMD_GP_FeedbackLog));
	}
	DWORD	dwRet;														//0���ɹ���1��ʧ��
	TCHAR	szDescribeString[128];										//������Ϣ
};

//��Դ�������
struct CMD_GP_GetDownloadResUrl
{
	CMD_GP_GetDownloadResUrl()
	{
		memset(this, 0, sizeof(CMD_GP_GetDownloadResUrl));
	}
	DWORD dwKindID;
};

struct CMD_GP_DownloadResUrl
{
	CMD_GP_DownloadResUrl()
	{
		memset(this, 0, sizeof(CMD_GP_DownloadResUrl));
	}
	DWORD dwKindID;
	DWORD dwVersion;
	DWORD  dwPakageSize;
	char  szResUrl[128];
	char  szMd5[33];
	char  szGameName[32];
};

//red notice
//δ����Ϣ����ɵ�����
struct  CMD_GP_UserIDNotify
{
	CMD_GP_UserIDNotify()
	{
		memset(this, 0, sizeof(CMD_GP_UserIDNotify));
	}
	DWORD dwCount;
};

//��ֵ�¶���
struct CMD_GP_RechargeOrder
{
	CMD_GP_RechargeOrder()
	{
		memset(this, 0, sizeof(CMD_GP_RechargeOrder));
	}
	TCHAR	szAccounts[LEN_ACCOUNTS];									//�ʺ�
	DWORD	dwOpTerminal;												//�����նˣ�1��pc, 2���ֻ�ţţ 3���ֻ��㣩
	DWORD	dwFirst;													//�״γ�ֵ
	DWORD	dwOrderAmount;												//���
	DWORD	dwShareID;													//�������� 2������ 7��֧����
	TCHAR	szLogonPass[LEN_MD5];										//��¼����
	TCHAR	szRechargeOrder[33];										//����
};

struct CMD_GP_RechargeOrderLog
{
	CMD_GP_RechargeOrderLog()
	{
		memset(this, 0, sizeof(CMD_GP_RechargeOrderLog));
	}
	DWORD	dwRet;														//0���ɹ���1��ʧ��
	TCHAR	szDescribeString[128];										//������Ϣ
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
//����������

//����״̬
enum MatchStatus
{
	Free = 1,				
	WaitFor,
	Start,
	Finish
};


struct tagMatchSerial  //����Ψһ��
{
	int operator==(const tagMatchSerial& pOther) const
	{
		return ((this->dwMatchInfoID==pOther.dwMatchInfoID) && (this->dwFullPlayerNum==pOther.dwFullPlayerNum)
			&(this->dwKindID==pOther.dwKindID) && (this->dwMatchType==pOther.dwMatchType));
	}
	tagMatchSerial()
	{
		memset(this, 0, sizeof(tagMatchSerial));
	}
	SCORE			dwMatchInfoID;   //�����˿���Ϊ0
	DWORD			dwKindID;
	DWORD			dwMatchType;             //4(���˿���)
	DWORD			dwFullPlayerNum;		//�����˿����������ã�6�ˣ�36�ˣ�
};

struct CMD_GL_MatchGame
{
	CMD_GL_MatchGame()
	{
		memset(this, 0, sizeof(CMD_GL_MatchGame));
	}
	DWORD			dwKindID;
	DWORD           dwSortID;
	DWORD           dwClientVersion;
	TCHAR			szClientEXEName[LEN_PROCESS];		//�������
	TCHAR			szGameName[LEN_KIND];
};

//����ɹ�
struct CMD_GL_SignUpSuccess
{
	CMD_GL_SignUpSuccess()
	{
		memset(this, 0, sizeof(CMD_GL_SignUpSuccess));
	}
	tagMatchSerial  MatchSerial;
	//SCORE			lMatchID;											//�����˿���Ϊ0
	//WORD			wKindID;											
	//WORD			wType;												//��������
	//WORD			wNumber;											//�����˿����������ã�6�ˣ�36�ˣ�
	SCORE           dwSignUpScore;
	DWORD			dwSignUpTicket;
	LONG			lResultCode;										//��������
	TCHAR			szDescribeString[128];								//�ɹ���Ϣ
};

//����ʧ��
struct CMD_GL_SignUpFailure
{
	CMD_GL_SignUpFailure()
	{
		memset(this, 0, sizeof(CMD_GL_SignUpFailure));
	}
	tagMatchSerial  MatchSerial;
	//SCORE			lMatchID;											//�����˿���
	//WORD			wKindID;											
	//WORD			wType;												//��������
	//WORD			wNumber;											//�����˿����������ã�6�ˣ�36�ˣ�
	LONG			lResultCode;										//��������
	TCHAR			szDescribeString[128];								//������Ϣ
};
//������ַ
struct CMD_GL_MatchServer
{
	CMD_GL_MatchServer()
	{
		memset(this, 0, sizeof(CMD_GL_MatchServer));
	}
	tagMatchSerial  MatchSerial;
	DWORD			dwServerID;
	DWORD			dwUserID;							//�û� I D
	SCORE           lIngot;								//Ԫ��
	SCORE			lUserScore;							//�û����
	TCHAR			szServerIP[LEN_ADDR];
	DWORD			dwServerPort; 
};
//��������
struct CMD_GL_MatchNum
{
	CMD_GL_MatchNum()
	{
		memset(this, 0, sizeof(CMD_GL_MatchNum));
	}
	tagMatchSerial  MatchSerial;
	//SCORE			lMatchID;											//�����˿���Ϊ0
	//WORD			wKindID;											
	//WORD			wType;												//��������
	//WORD			wNumber;											//�����˿����������ã�6�ˣ�36�ˣ�
	int				nSignUpNum;											//��������(-1����)
	DWORD			dwCurGroupCount;									//����ʱ����������������
};
////////////////////////////////////////////////////////////////////////////////////
struct CMD_GL_MatchType
{
	CMD_GL_MatchType()
	{
		memset(this, 0, sizeof(CMD_GL_MatchType));
	}
	DWORD		    dwType; //1ÿ��ѭ����2ÿ�춨ʱ��3��ʱ��4���˿���,5�����
	DWORD			dwSortID;
	TCHAR			szMatchType[32];
};
struct CMD_GL_MatchStatus
{
	CMD_GL_MatchStatus()
	{
		memset(this, 0, sizeof(CMD_GL_MatchStatus));
	}
	MatchStatus status;

	DWORD		dwMatchNum;
	DWORD		dwUserCount;
	DWORD		dwAndroidCount;
};

struct CMD_GL_MatchAward
{
	CMD_GL_MatchAward()
	{
		memset(this, 0, sizeof(CMD_GL_MatchAward));
	}
	DWORD		    dwMatchInfoID;   
	DWORD			dwBeginRank;
	DWORD			dwEndRank;
	DWORD           dwAward1;
	TCHAR			szAward1[LEN_AWARD];
	DWORD           dwAward2;
	DWORD           dwAwardID2;
	TCHAR			szAward2[LEN_AWARD];
	DWORD           dwAward3;
	DWORD           dwAwardID3;
	TCHAR			szAward3[LEN_AWARD];
};

enum SignUpStatus
{
	SignUp = 1,		//�ѱ��� 
	NoSignUp,		//û�б���
	NoMatchID,		//û�б����
	NotAllowSignUp  //���ⱨ��
};
struct CMD_GL_MatchInfo
{
	CMD_GL_MatchInfo()
	{
		memset(this, 0, sizeof(CMD_GL_MatchInfo));
	}
	tagMatchSerial  MatchSerial;
	DWORD           dwServerID;
	SYSTEMTIME	    tMatchTime;
	DWORD			dwSpaceTime;
	SCORE           dwSignUpScore;
	DWORD			dwSignUpTicket;
	//DWORD			dwCurPlayerNum;
	//DWORD			dwAllPlayerNum;	
	DWORD			dwCurGroupCount;									//����ʱ����������������
	DWORD			dwSignUpPlayerNum;
	CMD_GL_MatchAward  MatchAward[LEN_MATCHITEMAWARD];
	SignUpStatus    dwSignUp;							//������
	TCHAR			szRemark[LEN_REMARK];
};

struct CMD_GL_MatchChampion
{
	CMD_GL_MatchChampion()
	{
		memset(this, 0, sizeof(CMD_GL_MatchChampion));
	}
	SYSTEMTIME	   Time;
	TCHAR		   szTitle[MAX_CHAMPION_TEXT];
	TCHAR		   szUserNick[LEN_NICKNAME];
	TCHAR		   szChampionText[MAX_CHAMPION_TEXT];
};

struct CMD_GL_MatchConfig
{
	CMD_GL_MatchConfig()
	{
		memset(this, 0, sizeof(CMD_GL_MatchConfig));
	}
	SCORE			dwMatchID;
	SYSTEMTIME		tBeginTime;
	DWORD			dwAllPlayerNum;
	CMD_GL_MatchType   MatchType;
	CMD_GL_MatchAward  MatchAward[LEN_MATCHITEMAWARD];
};
struct CMD_GL_Match
{
	CMD_GL_Match()
	{
		memset(this, 0, sizeof(CMD_GL_Match));
	}
	SCORE				dwMatchID;
	DWORD				dwPlayerNum;
	SYSTEMTIME			tBeginTime;
	SYSTEMTIME			tEndTime;
};

//�û�����
struct tagMatchID
{
	tagMatchID()
	{
		memset(this, 0, sizeof(tagMatchID));
	}
	DWORD			dwClientVersion;									//��Ϸ�汾
	tagMatchSerial  MatchSerial;  
};

//����ʱ
struct CMD_GL_MatchCountDown
{
	CMD_GL_MatchCountDown()
	{
		memset(this, 0, sizeof(CMD_GL_MatchCountDown));
	}
	tagMatchSerial MatchSerial;
	DWORD	  dwCountDown;													//�������
};


struct CMD_FishMatch_Screen
{
	BYTE                            IsTimeMatch;
	BYTE                            dwMatchStatus;
	LONGLONG                   lTimerSpan;              //����ʱ��
};

struct CMD_FishMatch_Info
{


	TCHAR							  szTitle[128];						//��Ϣ����
	DWORD                          dwMeRank;
	DWORD                          dwHaveuser;
	BYTE                              lPlaycount;  // ���
	BYTE                              lAllPlayCount;
	SCORE                           lmeCurScore; // �ҵĵ�ǰ��߷�
	SCORE                           lmeMaxScore; // �ҵ���ʷ��߷�



	SCORE                           lMaxScore;  //ȫ����߷�
	BYTE                             cbMatchStatus;
	LONGLONG                    lTimerSpan;              //����ʱ��
};
//������Ϣ
struct CMD_GR_Match_Info
{
	BYTE                              cbMtachType;                         //��������
	TCHAR							   szMatchName[128];					//��Ϣ����
	TCHAR							   szTitle[128];						//��Ϣ����
	DWORD						   dwAllPlayerCount;					//��������
	WORD							   wJishu;							    //��Ϸ����
	SCORE    						   lOutScore;							//���ַ���

	DWORD                           dwGameRound;                         //����׶�

	DWORD                           dwAllGameRoundCount;                 //������ 
	DWORD                           dwAllGameCount;                      //�����  

	DWORD                           dwGameRoundCount;                    //����ڼ��� 
	DWORD                           dwGameCount;                           //����ڼ���  


	/////�����Ǵ�����

	DWORD							dwCurrentCount;					    //��ǰ����
	DWORD							dwAllCount;							//������
	SCORE                             lTopScore;                           //�����
};
//������Ϣ-����
struct CMD_GR_Match_Info_Jishu
{
	WORD							wJishu;							    //��Ϸ����
};
//������Ա��Ϣ
struct CMD_MatchuserDB
{
	DWORD  dwUserID;
	BYTE   cbMStatus;
	LONGLONG lMatchNO;
};

//��ʾ��Ϣ
struct CMD_GR_Match_Wait_Tip
{

	TCHAR							szMatchName[128];			//�������
	TCHAR							szMatchRoundName[128];			//�������
	SCORE							lScore;								//��ǰ���
	DWORD                        dwJinjirenshu;
	DWORD                        dwBisairenshu;
	WORD							wPlayingTable;						//��Ϸ����

	TCHAR							szMatchRoundMiaoshu[128];     //�������
	WORD							wRank;								        //��ǰ���
	WORD							wCurTableRank;						    //�������
	WORD							wUserCount;							    //��ǰ����
};

//������
struct CMD_GR_MatchResult
{
	BYTE                            cbResultType;                         //������� 1Ϊ�� 0Ϊ�޽�
	TCHAR							szName[64];					        //����-�װ���xxx
	TCHAR							szDescribe[256];					//�ý�����-��ϲ����X��
	TCHAR							szDescribe2[128];					//�ý�����-�ش˱���
	TCHAR							szDescribe3[128];					//�ý�����-�����
	SCORE							dwGold;								//��ҽ���
	DWORD                           dwRankLast;
	DWORD							dwMedal;							//���ƽ���
	DWORD							dwCansaijuan;						//����?��
	TCHAR			                    szAward1[32];
	TCHAR			                    szAward2[32];
	TCHAR			                    szAward3[32];
};
#define MAX_MATCH_DESC				4									//�������
//��������
struct CMD_GR_MatchDesc
{
	TCHAR							szMatchName[32];		//�������
	WORD                            dwTimes;                  //ƽ��ʱ��
	SCORE                           No1Score;               //�ھ���
	SCORE                           No2Score;               //�ھ���
	SCORE                           No3Score;               //�ھ���
	TCHAR							szNorDescribe[128];		//�������ݳ���
	TCHAR							szSysDescribe[128];		//��������С��ʿ
	DWORD						crTitleColor;			//������ɫ
	DWORD						crNorDescribe;			//������ɫ
	DWORD						crSysDescribe;			//������ɫ
	LONGLONG                       lTimerSpan;              //����ʣ��ʱ��
	BYTE                                cbMatchType;               //������
	TCHAR			                    szAward1[32];
	TCHAR			                    szAward2[32];
	TCHAR			                    szAward3[32];
};

//��Ϸ�ͻ�����ʾ�ȴ�
struct CMD_GR_Match_WAITSTART
{
	BYTE                               cbMatchStatus;                      //����״̬
	DWORD							dwWaitting;							//�ȴ�����
	DWORD							dwTotal;							//��������
	DWORD                            dwMeUserID;
	CMD_GR_MatchDesc      d_MatchDesc;
	BYTE                               cbNeedSortUserList;


};
struct MatchUserDB
{
	BYTE                               cbIndex; //��������
	DWORD                           dwUserID;
	DWORD                           dwRank;
	LONGLONG                      lScore;
	BYTE                               cbMatchStatus;
	TCHAR                            szName[128];
};

//��Ʒ��һ�
struct CMD_GP_GiftChange
{
	CMD_GP_GiftChange()
	{
		memset(this, 0, sizeof(CMD_GP_GiftChange));
	}
	TCHAR							szAccounts[LEN_ACCOUNTS];			//�ʺ�
	TCHAR							szGiftChangeOrder[32];				//��Ʒ��
	DWORD							dwOpTerminal;						//�����նˣ�1��pc, 2���ֻ�ţţ 3���ֻ��㣩
};

//��Ʒ��һ�����
struct CMD_GP_GiftChangeRet
{
	CMD_GP_GiftChangeRet()
	{
		memset(this, 0, sizeof(CMD_GP_GiftChangeRet));
	}
	DWORD   dwRet;														//0���ɹ���1��ʧ��
	TCHAR	szDescribeString[128];										//������Ϣ
};

#define SUB_MB_ACCESSTOKEN			5									//�����¼

#define DTP_GP_GET_LABA_COUNT		8	                                //���ȸ���
#define DTP_GP_GET_DIAL_COUNT		9									//ת�̸���


//�������
struct tagDataDescribe
{
	WORD							wDataSize;						//��ݴ�С
	WORD							wDataDescribe;					//�������
};

//������
struct DTP_GP_GetLabaCount
{
	DWORD							 dwLabaCount;			//������
};
//����ת�̴���
struct DTP_GP_DialCount
{
	DTP_GP_DialCount()
	{
		//memset(this, 0, sizeof(DTP_GP_DialCount));
	}
	DWORD		dwDialCount;
};

#pragma pack()

#endif
