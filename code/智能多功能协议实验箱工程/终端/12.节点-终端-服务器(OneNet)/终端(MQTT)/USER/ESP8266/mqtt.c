#include "mqtt.h"



unsigned char  MQTT_RxDataBuf[R_NUM][RBUFF_UNIT];  //���ݵĽ��ջ�����,���з��������������ݣ�����ڸû�����,��������һ���ֽڴ�����ݳ���
unsigned char *MQTT_RxDataInPtr;                   //ָ����ջ�����������ݵ�λ��
unsigned char *MQTT_RxDataOutPtr;                  //ָ����ջ�������ȡ���ݵ�λ��
unsigned char *MQTT_RxDataEndPtr;                  //ָ����ջ�����������λ��

unsigned char  MQTT_CMDBuf[C_NUM][CBUFF_UNIT];     //�������ݵĽ��ջ�����
unsigned char *MQTT_CMDInPtr;                      //ָ���������������ݵ�λ��
unsigned char *MQTT_CMDOutPtr;                     //ָ�����������ȡ���ݵ�λ��
unsigned char *MQTT_CMDEndPtr;                     //ָ���������������λ��




char ClientID[128]; //��ſͻ��� ID �Ļ�����
int ClientID_len; //��ſͻ��� ID �ĳ���
char Username[128]; //����û����Ļ�����
int Username_len; //����û����ĳ���
char Passward[128]; //�������Ļ�����
int Passward_len; //�������ĳ���
char ServerIP[128]; //��ŷ����� IP ��������
int ServerPort; //��ŷ������Ķ˿ں�
int Fixed_len; //�̶���ͷ����
int Variable_len; //�ɱ䱨ͷ����
int Payload_len; //��Ч���ɳ���
char Ping_flag; //ping ����״̬ 0������״̬���ȴ���ʱʱ�䵽������Ping ����

 //ping ����״̬ 1��Ping �����ѷ��ͣ����յ� �������ظ����ĵĺ� �� 1 ��Ϊ 0
char Connect_flag; //ͬ����������״̬ 0����û�����ӷ����� 1�������Ϸ�������
char ConnectPack_flag; //CONNECT ����״̬ 1��CONNECT ���ĳɹ�
char SubcribePack_flag; //���ı���״̬ 1�����ı��ĳɹ�

uint8_t data_buf[256]={0};
/*----------------------------------------------------------*/
/*�������������Ƴ�ʼ���������õ��ͻ��� ID���û��������� */
/*�� ������ */
/*����ֵ���� */
/*----------------------------------------------------------*/
void AliIoT_Parameter_Init(void)
{
	char temp[128]; //������ܵ�ʱ����ʱʹ�õĻ�����
	memset(ClientID,0,128); //�ͻ��� ID �Ļ�����ȫ������
	sprintf(ClientID,"%s",CLIENTID); 
	//�����ͻ��� ID�������뻺����
	ClientID_len = strlen(ClientID); //����ͻ��� ID �ĳ���
	memset(Username,0,128); //�û����Ļ�����ȫ������
	sprintf(Username,"%s",USERNAME); 
	//�����û����������뻺����
	Username_len = strlen(Username); //�����û����ĳ���
	memset(Passward,0,strlen(Passward));
	//�������
	sprintf(Passward,"%s",PASSWARD);
	
	Passward_len = strlen(Passward); //�����û����ĳ���
	memset(ServerIP,0,128); 
	sprintf(ServerIP,"%s",IP);  //��������������
	ServerPort = PORT; //�������˿ں� 1883
	
	printf("�� �� ����%s:%d\r\n",ServerIP,ServerPort); //�������������Ϣ
	printf("�ͻ��� ID��%s\r\n",ClientID); //�������������Ϣ
	printf("�� �� ����%s\r\n",Username); //�������������Ϣ
	printf("�� �룺%s\r\n",Passward); //�������������Ϣ
	ConnectServer("TCP",ServerIP,ServerPort);
	
}
/*----------------------------------------------------------*/
/*�����������ӷ��������� */
/*�� ������ */
/*����ֵ���� */
/*----------------------------------------------------------*/
void MQTT_ConectPack(void)
{
	Fixed_len = 2; //���ӱ����У��̶���ͷ����=2
	Variable_len = 10; //���ӱ����У��ɱ䱨ͷ����=10
	Payload_len = 2 + ClientID_len + 2 + Username_len + 2 + Passward_len; //�ܱ��ĳ��� 
	memset(WiFi_Tx_Buff,0,512);
	WiFi_Tx_Buff[0]=0x10; //�� 1 ���ֽ� ���̶� 0x01
	WiFi_Tx_Buff[1]=Variable_len + Payload_len; //�� 2 ���ֽ� ���ɱ䱨ͷ+��Ч���ɵĳ���
	WiFi_Tx_Buff[2]=0x00; //�� 3 ���ֽ� ���̶� 0x00 
	WiFi_Tx_Buff[3]=0x04; //�� 4 ���ֽ� ���̶� 0x04
	WiFi_Tx_Buff[4]=0x4D; //�� 5 ���ֽ� ���̶�0x4D
	WiFi_Tx_Buff[5]=0x51; //�� 6 ���ֽ� ���̶�0x51
	WiFi_Tx_Buff[6]=0x54; //�� 7 ���ֽ� ���̶�0x54
	WiFi_Tx_Buff[7]=0x54; //�� 8 ���ֽ� ���̶�0x54
	WiFi_Tx_Buff[8]=0x04; //�� 9 ���ֽ� ���̶�0x04
	WiFi_Tx_Buff[9]=0xC2; //�� 10 ���ֽڣ�ʹ���û���������У�飬��ʹ���������������Ự
	WiFi_Tx_Buff[10]=0x00; //�� 11 ���ֽڣ�����ʱ����ֽ� 0x00
	WiFi_Tx_Buff[11]=0x64; //�� 12 ���ֽڣ�����ʱ����ֽ� 0x64 100s
	/* CLIENT_ID */
	WiFi_Tx_Buff[12] = ClientID_len/256; //�ͻ��� ID ���ȸ��ֽ�
	WiFi_Tx_Buff[13] = ClientID_len%256; //�ͻ��� ID ���ȵ��ֽ�
	memcpy(&WiFi_Tx_Buff[14],ClientID,ClientID_len); //���ƹ����ͻ��� ID �ִ�
	/* �û��� */
	WiFi_Tx_Buff[14+ClientID_len] = Username_len/256;  //�û������ȸ��ֽ�
	WiFi_Tx_Buff[15+ClientID_len] = Username_len%256; //�û������ȵ��ֽ�
	memcpy(&WiFi_Tx_Buff[16+ClientID_len],Username,Username_len); //���ƹ����û����ִ�
	/* ���� */
	WiFi_Tx_Buff[16+ClientID_len+Username_len] = Passward_len/256;
	 //���볤�ȸ��ֽ�
	WiFi_Tx_Buff[17+ClientID_len+Username_len] = Passward_len%256;
	 //���볤�ȵ��ֽ�
	memcpy(&WiFi_Tx_Buff[18+ClientID_len+Username_len],Passward,Passward_len); //���ƹ��������ִ�
	WIFI_Send_Buf(WiFi_Tx_Buff, Fixed_len + Variable_len + Payload_len); //���뷢�����ݻ�����
}
/*----------------------------------------------------------*/
/*��������SUBSCRIBE ���� topic ���� */
/*�� ����QoS�����ĵȼ� */
/*�� ����topic_name������ topic �������� */
/*����ֵ���� */
/*----------------------------------------------------------*/
void MQTT_Subscribe(char *topic_name, int QoS)
{
	Fixed_len = 2; //SUBSCRIBE �����У��̶���ͷ����=2
	Variable_len = 2; //SUBSCRIBE �����У��ɱ䱨ͷ����=2
	Payload_len = 2 + strlen(topic_name) + 1; //������Ч���ɳ��� = 2 �ֽ�(topic_name ����)+ topic_name �ַ����ĳ��� + 1 �ֽڷ���ȼ�
	memset(WiFi_Tx_Buff,0,512);
	WiFi_Tx_Buff[0]=0x82; //�� 1 ���ֽ� ���̶�0x82 
	WiFi_Tx_Buff[1]=Variable_len + Payload_len; //�� 2 ���ֽ� ���ɱ䱨ͷ+��Ч���ɵĳ���
	WiFi_Tx_Buff[2]=0x00; //�� 3 ���ֽ� �����ı�ʶ�����ֽڣ��̶�ʹ�� 0x00
	WiFi_Tx_Buff[3]=0x01; //�� 4 ���ֽ� �����ı�ʶ�����ֽڣ��̶�ʹ�� 0x01
	WiFi_Tx_Buff[4]=strlen(topic_name)/256; //�� 5 ���ֽ� ��topic_name ���ȸ��ֽ�
	WiFi_Tx_Buff[5]=strlen(topic_name)%256; //�� 6 ���ֽ� ��topic_name ���ȵ��ֽ�
	memcpy(&WiFi_Tx_Buff[6],topic_name,strlen(topic_name)); //�� 7 ���ֽڿ�ʼ �����ƹ��� topic_name �ִ�
	WiFi_Tx_Buff[6+strlen(topic_name)]=QoS; //��� 1 ���ֽڣ����ĵȼ�
	WIFI_Send_Buf(WiFi_Tx_Buff,Fixed_len + Variable_len + Payload_len);
}

void MQTT_UNSUBSCRIBE(char *topic_name)
{
	Fixed_len = 2; //SUBSCRIBE �����У��̶���ͷ����=2
	Variable_len = 2; //SUBSCRIBE �����У��ɱ䱨ͷ����=2
	Payload_len = 2 + strlen(topic_name) ; //������Ч���ɳ��� = 2 �ֽ�(topic_name ����)+ topic_name �ַ����ĳ���
	WiFi_Tx_Buff[0]=0xA2; //�� 1 ���ֽ� ���̶�0x82 
	WiFi_Tx_Buff[1]=Variable_len + Payload_len; //�� 2 ���ֽ� ���ɱ䱨ͷ+��Ч���ɵĳ���
	WiFi_Tx_Buff[2]=0x00; //�� 3 ���ֽ� �����ı�ʶ�����ֽڣ��̶�ʹ�� 0x00
	WiFi_Tx_Buff[3]=0x02; //�� 4 ���ֽ� �����ı�ʶ�����ֽڣ��̶�ʹ�� 0x01
	WiFi_Tx_Buff[4]=strlen(topic_name)/256; //�� 5 ���ֽ� ��topic_name ���ȸ��ֽ�
	WiFi_Tx_Buff[5]=strlen(topic_name)%256; //�� 6 ���ֽ� ��topic_name ���ȵ��ֽ�
	memcpy(&WiFi_Tx_Buff[6],topic_name,strlen(topic_name)); //�� 7 ���ֽڿ�ʼ �����ƹ��� topic_name �ִ�
	WIFI_Send_Buf(WiFi_Tx_Buff,Fixed_len + Variable_len + Payload_len);

}
/*----------------------------------------------------------*/
/*��������PING ���ģ������� */
/*�� ������ */
/*����ֵ���� */
/*----------------------------------------------------------*/
void MQTT_PingREQ(void)
{
	memset(WIFI_Send_Buf,0,512);
	WiFi_Tx_Buff[0]=0xC0; //�� 1 ���ֽ� ���̶� 0xC0 
	WiFi_Tx_Buff[1]=0x00; //�� 2 ���ֽ� ���̶� 0x00 
	WIFI_Send_Buf(WiFi_Tx_Buff, 2); //�������ݵ�������
}
/*----------------------------------------------------------*/
/*���������ȼ� 0 ������Ϣ���� */
/*�� ����topic_name��topic ���� */
/*�� ����data������ */
/*�� ����data_len�����ݳ��� */
/*����ֵ���� */
/*----------------------------------------------------------*/
void MQTT_PublishQs0(char *topic, char *data, int data_len)
{
	Fixed_len = 2; //�̶���ͷ���ȣ�2 �ֽ�
	Variable_len = 2 + strlen(topic); //�ɱ䱨ͷ���ȣ�2 �ֽ�(topic ����)+ topic �ַ����ĳ���
	Payload_len = data_len; //��Ч���ɳ��ȣ����� data_len
	memset(WiFi_Tx_Buff,0,512);
	WiFi_Tx_Buff[0]=0x30; //�� 1 ���ֽ� ���̶� 0xC0 
	WiFi_Tx_Buff[1]=Variable_len + Payload_len; //�� 2 ���ֽ� ���ɱ䱨ͷ+��Ч���ɵĳ���
//	printf("DATA_LEN==%d\r\n",Variable_len + Payload_len);
	if(Variable_len + Payload_len>=128)
	{

		Fixed_len = 3;
		WiFi_Tx_Buff[2]=0x01;
		WiFi_Tx_Buff[3]=strlen(topic)/256; //�� 3 ���ֽ� ��topic ���ȸ��ֽ�
		WiFi_Tx_Buff[4]=strlen(topic)%256; //�� 4 ���ֽ� ��topic ���ȵ��ֽ�
		memcpy(&WiFi_Tx_Buff[5],topic,strlen(topic)); //�� 5 ���ֽڿ�ʼ ������ topic �ַ���
		memcpy(&WiFi_Tx_Buff[5+strlen(topic)],data,data_len); //����data ����
	}else
	{
		WiFi_Tx_Buff[2]=strlen(topic)/256; //�� 3 ���ֽ� ��topic ���ȸ��ֽ�
		WiFi_Tx_Buff[3]=strlen(topic)%256; //�� 4 ���ֽ� ��topic ���ȵ��ֽ�
		memcpy(&WiFi_Tx_Buff[4],topic,strlen(topic)); //�� 5 ���ֽڿ�ʼ ������ topic �ַ���
		memcpy(&WiFi_Tx_Buff[4+strlen(topic)],data,data_len); //����data ����
	}
	WIFI_Send_Buf(WiFi_Tx_Buff, Fixed_len + Variable_len + Payload_len); //���뷢�����ݻ�����
}

/*----------------------------------------------------------*/
/*����������������������ĵȼ�0������                       */
/*��  ����redata�����յ�����                                */
/*����ֵ����                                                */
/*----------------------------------------------------------*/
void MQTT_DealPushdata_Qs0(unsigned char *redata)
{
	int  re_len;               	           //����һ����������Ž��յ������ܳ���
	int  pack_num;                         //����һ�����������������һ�����ʱ���������͵ĸ���
    int  temp,temp_len;                    //����һ���������ݴ�����
    int  totle_len;                        //����һ������������Ѿ�ͳ�Ƶ����͵���������
	int  topic_len;              	       //����һ���������������������ĳ���
	int  cmd_len;                          //����һ����������������а������������ݵĳ���
	int  cmd_loca;                         //����һ����������������а������������ʼλ��
	int  i;                                //����һ������������forѭ��
	int  local,multiplier;
	unsigned char tempbuff[RBUFF_UNIT];	   //��ʱ������
	unsigned char *data;                   //redata������ʱ�򣬵�һ���ֽ�������������data����ָ��redata�ĵ�2���ֽڣ����������ݿ�ʼ�ĵط�
		
	re_len = redata[0]*256+redata[1];                     //��ȡ���յ������ܳ���		
	data = &redata[2];                                    //dataָ��redata�ĵ�2���ֽڣ����������ݿ�ʼ�� 
	pack_num = temp_len = totle_len = temp = 0;           //������������
	local = 1;
	multiplier = 1;
	do{
		pack_num++;                                       //��ʼѭ��ͳ�����͵ĸ�����ÿ��ѭ�����͵ĸ���+1	
		do{
			temp = data[totle_len + local];   
			temp_len += (temp & 127) * multiplier;
			multiplier *= 128;
			local++;
		}while ((temp & 128) != 0);
		totle_len += (temp_len + local);                  //�ۼ�ͳ�Ƶ��ܵ����͵����ݳ���
		re_len -= (temp_len + local) ;                    //���յ������ܳ��� ��ȥ ����ͳ�Ƶ����͵��ܳ���      
		local = 1;
		multiplier = 1;
		temp_len = 0;
	}while(re_len!=0);                                    //������յ������ܳ��ȵ���0�ˣ�˵��ͳ�������
	printf("���ν�����%d����������\r\n",pack_num);		//���������Ϣ
	temp_len = totle_len = 0;                		      //������������
	local = 1;
	multiplier = 1;
	for(i = 0; i < pack_num; i++)			//�Ѿ�ͳ�Ƶ��˽��յ����͸�������ʼforѭ����ȡ��ÿ�����͵����� 
	{                                		
		do{
			temp = data[totle_len + local];   
			temp_len += (temp & 127) * multiplier;
			multiplier *= 128;
			local++;
		}while ((temp & 128) != 0);				
		topic_len = data[local + totle_len]*256 + data[local + 1 + totle_len] + 2; //���㱾����������������ռ�õ�������
		cmd_len = temp_len - topic_len;                              			   //���㱾��������������������ռ�õ�������
		cmd_loca = totle_len + local +  topic_len;                  			   //���㱾�������������������ݿ�ʼ��λ��
		memcpy(tempbuff, &data[cmd_loca], cmd_len);                   			   //�������ݿ�������		                 
		CMDBuf_Deal(tempbuff, cmd_len);                             			   //�������������
		totle_len += (temp_len + local);                              			   //�ۼ��Ѿ�ͳ�Ƶ����͵����ݳ���
		local = 1;
		multiplier = 1;
		temp_len = 0;
	}	
}

/*----------------------------------------------------------*/
/*�������������������									 */
/*��  ����data������                                        */
/*��  ����size�����ݳ���                                    */
/*����ֵ����                                                */
/*----------------------------------------------------------*/
void CMDBuf_Deal(unsigned char *data, int size)
{
	memcpy(&MQTT_CMDInPtr[2], data,size);         //�������ݵ��������
	MQTT_CMDInPtr[0] = size/256;              	  //��¼���ݳ���
	MQTT_CMDInPtr[1] = size%256;                  //��¼���ݳ���
	MQTT_CMDInPtr[size+2] = '\0';                 //�����ַ���������
	MQTT_CMDInPtr += CBUFF_UNIT;               	  //ָ������
	if(MQTT_CMDInPtr == MQTT_CMDEndPtr)           //���ָ�뵽������β����
		MQTT_CMDInPtr = MQTT_CMDBuf[0];        	  //ָ���λ����������ͷ
}
