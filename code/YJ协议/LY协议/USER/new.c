#include "new.h"

uint8_t YJ_RX_Aframe_OK = 0;//����һ֡������ɱ�־λ��Ҳ��Ϊ������ɱ�־λ�� �ڴ�������յ������ݺ����ֶ����� ��������
static uint8_t Multipe_data_frames_Flag = 1;//һ���Դ���������֡������־λ Ĭ��Ϊ1
static uint8_t YJ_TASK_FALG = YJ_OFF;//�������Ƿ������б�־λ

struct Yj_node Yjnode1 = {{0},NULL};//����1
struct Yj_node Yjnode2 = {{0},NULL};//����2
struct Yj_node Yjnode3 = {{0},NULL};//����3


//�����ʼ��
void YJ_Init(void)
{

	Yjnode1.next = &Yjnode2;
	Yjnode2.next = &Yjnode3;
	
}

//�������
void YJ_Tset(void)
{
	struct Yj_node *p = &Yjnode1;
	while(p != NULL)
	{
		printf("%s\r\n",p->YJ_RX_Buf);
		p = p->next;
	}

}
/*
�������ܣ���Ƭ�����뼶����ʱ
�������ӳٺ�����
����ֵ��void
��ע���˺����Ǳ���ʹ�� 
*/
void YJ_Delay_Ms(uint16_t time)
{
	uint8_t MHz = System_Frequency_MHz;
	time = time * 100;
	while(time--)
	{
		while(MHz--)
		{
			/*ʲôҲ������*/
		}
		MHz = System_Frequency_MHz;
	}
}

/*
�������ܣ����ݴ�������ʼִ��
������void
����ֵ��void
��ע�����ݴ����� ��ʼִ��ʱ�� ���ݴ�����״̬��־λ ��0
*/
void YJ_TASK_START(void)
{
	YJ_TASK_FALG = YJ_ON;
}

/*
�������ܣ����ݴ���������ִ��
������void
����ֵ��void
��ע�����ݴ����� ����ִ��ʱ�� ���ݴ�����״̬��־λ ��0
*/
void YJ_TASK_END(void)
{
	YJ_TASK_FALG = YJ_OFF;
}

/*
�������ܣ��鿴���ݴ������Ƿ��ڽ���
������void
����ֵ��YJ_TASK_FALG
��ע���鿴��ǰ��ϵͳ�Ƿ����ڴ���֮ǰ���յ�����
*/
uint8_t YJ_TASK_STATUS(void)
{
	return YJ_TASK_FALG;
}


/*
�������ܣ����ڴ��ڽ�������
������data ���� ȡ�Դ��ڵ�DR�Ĵ�����stm32ƽ̨��  ����ƽ̨ȡ��ؼĴ���������
����ֵ��void
��ע���˺������ڵ�Ƭ���Ĵ��ڽ����ж� һ��ֻ����һ���ֽ�
     �û�����Ҫ�õ����ڿ����жϿ����и���������д
*/
void YJ_Data_reception(uint8_t data)
{
	static uint8_t YJ_Rx_Count= 1;//����ֵ ��8��1
	static uint8_t YJ_Rx_head = 0;//��ʼλ���ջ�����
	static uint8_t YJ_Rx_flag = 0;//��ʼ���ձ�־λ
	static struct Yj_node *P_RX = &Yjnode1; //Ĭ�Ͻ����ݷ���buf1
	
	//�����һ���ֽ��ǿ�ʼλ
	 if((data == Start_bit) && (YJ_Rx_flag == 0))YJ_Rx_head = data;//�����ֽڱ�������ʼλ������ 
	 else if((data == Address) && (YJ_Rx_head == Start_bit) && (YJ_Rx_flag == 0))//����ڶ����ֽ��ǵ�ַλ 
	 {
			YJ_Rx_flag = 1;//���ձ�־λ��1 ��ʼ����
			if(((YJ_RX_Aframe_OK == 1)|| (YJ_TASK_STATUS() == YJ_ON)) && (P_RX->next != NULL)){P_RX = P_RX->next;Multipe_data_frames_Flag++;}//���������������֡���ߵ�ǰ�������ڴ����һ����㹻�Ĵ���ռ� �����ݱ��浽��һ��buf ��������֡������־λ��1
			else P_RX = &Yjnode1;//����ָ��buf1
			P_RX->YJ_RX_Buf[0] = YJ_Rx_head;//����ʼλ����buf[0]����
			YJ_Rx_head = 0;//��ʼλ���ջ�������0
	 }
	 else  //����ڶ����ֽڲ��ǵ�ַλ ������֡������Ǵ���������
		 YJ_Rx_head = 0;//��ʼλ���ջ�������0

	//������ձ�־λΪ1 ��ʼ����
	 if(YJ_Rx_flag == 1)
	 {
		P_RX->YJ_RX_Buf[YJ_Rx_Count++] = data;//�����ݷ���BUF����
		 //������һ��bufʱ Ĭ��һ֡���ݽ������ ����־λ��λ �Խ�����һ֡����
		 if(YJ_Rx_Count == Data_Frame_Max)
		 {
			 YJ_Rx_flag = 0;//���տ�ʼλ��0
			 YJ_Rx_Count = 1;//����ֵ��λ
			 YJ_Rx_head = 0;//��ʼλ��������λ ��ֹ����
			 YJ_RX_Aframe_OK = 1;//������һ֡���ݺ� ���ձ�־λ��1
		 }
	 }
}

/*
�������ܣ����ݴ�����
�������û��Զ��壨Ĭ��void��
����ֵ���û��Զ��� ��Ĭ��void��
��ע���˺���������Ϊ���������ݽ������� ���������ݴ�������������ط�д�����û��Զ��壩
			�˺���Ҳ������Ϊ���ݴ�����
*/
void YJ_Data_Analysis(void)
{
		static struct Yj_node *P_handle = &Yjnode1; //Ĭ�ϴ���buf1������
		
		YJ_Delay_Ms(1);//Ϊ��ֹ���� �˴��ɷ���1ms��ʱ ������ʱʱ��ɸ��ݴ��ڲ��������� ���������͵���ʱ���������û��������ʹ��
	//���ݴ��ڲ�����115200�����ݴ����ʽΪ1����ʼλ��1��ֹͣλ��8������λ���� 1ms�ɽ���11.5���ֽ� �պñ�һ֡���ݽ��յ�ʱ�䳤 
		
		YJ_TASK_START();//���ݴ�������ʼ
		
		while(Multipe_data_frames_Flag--)//�������
		{
			//************�û��Զ�����*****************//
			
			for(u8 i=0;i<Data_Frame_Max;i++)
				{
					printf("%x ",P_handle->YJ_RX_Buf[i]);
				}
			printf("\r\n");
			
			memset(P_handle->YJ_RX_Buf,0,Data_Frame_Max);//�˺��������û������� �û��ɸ����������ɾ�� ��һ������»���������Է�ֹ����
			
			//************�û��Զ�����*****************//
			
			if(Multipe_data_frames_Flag != 0)//�������δ���������
				P_handle = P_handle->next;//��ָ����һ������
		}
		P_handle = &Yjnode1;//�����������ݺ� ����ָ��buf1 �Ա�������ݵĴ���
		Multipe_data_frames_Flag = 1;//����֡������־λ������1
		YJ_RX_Aframe_OK = 0;//����һ֡��־λ����
		
		YJ_TASK_END();//���ݴ���������
}





