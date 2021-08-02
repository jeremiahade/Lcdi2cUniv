/*******************************************************************
**File Name: xxxxxxxxxxxx.h/c                                     **
**Library Name: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx.          **
**Original Project Name: Null.                                    **
**Author Name: Jeremiah A.                                        **
**Version No:  2.0                                                **
**Date Created: 11th May 2020.                                    **
**Last Edited: 29th August 2020.                                  **
********************************************************************/

#ifndef LCDI2C_H_
#define LCDI2C_H_

#include "hwinterface.h"
#include <stdint.h>

#define CLEAR_SCREEN		0x01
#define	RETURN_HOME			0x02
#define DECREMENT_CURSOR	0x04
#define DECREMENT_CURSOR1	0x05
#define	INCREMENT_CURSOR	0x06
#define	INCREMENT_CURSOR1	0x07
#define	DISPOFF_CURSOROFF	0x08
#define DISPOFF_CURSORON	0x0A
#define DISPON_CURSOROFF	0x0C
#define DISPON_CURSORON		0x0E
#define DISPON_CURSORBLINK	0x0F
#define SHIFT_CURSOR_LEFT	0x10
#define SHIFT_CURSOR_RIGHT	0x14
#define SHIFT_DISP_LEFT		0x18
#define SHIFT_DISP_RIGHT	0x1C
#define MOVE_CURSOR_1STLINE	0x80
#define	MOVE_CURSOR_2NDLINE	0xC0
#define	TWOLINES_5x8MATRX	0x28
#define TWOLINES_5x10MATRX	0x2C
#define ONELINE_5x8MATRX	0x20
#define ONELINE_5x10MATRX	0x24
#define RS					0x00
#define R_W					0x01
#define EN					0x02
#define LED					0x03
#define INIT_CMD			0x33
#define INIT_CMD1			0x32

//Overflow definition
#define LCD16x2XMAX			16
#define LCD16x2YMAX2LINE	2
#define LCD16x2YMAX1LINE	1
#define LCD20x4XMAX			20
#define LCD20x4YMAX2LINE	4
#define LCD20x4YMAX1LINE	1
#define LCD16x2XMIN			0
#define LCD20x4XMIN			0


typedef enum LCD_TYPE{
	LCD16x2,
	LCD20x4
}lcdType_t;

typedef enum LINE_MATRX{
	twolines_5x8Matrix,
	twolines_5x10Matrix,
	oneline_5x8Matrix,
	oneline_5x10Matrix
}lineMatrx_t;

typedef enum DISP_CURSOR_ST{
	dispOff_CursorOff,
	dispOff_CursorOn,
	dispOn_Cursoroff,
	dispOn_CursorOn,
	dispOn_CursorBlink
}dispCursor_t;

typedef enum CURSOR_MOVEMENT{
	decrementCursor,
	decrementCursorShiftDisp,
	incrementCursor,
	incrementCursorShiftDisp
}cursrMovment_t;

typedef enum DATA_TYPE{
	integer,
	string,
	floating,
}dataType_t;


typedef struct LCD_MODULE lcdmodule_t;

struct LCD_MODULE{
 uint8_t		addr;
 lcdType_t		type;
 lineMatrx_t	lineMatrixSel;
 dispCursor_t	dispCursorSt;
 cursrMovment_t cursrMovementSel;
};

#endif /* LCDI2C_H_ */
