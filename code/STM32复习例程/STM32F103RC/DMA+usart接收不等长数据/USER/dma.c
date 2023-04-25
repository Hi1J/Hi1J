#include "dma.h"

#if USE_USART_DMA_RX 
 
void USARTx_DMA_Rx_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
 
	// ����DMAʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;// ����DMAԴ��ַ���������ݼĴ�����ַ*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART1_RX_BUF;// �ڴ��ַ(Ҫ����ı�����ָ��)
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;// ���򣺴����赽�ڴ�	
	DMA_InitStructure.DMA_BufferSize = USART1_REC_LEN;// DMAͨ����DMA����Ĵ�С    
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;// �����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	//�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;// �������ݵ�λ  8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 // �ڴ����ݵ�λ   8λ
	// DMAģʽ��һ�λ���ѭ��ģʽ
	//DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; // ���ȼ�����	
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
		   
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);// ����DMAͨ��		����1���ն�Ӧͨ��5	
	
	DMA_ClearFlag(DMA1_FLAG_TC5);// ���DMA���б�־
	DMA_ITConfig(DMA1_Channel5, DMA_IT_TE, ENABLE);//������� �ж�����
	// ʹ��DMA
	DMA_Cmd (DMA1_Channel5,ENABLE);
}
#endif