#include "DF_Filter.hpp"

ICM20602::Acc_f  filter_acc;//�˲�����ٶ�
ICM20602::Gyro_f filter_gyro;//�˲�����ٶ�
ICM20602::Gyro_f SI_gyro;//���ٶ����ݣ�rad��
ICM20602::Acc_f  nacc;//�ߵ����ٶ�


//��ֹƵ��20hz
double b[7]={4.863987500781e-08,2.918392500469e-07,7.295981251171e-07,9.727975001562e-07,
  7.295981251171e-07,2.918392500469e-07,4.863987500781e-08};
double a[7]={1,   -5.514535121166,    12.68911305652,    -15.5936352107,
      10.79329667049,   -3.989359404231,   0.6151231220526};

struct _filter groy_filter[3];
struct _filter acc_filter[3];
struct _filter gyro_filter[3];

float IIRLowPass(int16_t x, struct _filter *Acc)
{
	int i;
	//����֮ǰBuf��ǰ�ƶ�һ��λ�ã��Ա���֮ǰBuf�����ݣ�
	for(i=6; i>0; i--){
		Acc->outBuf[i] = Acc->outBuf[i-1];
		Acc->inBuf[i] = Acc->inBuf[i-1];
	}
	Acc->inBuf[0] = x;
	Acc->outBuf[0] = 0;
	for(i=1;i<7;i++)
	{
		Acc->outBuf[0] = Acc->outBuf[0] + b[i] * Acc->inBuf[i];
		Acc->outBuf[0] = Acc->outBuf[0] - a[i] * Acc->outBuf[i];
	}
	Acc->outBuf[0] = Acc->outBuf[0] + b[0] * Acc->inBuf[0];
	return Acc->outBuf[0];
}

void GROY_IIR_Filter( ICM20602::Gyro_t *Acc_in,ICM20602::Gyro_f *Acc_out)
{ //���µõ����ݺ��ϴ���������ݽ��д���
	Acc_out->x = IIRLowPass(Acc_in->x, &groy_filter[0]); 
	Acc_out->y = IIRLowPass(Acc_in->y, &groy_filter[1]);
	Acc_out->z = IIRLowPass(Acc_in->z, &groy_filter[2]); 
}

void ACC_IIR_Filter( ICM20602::Acc_t *Acc_in, ICM20602::Acc_f *Acc_out)
{ //���µõ����ݺ��ϴ���������ݽ��д���
	Acc_out->x = IIRLowPass(Acc_in->x, &acc_filter[0]); 
	Acc_out->y = IIRLowPass(Acc_in->y, &acc_filter[1]);  
	Acc_out->z = IIRLowPass(Acc_in->z, &acc_filter[2]); 
}


//�õ�����
void Get_Radian(ICM20602::Gyro_t *Gyro_in,struct ICM20602::Gyro_f *Gyro_out)
{
	Gyro_out->x = (float)(Gyro_in->x * RawData_to_Radian);
	Gyro_out->y = (float)(Gyro_in->y * RawData_to_Radian);
	Gyro_out->z = (float)(Gyro_in->z * RawData_to_Radian);
	//ע�������Gyro_out->x �����Ķ�������void Get_Radian(struct _gyro *Gyro_in,struct _SI_float *Gyro_out)
	//���ǿ���ע�⵽����ʵ����������ǽ����ݷŵ��� ��Gyro_in����Gyr o_out�� ��������ַ����
}

