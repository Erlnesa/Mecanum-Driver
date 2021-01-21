#include "convert.h"
#include <math.h>

long int convert(u8 bit1,u8 bit2)
{
	u8 NH,NL;
	double small=0.0,end=0.0;
//	mod=bit1>>7;			//符号位
	NH=(bit1>>3)&0x0f;
	NL=bit1&0x07;
	if(NH)
	{
		end=pow(2,NH-7);
		if(NL&0x04)small=small+0.5;
		if(NL&0x02)small=small+0.25;
		if(NL&0x01)small=small+0.125;
		if(bit2&0x80)small=small+0.0625;
		if(bit2&0x40)small=small+0.03125;
		if(bit2&0x20)small=small+0.015625;
		if(bit2&0x10)small=small+0.0078125;
		if(bit2&0x08)small=small+0.00390625;
		end=end*(1+small);
		end=end*100;
		return end;
	}
	else 
	{
		end=pow(2,NH-6);
		if(NL&0x04)small=small+0.5;
		if(NL&0x02)small=small+0.25;
		if(NL&0x01)small=small+0.125;
		if(bit2&0x80)small=small+0.0625;
		if(bit2&0x40)small=small+0.03125;
		if(bit2&0x20)small=small+0.015625;
		if(bit2&0x10)small=small+0.0078125;
		if(bit2&0x08)small=small+0.00390625;
		end=end*small;
		end=end*100;//强制保留两位小数
		return end;
	}
}
long int convert_t(u8 bit1,u8 bit2)
{
	u8 NH,NL;
	double small=0.0,end=0.0;
//	mod=bit1>>7;			//符号位
	NH=(bit1>>3)&0x0f;
	NL=bit1&0x07;
	if(NH)
	{
		end=pow(2,NH-7);
		if(NL&0x04)small=small+0.5;
		if(NL&0x02)small=small+0.25;
		if(NL&0x01)small=small+0.125;
		if(bit2&0x80)small=small+0.0625;
		if(bit2&0x40)small=small+0.03125;
		if(bit2&0x20)small=small+0.015625;
		if(bit2&0x10)small=small+0.0078125;
		if(bit2&0x08)small=small+0.00390625;
		end=end*(1+small);
		end=(end-30)/1.8;
		end=end*100;
		return end;
	}
	else 
	{
		end=pow(2,NH-6);
		if(NL&0x04)small=small+0.5;
		if(NL&0x02)small=small+0.25;
		if(NL&0x01)small=small+0.125;
		if(bit2&0x80)small=small+0.0625;
		if(bit2&0x40)small=small+0.03125;
		if(bit2&0x20)small=small+0.015625;
		if(bit2&0x10)small=small+0.0078125;
		if(bit2&0x08)small=small+0.00390625;
		end=end*small;
		end=(end-30)/1.8;
		end=end*100;//强制保留两位小数
		return end;
	}
}
