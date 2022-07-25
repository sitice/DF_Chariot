#include "DF_Move.hpp"
#include "pwm.h"
/*
	���ļ����������˶����ƺ����Լ�������Ϣ��ȡ����
	
*/
Move_t Move; //��������Ϣ���дͷ�ļ���
/**
  * @brief ����ȫ���ƶ�
	* @note  ���������˳ʱ����תʱΪ������,����˳ʱ����תΪ������
  * @param x:x�����ϵ��ٶ�
	* @param y:y�����ϵ��ٶ�
	* @param z:z�������ٶ�
  * @retval none
  */
void OmnibearingMove(uint16_t x,uint16_t y,uint16_t z)
{
	int16_t speed1 = -x-y-z;
	int16_t speed2 = -x+y-z;
	int16_t speed3 = x+y-z;
	int16_t speed4 = x-y-z;
	
	speed1 = speed1 > 1000 ? 1000 : speed1;
	speed1 = speed1 < -1000 ? -1000 : speed1;
	speed2 = speed2 > 1000 ? 1000 : speed2;
	speed2 = speed2 < -1000 ? -1000 : speed2;
	speed3 = speed3 > 1000 ? 1000 : speed3;
	speed3 = speed3 < -1000 ? -1000 : speed3;
	speed4 = speed4 > 1000 ? 1000 : speed4;
	speed4 = speed4 < -1000 ? -1000 : speed4;
	
	Move.motor1->SetRpm(speed1);
	Move.motor2->SetRpm(speed2);
	Move.motor3->SetRpm(speed3);
	Move.motor4->SetRpm(speed4);
}
/**
  * @brief ��ȡ����x��y������λ��
	* @note  none
  * @param none
  * @retval none
  */
void GetDistance(void)
{
	Move.Distance.x = -((Move.motor4->EncoderCntSum-Move.motor2->EncoderCntSum)-(Move.motor1->EncoderCntSum-Move.motor3->EncoderCntSum))*0.707106781f*0.023419f;
	Move.Distance.y = ((Move.motor4->EncoderCntSum-Move.motor2->EncoderCntSum)+(Move.motor1->EncoderCntSum-Move.motor3->EncoderCntSum))*0.707106781f*0.023419f;
}

