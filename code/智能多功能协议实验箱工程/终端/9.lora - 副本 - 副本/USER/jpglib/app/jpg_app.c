#include "jpg_app.h"
static int MaskL = 0; 
static int MaskR = LCD_WIDTH - 1; 
static int MaskT = 0; 
static int MaskB = LCD_HEIGHT - 1;
u16 jpg_start_x=0,jpg_start_y=0;
/*********************************************************************************************
  ��������:    STM32_Display
  ��������:    ��TFTLCD��Ļ����ʾͼƬ 
  ��ڲ���:    ������ͷ
  ���ڲ���:    ��
  ȫ�ֱ���:    ��
  ��ע˵��:    ��
*********************************************************************************************/
void Lcd_Display_Rect( 
                            int left,                            /*ͼƬ����ʼ�㣬��һ�е���ʼ�� */ 
                            int right,                           /*ͼƬ�ҷ��Ľ����㣬��һ�еĽ�����*/ 
                            int top,                             /* ͼƬ�Ϸ�����ʼ�㣬��һ�е���ʼ�� */ 
                            int bottom,                          /*ͼ���·��Ľ����㣬��һ�еĽ����� */ 
                            const uint16_t * RGB_Data_Pointer    /* ����ʾ��ͼ�����ݣ�RGB��ʽ*/ 
                        ) 
{ 
	  u32 total_pixelpoint = 0;//ͳ����ʾͼƬһ�������ص�ĸ���
    int hight_jpg,width_jpg; //��¼��ʾͼƬ�ĸ߶ȺͿ��
	  left += jpg_start_x;     //�޸�x����ʾλ��
	  right += jpg_start_x;
	  top += jpg_start_y;      //�޸�y����ʾλ��
	  bottom += jpg_start_y;
    if (left > right || top > bottom)  
    { 
        return;             // Check varidity 
    } 
    
    if (left > MaskR || right < MaskL  || top > MaskB || bottom < MaskT)  
    { 
        return;                // Check if in active area  
    }
		hight_jpg = bottom - top + 1 ;           /* Vertical size *///����ͼ��ĸ߶�
    width_jpg = right - left + 1;            /* Horizontal size and skip *///����ͼ��Ŀ��
		LCD_Address_Set(left,top,right,bottom);  //������ʾ����
		total_pixelpoint = hight_jpg*width_jpg;  //������ʾ���������ص�ĸ���
		for(u32 i=0;i<total_pixelpoint;i++)      //��ʼ����Ļ����ʾ����
		{	
				LCD_WR_DATA(*RGB_Data_Pointer++);	 			 
		}		
} 
/* 
User defined input funciton                          
jd        //���������Ķ�����Ϣ�Ľṹ��  
buff,     //�������ݻ����� (NULL:ɾ������)
nbyte     //��Ҫ����������������/ɾ����������
*/

uint16_t in_func (JDEC* jd, uint8_t* buff, uint16_t nbyte)
{
	UINT  rb; 
	FIL *dev = (FIL *)jd->device;   /* Device identifier for the session (5th argument of jd_prepare function) */

	if (buff) {
			/* Read bytes from input stream */
		  f_read(dev, buff, nbyte, &rb); //����FATFS��f_read���������ڰ�jpeg�ļ������ݶ�ȡ����
			return rb;                     // ���ض�ȡ�����ֽ���Ŀ 
	} else {
			/* Remove bytes from input stream */
			return (f_lseek(dev, f_tell(dev) + nbyte) == FR_OK) ? nbyte : 0;/* ���¶�λ���ݵ㣬�൱��ɾ��֮ǰ��n�ֽ����� */ 
	}
}

/*------------------------------*/
/* User defined output funciton */
/*------------------------------*/

uint16_t out_func (JDEC* jd, void* bitmap, JRECT* rect)
{
		jd = jd;    /* ˵�������������JDEC�ṹ��û���õ� */ 
    //��ʾһ������LCD��
		Lcd_Display_Rect(rect->left, rect->right, rect->top, rect->bottom, (uint16_t*)bitmap);
    return 1;    /* Continue to decompress */ /*����1��ʹ���빤������ִ�� */ 
}

//���ز���ʾjpg�ļ�
void load_jpg (
    FIL *fp,        /* ָ��򿪵��ļ�ִ�� */
    void *work,     /*ָ�����ֽڶ���Ĺ��������� */
    UINT sz_work    /*�������Ĵ�С */
)
{
    JDEC jd;        /* Decoder object (124 bytes) */
    JRESULT rc;
    BYTE scale;
    /* Prepare to decompress the file */
    rc = jd_prepare(&jd, in_func, work, sz_work, fp);
    if (rc == JDR_OK) 
    {
			  printf("Image dimensions: %u by %u. %u bytes used.\r\n", jd.width, jd.height, 4096 - jd.sz_pool);
        /* ����ͼƬ��Сѡѡ��һ���ոպ��ܹ����ŵ�ͼƬ���� */
        for (scale = 0; scale < 3; scale++) 
        {
						
            if ((jd.width >> scale) <= LCD_WIDTH && (jd.height >> scale) <= LCD_HEIGHT)break;
						
        }

        rc = jd_decomp(&jd, out_func, scale);    /* Start to decompress */

    } else 
    {
        //��ʾ����,��������Ϣ��ӡ����Ļ��
        printf("jpg error %d\r\n",rc);
    }
}
#define SIZE_OF_FILE_BUFFER        4096
BYTE jpegbuffer[SIZE_OF_FILE_BUFFER]; //ͼ���ļ����ݻ�����
/*����:ָ���ļ��� */
void JPG_ShowFunc (const char *fn,u16 start_x,u16 start_y)
{
    FIL fil;            /* Pointer to a file object */
    
    if (f_open(&fil, fn, FA_READ) == FR_OK) 
    {
			jpg_start_x=start_x;//����ʾͼƬ������ʼ���е�ַ���е�ַ
			jpg_start_y=start_y;
			load_jpg(&fil, (void *)jpegbuffer, SIZE_OF_FILE_BUFFER );  //��jpg�ļ���������ʾ
      f_close(&fil);
    }
    else
    {
        printf("open file failed\r\n");
    }
}


