/*
 * magneto.h
 *
 * Created: 25-5-2020 20:31:51
 *  Author: Wouter
 */ 

//LSM303D
#ifndef MAGNETO_H_
#define MAGNETO_H_

#include <avr/io.h>
#include <util/delay.h>

#include "twi.h"

#define MAGNETO_ADDR 0x3B

/*LSM303D Registers*/
#define TEMP_OUT_L		0x05	//r

#define TEMP_OUT_H		0x06	//r

#define STATUS_M		0x07	//r
#define TEMOR		7
#define ZMOR		6
#define YMOR		5
#define XMOR		4
#define TEMPDA		3
#define ZMDA		2
#define YMDA		1
#define XMDA		0

#define OUT_X_L_M		0x08	//r

#define OUT_X_H_M		0x09	//r

#define OUT_Y_L_M		0x0A	//r

#define OUT_Y_H_M		0x0B	//r

#define OUT_Z_L_M		0x0C	//r

#define OUT_Z_H_M		0x0D	//r

#define WHO_AM_I		0x0F	//r

#define INT_CTRL_M		0x12	//rw
#define XMIEN		7
#define YMIEN		6
#define ZMIEN		5
#define PP_OD		4
#define MIEA		3
#define MIEL		2
#define _4D			1 
#define MIEN		0

#define INT_CRC_M		0x13	//w
#define M_PTH_X		7
#define M_PTH_Y		6
#define M_PTH_Z		5
#define M_NTH_X		4
#define M_NTH_Y		3
#define M_NTH_Z		2
#define MROI		1
#define MINT		0

#define INT_THS_L_M		0x14	//rw
#define THS07		7
#define THS06		6
#define THS05		5
#define THS04		4
#define THS03		3
#define THS02		2
#define THS01		1
#define THS00		0

#define INT_THIS_H_M	0x15	//rw
#define THS16		6
#define THS15		5
#define THS14		4
#define THS13		3
#define THS12		2
#define THS11		1
#define THS10		0

#define OFFSET_X_L_M	0x16	//rw
#define OFF_X07		7
#define OFF_X06		6
#define OFF_X05		5
#define OFF_X04		4
#define OFF_X03		3
#define OFF_x02		2
#define OFF_X01		1
#define OFF_X00		0

#define OFFSET_X_H_M	0x17	//rw
#define OFF_X17		7
#define OFF_X16		6
#define OFF_X15		5
#define OFF_X14		4
#define OFF_X13		3
#define OFF_X12		2
#define OFF_X11		1
#define OFF_X10		0

#define OFFSET_Y_L_M	0x18	//rw
#define OFF_Y07		7
#define OFF_Y06		6
#define OFF_Y05		5
#define OFF_Y04		4
#define OFF_Y03		3
#define OFF_Y02		2
#define OFF_Y01		1
#define OFF_Y00		0

#define OFFSET_Y_H_M	0x19	//rw
#define OFF_Y17		7
#define OFF_Y16		6
#define OFF_Y15		5
#define OFF_Y14		4
#define OFF_Y13		3
#define OFF_Y12		2
#define OFF_Y11		1
#define OFF_Y10		0

#define OFFSET_Z_L_M	0x1A	//rw
#define OFF_Z07		7
#define OFF_Z06		6
#define OFF_Z05		5
#define OFF_Z04		4
#define OFF_Z03		3
#define OFF_Z02		2
#define OFF_Z01		1
#define OFF_Z00		0

#define OFFSET_Z_H_M	0x1B	//rw
#define OFF_Z17		7
#define OFF_Z16		6
#define OFF_Z15		5
#define OFF_Z14		4
#define OFF_Z13		3
#define OFF_Z12		2
#define OFF_Z11		1
#define OFF_Z10		0

#define REFERECNE_X		0x1C	//rw

#define REFERENCE_Y		0x1D	//rw

#define REFERENCE_Z		0x1E	//rw

#define CTRL0			0x1F	//rw
#define BOOT		7
#define FIFO_EN		6
#define FTH_EN		5
#define HP_CLICK	2
#define HPIS1		1
#define HPIS2		0

#define CTRL1			0x20	//rw
#define AODR3		7		//Table 36 of Datasheet
#define AODR2		6
#define AODR1		5
#define AODR0		4
#define BDU			3
#define AZEN		2
#define AYEN		1
#define AXEN		0

#define CTRL2			0x21	//rw
#define ABW1		7
#define ABW0		6
#define AFS2		5
#define AFS1		4
#define AFS0		3
#define AST			1
#define SIM			0

#define CTRL3			0x22	//rw
#define INT1_BOOT	7
#define INT1_CLICK	6
#define INT1_IG1	5
#define INT1_IG2	4
#define INT1_IGM	3
#define INT1_DRDY_A	2
#define INT1_DRDY_M	1
#define INT1_EMPTY	0

#define CTRL4			0x23	//rw
#define INT2_CLICK	7
#define INT2_INT1	6
#define INT2_INT2	5
#define INT2_INTM	4
#define INT2_DRDY_A	3
#define INT2_DRDY_M	2
#define INT2_OVERRUN	1
#define INT2_FTH	0

#define CTRL5			0x24	//rw
#define TEMP_EN		7
#define M_RES1		6
#define M_RES0		5
#define M_ODR2		4		//Table 47 off Datasheet
#define M_ODR1		3
#define M_ODR0		2
#define LIR2		1
#define LIR1		0

#define CTRL6			0x25	//rw
#define MFS1		6		//Table 50 of Datasheet
#define MFS0		5

#define CTRL7			0x26	//rw
#define AHPM1		7		//Table 53 of Datasheet
#define APHM0		6
#define AFDS		5
#define T_ONLY		4
#define MLP			2
#define MD1			1		//Table 54 of Datasheet
#define	MD0			0

#define STATUS_A		0x27	//r
#define ZYXAOR		7
#define ZAOR		6
#define YAOR		5
#define XAOR		4
#define ZYXADA		3
#define ZADA		2
#define YADA		1
#define XADA		0

#define OUT_X_L_A		0x28	//r

#define OUT_X_H_A		0x29	//r

#define OUT_Y_L_A		0x2A	//r

#define OUT_Y_H_A		0x2B	//r

#define OUT_Z_L_A		0x2C	//r

#define OUT_Z_H_A		0x2D	//r

#define FIFO_CTRL		0x2E	//rw
#define FM2			7		//Table 59 of Datasheet
#define FM1			6
#define FM0			5
#define FTH4		4		
#define FTH3		3
#define FTH2		2
#define FTH1		1
#define FTH0		0

#define FIFO_CRC		0x2F	//r
#define FTH			7
#define OVRN		6
#define EMPTY		5
#define FSS4		4
#define FSS3		3
#define FSS2		2
#define FSS1		1
#define FSS0		0

#define IG_CFG1			0x30	//rw
#define AOI1		7		//Table 64 of Datasheet
#define _6D1		6
#define ZHIE_ZUPE1	5
#define ZLIE_ZDOWNE1	4
#define YHIE_YUP1	3
#define YLIE_YDOWNE1	2
#define XHIE_XUPE1	1
#define XLIE_DOWNE1	0

#define IG_SRC1			0x31	//r
#define IA1			6
#define ZH1			5
#define ZL1			4
#define YH1			3
#define YL1			2
#define XH1			1
#define XL1			0

#define IG_THS1			0x32	//rw
#define THS16		6
#define THS15		5
#define THS14		4
#define THS13		3
#define THS12		2
#define THS11		1
#define THS10		0

#define IG_DUR1			0x33	//rw
#define D16			6
#define D15			5
#define D14			4
#define D13			3
#define D12			2
#define D11			1
#define D10			0

#define IG_CFG2			0x34	//rw
#define AOI2		7		//Table 73 of Datasheet
#define _6D2		6
#define ZHIE_ZUPE2	5
#define ZLIE_ZDOWNE2	4
#define YHIE_YUP2	3
#define YLIE_YDOWNE2	2
#define XHIE_XUPE2	1
#define XLIE_DOWNE2	0

#define IG_SRC2			0x35	//r
#define IA2			6
#define ZH2			5
#define ZL2			4
#define YH2			3
#define YL2			2
#define XH2			1
#define XL2			0

#define IG_THS2			0x36	//rw
#define THS26		6
#define THS25		5
#define THS24		4
#define THS23		3
#define THS22		2
#define THS21		1
#define THS20		0

#define IG_DUR2			0x37	//rw
#define D26			6
#define D25			5
#define D24			4
#define D23			3
#define S22			2
#define S21			1
#define D20			0

#define CLICK_CFG		0x38	//rw
#define ZD			5
#define ZS			4
#define YD			3
#define YS			2
#define XD			1
#define XS			0

#define CLICK_SRC		0x39	//r
#define IAC			6
#define DCLICK		5
#define SCLICK		4
#define SIGN		3
#define Z			2
#define Y			1
#define X			0

#define CLICK_THS		0x3A	//rw
#define THSC6		6
#define THSC5		5
#define THSC4		4
#define THSC3		3
#define THSC2		2
#define THSC1		1
#define THSC0		0

#define TIME_LIMIT		0x3B	//rw
#define TLI7		7
#define TLI6		6
#define TLI5		5
#define TLI4		4
#define TLI3		3
#define TLI2		2
#define TLI1		1
#define TLI0		0

#define TIME_LATENCY	0x3C	//rw
#define TLA7		7
#define TLA6		6
#define TLA5		5
#define TLA4		4
#define TLA3		3
#define TLA2		2
#define TLA1		1
#define TLA0		0

#define TIME_WINDOW		0x3D	//rw
#define TW7			7
#define TW6			6
#define TW5			5
#define TW4			4
#define TW3			3
#define TW2			2
#define TW1			1
#define TW0			0

#define ACT_THS			0x3E	//rw
#define ACTH6		6
#define ACTH5		5
#define ACTH4		4
#define ACTH3		3
#define ACTH2		2
#define ACTH1		1
#define ACHT0		0

#define ACT_DUR			0x3F	//rw
#define ACTD7		7
#define ACTD6		6
#define ACTD5		5
#define ACTD4		4
#define ACTD3		3
#define ACTD2		2
#define ACTD1		1
#define ACTD0		0

void initMagneto(void);
uint16_t getMagnetoDataX(void);
uint16_t getMagnetoDataY(void);

#endif /* MAGNETO_H_ */