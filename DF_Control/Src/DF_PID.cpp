#include "DF_PID.hpp"

 _PID_Position PID_Pos_1;
 _PID_Position PID_Pos_2;
 _PID_Position PID_Pos_3;
 _PID_Position PID_Pos_4;
 _PID_Position PID_Pos_5;
 _PID_Position PID_Pos_6;
 _PID_Position PID_Pos_7;
 _PID_Position PID_Pos_8;
 _PID_Position PID_Pos_9;
 
float Pid_Value_Array [9][6]=
{ 
	{350, 3, 4,   1000,     -1000,      0 },
	{350, 3, 4,   1000,     -1000,      0 },
	{350, 3, 4,   1000,     -1000,      0 },
	{350, 3, 4,   1000,     -1000,      0 },
	
	{1, 1,  1,    1000,     -1000,      0  },
	{1, 1,  1,    1000,     -1000,      0  },
	{1, 1,  1,    1000,     -1000,      0  },
	{1, 1,  1,    1000,     -1000,      0  },
	
  {1, 1,  1,    1000,     -1000,      0  },
};




