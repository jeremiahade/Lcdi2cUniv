/*******************************************************************
**File Name: xxxxxxxxxxxx.h/c                                     **
**Library Name: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx.          **
**Original Project Name: Null.                                    **
**Author Name: Jeremiah A.                                        **
**Version No:  2.0                                                **
**Date Created: 11th May 2020.                                    **
**Last Edited: 29th August 2020.                                  **
********************************************************************/

#include "lcdi2c.h"

uint8_t lcdPortCmd = 0;

void InitialiseLCD(lcdmodule_t* lcdModule);
void WriteLCDCmdReg(const uint8_t addr, uint8_t cmd);
void PrintLCDChar(const uint8_t addr, unsigned char data);
void PrintLCDCharXY16x2(const uint8_t addr, uint8_t data, uint8_t X, uint8_t Y);
void PrintLCDCharXY20x4(const uint8_t addr, uint8_t data, uint8_t X, uint8_t Y);
void PrintStringLCD(const uint8_t addr, char* data);
void PrintStringLCDXY16x2(const uint8_t addr, char* data, uint8_t X, uint8_t Y);
void PrintStringLCDXY20x4(const uint8_t addr, char* data, uint8_t X, uint8_t Y);
void ShiftCursorPos16x2(uint8_t addr, uint8_t X, uint8_t Y);
void ShiftCursorPos20x4(uint8_t addr, uint8_t X, uint8_t Y);
void SwitchDisplayCursor(uint8_t addr, dispCursor_t state);
void SwitchLineNumMatrixState(uint8_t addr, lineMatrx_t state);
void SelCursorMovement(uint8_t addr, cursrMovment_t state);
void ShiftDisplayCursrRight(uint8_t addr);
void GoHome(uint8_t addr);
void GoFirstLine(uint8_t addr);
void GoSecondLine(uint8_t addr);
void SwitchLEDOn(uint8_t addr);
void ClearDisplay16x2(uint8_t addr);
void ClearDisplay20x4(uint8_t addr);
void ClearDisplay1stLine16x2(uint8_t addr);
void ClearDisplay2ndLine16x2(uint8_t addr);
void ClearDisplay1stLine20x4(uint8_t addr);
void ClearDisplay2ndLine20x4(uint8_t addr);
void ClearDisplay3rdLine20x4(uint8_t addr);
void ClearDisplay4thLine20x4(uint8_t addr);
void ClearDisplay(uint8_t addr);


void InitialiseLCD(lcdmodule_t* lcdModule)
{
	Writei2c(lcdModule->addr, 0xFF);
	MsDelay(50);
	WriteLCDCmdReg(lcdModule->addr, INIT_CMD);
	MsDelay(200);
	WriteLCDCmdReg(lcdModule->addr, INIT_CMD1);
	MsDelay(200);
	SwitchLineNumMatrixState(lcdModule->addr, lcdModule->lineMatrixSel);
	SwitchDisplayCursor(lcdModule->addr, lcdModule->dispCursorSt);
	SelCursorMovement(lcdModule->addr, lcdModule->cursrMovementSel);
	if (lcdModule->type == LCD16x2)
	{
		ClearDisplay16x2(lcdModule->addr);
	}
	else
	{
		ClearDisplay20x4(lcdModule->addr);
	}
}

void WriteLCDCmdReg(const uint8_t addr, uint8_t cmd)
{
	//uint8_t lcdPortCmd	=	0;
	uint8_t addrBuf =	addr;
	lcdPortCmd	=	(lcdPortCmd & 0x0F) | (cmd & 0xF0); //Get Upper Bits
	lcdPortCmd	&=	~(1<<RS);
	lcdPortCmd  &=   ~(1<<R_W);
	lcdPortCmd  |=  (1<<LED);
	Writei2c(addrBuf, lcdPortCmd);
	lcdPortCmd	&=	~(1<<RS);
	lcdPortCmd &=   ~(1<<R_W);
	Writei2c(addrBuf, lcdPortCmd);
	lcdPortCmd	|=	(1 << EN);
	Writei2c(addrBuf, lcdPortCmd);
	uSDelay(1);
	lcdPortCmd &=	~(1 << EN);
	Writei2c(addrBuf, lcdPortCmd);
	uSDelay(200);

	lcdPortCmd	=	(lcdPortCmd & 0x0F)	|	(cmd << 4); //Get Lower Bits
	//lcdPortCmd	&=	~(1<<RS);
	//lcdPortCmd  &=   ~(1<<R_W);
	lcdPortCmd  |=  (1<<LED);
	Writei2c(addrBuf, lcdPortCmd);
	lcdPortCmd	|=	(1 << EN);
	Writei2c(addrBuf, lcdPortCmd);
	uSDelay(1);
	lcdPortCmd	&=	~(1 << EN);
	MsDelay(2);
}

void PrintLCDChar(const uint8_t addr, unsigned char data)
{
	//uint8_t dataBuf =	0;
	uint8_t	addrBuf	=	addr;
	lcdPortCmd = (lcdPortCmd & 0x0F ) | (data & 0xF0);;

	Writei2c(addrBuf, lcdPortCmd);
	lcdPortCmd |=	(1 << RS);
	Writei2c(addrBuf, lcdPortCmd);
	lcdPortCmd |=	(1<<EN);
	Writei2c(addrBuf, lcdPortCmd);
	uSDelay(1);
	lcdPortCmd &=	~(1<<EN);
	Writei2c(addrBuf, lcdPortCmd);
	uSDelay(200);

	lcdPortCmd	=	(lcdPortCmd & 0x0F)	|	(data << 4);
	Writei2c(addrBuf, lcdPortCmd);
	lcdPortCmd |= (1 << EN);
	Writei2c(addrBuf, lcdPortCmd);
	uSDelay(1);
	lcdPortCmd	&=	~(1	<< EN);
	Writei2c(addrBuf, lcdPortCmd);
	MsDelay(2);
}

void PrintLCDCharXY16x2(const uint8_t addr, uint8_t data, uint8_t X, uint8_t Y)
{
	ShiftCursorPos16x2(addr, X, Y);
	PrintLCDChar(addr, data);
}

void PrintLCDCharXY20x4(const uint8_t addr, uint8_t data, uint8_t X, uint8_t Y)
{
	ShiftCursorPos20x4(addr, X, Y);
	PrintLCDChar(addr, data);
}

void PrintStringLCD(const uint8_t addr, char* data)
{
	uint16_t iteration = 0;

	while(data[iteration] != 0)
	{
		PrintLCDChar(addr, data[iteration]);
		iteration++;
	}

}

void PrintStringLCDXY16x2(const uint8_t addr, char* data, uint8_t X, uint8_t Y)
{
	ShiftCursorPos16x2(addr, X, Y);
	PrintStringLCD(addr, data);
}

void PrintStringLCDXY20x4(const uint8_t addr, char* data, uint8_t X, uint8_t Y)
{
	ShiftCursorPos20x4(addr, X, Y);
	PrintStringLCDXY20x4(addr, data, X, Y);
}

void ShiftCursorPos16x2(uint8_t addr, uint8_t X, uint8_t Y)
{
	X -= 1;
	Y -= 1;


	if(X <= LCD16x2XMIN)
	{
		X = 0;
	}
	if(X >= LCD16x2XMAX)
	{
		X = 15;
	}
	if(Y <= LCD16x2XMIN)
		{
			Y = 0;
		}
	if(Y >= LCD16x2YMAX2LINE)
	{
		Y = 1;
	}


	switch(Y)
	{
	case 0:
		WriteLCDCmdReg(addr, ((X  + 0x00) | MOVE_CURSOR_1STLINE));
		break;
	case 1:
		WriteLCDCmdReg(addr, ((X + 0x40) | MOVE_CURSOR_2NDLINE));
		break;
	default:
		break;
	}
}

void ShiftCursorPos20x4(uint8_t addr, uint8_t X, uint8_t Y)
{
	X -= 1;
	Y -= 1;

	if(X <= LCD20x4XMIN)
		{
			X = 0;
		}
	if(X >= LCD20x4XMAX)
	{
		X = 19;
	}
	if(Y <= LCD20x4XMIN)
			{
				Y = 0;
			}
	if(Y >= LCD20x4YMAX2LINE)
	{
		Y = 3;
	}
	switch(Y)
	{
	case 0:
		WriteLCDCmdReg(addr, ((X & 0x0F) | MOVE_CURSOR_1STLINE));
		break;
	case 1:
		WriteLCDCmdReg(addr, ((X & 0x0F) | MOVE_CURSOR_2NDLINE));
		break;
	case 2:
		WriteLCDCmdReg(addr, (((X+20) & 0x0F) | MOVE_CURSOR_1STLINE));
		break;
	case 3:
		WriteLCDCmdReg(addr, (((X+20) & 0x0F) | MOVE_CURSOR_2NDLINE));
		break;
	default:
		break;
		}
}

void SwitchDisplayCursor(uint8_t addr, dispCursor_t state)
{
	switch(state)
	{
	case dispOff_CursorOff:
		WriteLCDCmdReg(addr, DISPOFF_CURSOROFF);
		break;
	case dispOff_CursorOn:
		WriteLCDCmdReg(addr, DISPOFF_CURSORON);
		break;
	case dispOn_Cursoroff:
		WriteLCDCmdReg(addr, DISPON_CURSOROFF);
		break;
	case dispOn_CursorOn:
		WriteLCDCmdReg(addr, DISPON_CURSORON);
		break;
	case dispOn_CursorBlink:
		WriteLCDCmdReg(addr, DISPON_CURSORBLINK);
		break;
	default:
		WriteLCDCmdReg(addr, DISPON_CURSOROFF);
	}
}

void SwitchLineNumMatrixState(uint8_t addr, lineMatrx_t state)
{
	switch(state)
	{
	case twolines_5x8Matrix:
		WriteLCDCmdReg(addr, TWOLINES_5x8MATRX);
		break;
	case twolines_5x10Matrix:
		WriteLCDCmdReg(addr, TWOLINES_5x10MATRX);
		break;
	case oneline_5x8Matrix:
		WriteLCDCmdReg(addr, ONELINE_5x8MATRX);
		break;
	case oneline_5x10Matrix:
		WriteLCDCmdReg(addr, ONELINE_5x10MATRX);
		break;
	default:
		WriteLCDCmdReg(addr, TWOLINES_5x8MATRX);
	}
}

void SelCursorMovement(uint8_t addr, cursrMovment_t state)
{
	switch(state)
	{
	case incrementCursor:
		WriteLCDCmdReg(addr, INCREMENT_CURSOR);
		break;
	case incrementCursorShiftDisp:
		WriteLCDCmdReg(addr, INCREMENT_CURSOR1);
		break;
	case decrementCursor:
		WriteLCDCmdReg(addr, DECREMENT_CURSOR);
		break;
	case decrementCursorShiftDisp:
		WriteLCDCmdReg(addr, DECREMENT_CURSOR1);
		break;
	default:
		WriteLCDCmdReg(addr, INCREMENT_CURSOR);
	}
}




void GoHome(uint8_t addr)
{
	WriteLCDCmdReg(addr, RETURN_HOME);
}

void GoFirstLine(uint8_t addr)
{
	WriteLCDCmdReg(addr, MOVE_CURSOR_1STLINE);
}

void GoSecondLine(uint8_t addr)
{
	WriteLCDCmdReg(addr, MOVE_CURSOR_2NDLINE);
}

void SwitchLEDOn(uint8_t addr)
{
	Writei2c(addr, (1<<LED));
}

void ClearDisplay16x2(uint8_t addr)
{
	uint8_t X, Y = 0;

	for(Y = 0; Y < 2; Y++)
	{
		for(X = 0; X < 16; X++)
		{
			PrintLCDCharXY16x2(addr, ' ', X, Y);
		}
	}
	GoHome(addr);
}

void ClearDisplay20x4(uint8_t addr)
{
	uint8_t X, Y = 0;

		for(Y = 0; Y < 4; Y++)
		{
			for(X = 0; X < 20; X++)
			{
				PrintLCDCharXY16x2(addr, ' ', X, Y);
			}
		}
		GoHome(addr);
}

void ClearDisplay1stLine16x2(uint8_t addr)
{
	uint8_t X = 0;
	for(X = 0; X < 16; X++)
	{
		PrintLCDCharXY16x2(addr, ' ', X, 0);
	}
	GoHome(addr);
}
void ClearDisplay2ndLine16x2(uint8_t addr)
{
	uint8_t X = 0;
	for(X = 0; X < 16; X++)
	{
		PrintLCDCharXY16x2(addr, ' ', X, 1);
	}
	GoHome(addr);
}
void ClearDisplay1stLine20x4(uint8_t addr)
{
	uint8_t X = 0;
	for(X = 0; X < 20; X++)
	{
		PrintLCDCharXY20x4(addr, ' ', X, 0);
	}
	GoHome(addr);
}
void ClearDisplay2ndLine20x4(uint8_t addr)
{
	uint8_t X = 0;
	for(X = 0; X < 20; X++)
	{
		PrintLCDCharXY20x4(addr, ' ', X, 1);
	}
	GoHome(addr);
}
void ClearDisplay3rdLine20x4(uint8_t addr)
{
	uint8_t X = 0;
	for(X = 0; X < 20; X++)
	{
		PrintLCDCharXY20x4(addr, ' ', X, 2);
	}
	GoHome(addr);
}
void ClearDisplay4thLine20x4(uint8_t addr)
{
	uint8_t X = 0;
	for(X = 0; X < 20; X++)
	{
		PrintLCDCharXY20x4(addr, ' ', X, 3);
	}
	GoHome(addr);
}

void ClearDisplay(uint8_t addr)
{
	WriteLCDCmdReg(addr, CLEAR_SCREEN);
	uSDelay(2000);
}
