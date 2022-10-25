#include "DF_Protocol.hpp"
#include <vector>

struct Protocol_t
{
	uint16_t overTime = 500;
	uint16_t nowTime;
};

std::vector <Protocol_t> sendProtocol;

void AnalysisData(uint8_t *data,uint16_t len)
{
	Protocol_t a;
	sendProtocol.push_back(a);
	for(size_t i = 0; i < len;i++)
	{
		
	}
}