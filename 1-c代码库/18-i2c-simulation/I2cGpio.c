/*
## @file
#
#  Copyright (c) 2018 Loongson Technology Corporation Limited (www.loongson.cn).
#  All intellectual property rights(Copyright, Patent and Trademark) reserved.
#
#  Any violations of copyright or other intellectual property rights of the Loongson Technology
#  Corporation Limited will be held accountable in accordance with the law,
#  if you (or any of your subsidiaries, corporate affiliates or agents) initiate
#  directly or indirectly any Intellectual Property Assertion or Intellectual Property Litigation:
#  (i) against Loongson Technology Corporation Limited or any of its subsidiaries or corporate affiliates,
#  (ii) against any party if such Intellectual Property Assertion or Intellectual Property Litigation arises
#  in whole or in part from any software, technology, product or service of Loongson Technology Corporation
#  Limited or any of its subsidiaries or corporate affiliates, or (iii) against any party relating to the Software.
#
#  THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
#  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR
#  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION).
#
#
##
*/

#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

#if 0
//use for DVO1
#define SCL1    3
#define SDA1    2

//use for DVO0
#define SCL0    1
#define SDA0    0

#define LOW     0
#define HIGH    1

#define MYOUT   0
#define MYIN    1

#define CHIP_7034_ADDR         0xea
#define CHIP_9022_ADDR         0x72
#define CHIP_NCS8805_ADDR1     0xE0
#define CHIP_NCS8805_ADDR2     0xEA
#define CHIP_NCS8805_ADDR3     0x80
#define GPIO_IN_REG_OFFSET     0x1650
#define GPIO_OUT_REG_OFFSET    0x1650
#define GPIO_EN_OFFSET         0x1660
#define HT1_MEM_BASE_ADDR      0x90000e0000000000
#define HT_CONF_TYPE0_ADDR     0x90000efdfe000000
#define writel(addr,Value)     (*(volatile UINT32 *)(addr))=Value
#define READL(addr)            (*(volatile UINT32 *)(addr))
#define WRITEL(addr,Value)     (*(volatile UINT32 *)(addr))=Value

#define EDP_1024_768

UINT64 DcBase = 0;
UINT64 GpioValBase = 0;
UINT64 GpioDirBase = 0;


INTN
GpioI2cReadAck(INTN Scl, INTN Sda)
{
  SetGpioDirection(Sda, MYIN);
  gBS->Stall(100);

  SetGpioValue(Scl, HIGH);
  gBS->Stall(100);
  /* the ack signal will hold on Sda untill Scl falling edge */
  while(GetGpioValue(Sda))
    gBS->Stall(100);

  SetGpioValue(Scl, LOW);
  gBS->Stall(100);
  return 0;
}

#if 0
 VOID Gpioi2c_send_ack()
{
  SetGpioDirection(SDA, MYOUT);
  SetGpioValue(SCL, LOW);
  gBS->Stall(100);
  SetGpioValue(SDA, 0);
  gBS->Stall(100);
  SetGpioValue(SCL, HIGH);
  gBS->Stall(100);
}
#endif

INTN
GpioI2cWriteByte(
  UINT8 DevAddr,
  UINT8 c
  )
{
  INTN i;
  INTN Sda = 0, Scl = 0;

  if (DevAddr == CHIP_7034_ADDR) {
    Sda = SDA1;
    Scl = SCL1;
  }else if((DevAddr == CHIP_NCS8805_ADDR1)||
      (DevAddr == CHIP_NCS8805_ADDR2) ||
      (DevAddr == CHIP_NCS8805_ADDR3)){
    Sda = SDA1;
    Scl = SCL1;
  } else if (DevAddr == CHIP_9022_ADDR) {
    Sda = SDA0;
    Scl = SCL0;
  }

  SetGpioDirection(Sda, MYOUT);

  for(i = 7; i >= 0; i--)
  {
    SetGpioValue(Scl, LOW);
    gBS->Stall(100);
    SetGpioValue(Sda, (c & (1U << i)) ? 1 : 0);//high bit --> low bit
    gBS->Stall(100);
    SetGpioValue(Scl, HIGH);
    gBS->Stall(1);
  }

  SetGpioValue(Scl, LOW);
  gBS->Stall(100);

  if (GpioI2cReadAck(Scl, Sda))
  {
    DbgPrint(EFI_D_INFO, "read slave dev ack inValid\n");//0:Valid
    return 0;
  }
  return 1;

}

VOID
GpioI2cReadByte(
  UINT8 DevAddr,
  UINT8 *c
  )
{
  INTN i;
  INTN Sda=0, Scl=0;

  if (DevAddr == CHIP_7034_ADDR) {
    Sda = SDA1;
    Scl = SCL1;
  }else if((DevAddr == CHIP_NCS8805_ADDR1)||
      (DevAddr == CHIP_NCS8805_ADDR2) ||
      (DevAddr == CHIP_NCS8805_ADDR3)){
    Sda = SDA1;
    Scl = SCL1;
  } else if (DevAddr == CHIP_9022_ADDR) {
    Sda = SDA0;
    Scl = SCL0;
  }

  *c = 0;
  SetGpioDirection(Sda, MYIN);

  for (i = 7; i >= 0; i--)
  {
    SetGpioValue(Scl, HIGH);
    gBS->Stall(100);
    *c = (*c << 1) | GetGpioValue(Sda);
    SetGpioValue(Scl, LOW);
    gBS->Stall(100);
  }
//  Gpioi2c_send_ack();
}

VOID
GpioI2cStart(
  UINT8 DevAddr
  )
{
  INTN Sda = 0, Scl = 0;

  if (DevAddr == CHIP_7034_ADDR) {
    Sda = SDA1;
    Scl = SCL1;
  }else if((DevAddr == CHIP_NCS8805_ADDR1)||
      (DevAddr == CHIP_NCS8805_ADDR2) ||
      (DevAddr == CHIP_NCS8805_ADDR3)){
    Sda = SDA1;
    Scl = SCL1;
  } else if (DevAddr == CHIP_9022_ADDR) {
    Sda = SDA0;
    Scl = SCL0;
  }

  /* if set Sda output without setting Sda high,
   * the Sda output Value may be low
   * */
  SetGpioValue(Sda, HIGH);

  SetGpioDirection(Sda, MYOUT);
  SetGpioDirection(Scl, MYOUT);

  SetGpioValue(Sda, HIGH);
  SetGpioValue(Scl, HIGH);
  gBS->Stall(100);

  /* start signal: Sda from high to low */
  SetGpioValue(Sda, HIGH);
  gBS->Stall(100);
  SetGpioValue(Sda, LOW);
  gBS->Stall(100);
}

VOID
GpioI2cStop(
  UINT8 DevAddr
  )
{
  INTN Sda = 0, Scl = 0;

  if (DevAddr == CHIP_7034_ADDR) {
    Sda = SDA1;
    Scl = SCL1;
  }else if((DevAddr == CHIP_NCS8805_ADDR1)||
      (DevAddr == CHIP_NCS8805_ADDR2) ||
      (DevAddr == CHIP_NCS8805_ADDR3)){
    Sda = SDA1;
    Scl = SCL1;
  } else if (DevAddr == CHIP_9022_ADDR) {
    Sda = SDA0;
    Scl = SCL0;
  }

  SetGpioDirection(Sda, MYOUT);

  SetGpioValue(Scl, HIGH);
  gBS->Stall(100);

  SetGpioValue(Sda, LOW);
  gBS->Stall(100);
  SetGpioValue(Sda, HIGH);
  gBS->Stall(100);
}

VOID
GpioI2cWrite(
  UINT8 DevAddr,
  UINT8 DataAddr,
  UINT8 Data
  )
{
  GpioI2cStart(DevAddr);
  if (!GpioI2cWriteByte(DevAddr, DevAddr))
  {
    DbgPrint(EFI_D_INFO, "Gpioi2c write DevAddr fail\n");
    return;
  }
  if (!GpioI2cWriteByte(DevAddr, DataAddr))
  {
    DbgPrint(EFI_D_INFO, "Gpioi2c write DataAddr fail\n");
    return;
  }
  if (!GpioI2cWriteByte(DevAddr, Data))
  {
    DbgPrint(EFI_D_INFO, "Gpioi2c write Data fail\n");
    return;
  }
  GpioI2cStop(DevAddr);
}

VOID
GpioI2cRead(
  UINT8 DevAddr,
  UINT8 DataAddr,
  UINT8 *Data
  )
{
  /* bit0 :1 read, 0 write */

  GpioI2cStart(DevAddr);

  if (!GpioI2cWriteByte(DevAddr, DevAddr))
  {
    DbgPrint(EFI_D_INFO, "Gpioi2c write DevAddr fail\n");
    return;
  }

  if (!GpioI2cWriteByte(DevAddr, DataAddr))
  {
    DbgPrint(EFI_D_INFO, "Gpioi2c write DataAddr fail\n");
    return;
  }

  GpioI2cStart(DevAddr);
  if (!GpioI2cWriteByte(DevAddr, DevAddr | 0x01))//for read
  {
    DbgPrint(EFI_D_INFO, "Gpioi2c write DevAddr fail\n");
    return;
  }

  GpioI2cReadByte(DevAddr, Data);
  GpioI2cStop(DevAddr);
}


VOID
GpioI2cConfigCh7034(
  VOID
  )
{
  INTN  Count;
  UINT8 DevAddr = CHIP_7034_ADDR;
  UINT8 DataAddr;
  UINT8 Data;

  /* get reg base addr */
  GpioI2cInit();

  for (Count = 0; Count < CH7034_REGMAP_LENGTH_VGA; Count++)
  {
    DataAddr = CH7034_VGA_REG_TABLE2[0][Count][0];
    Data = CH7034_VGA_REG_TABLE2[0][Count][1];

    GpioI2cWrite(DevAddr, DataAddr, Data);
    #if DEBUG_CH7034
    GpioI2cRead(DevAddr, DataAddr, &Data2);

    if (Data != Data2)
      DbgPrint(EFI_D_INFO, "not eq, Data 0x%2x, Data2 0x%2x\n\n\n", Data, Data2);
    #endif
  }
}

VOID
GpioI2cConfigSii9022a(
  VOID
  )
{
  UINT8 Id0;
  UINT8 Id1;
  UINT8 id2;
  UINT8 Data;

  UINT8 DevAddr = CHIP_9022_ADDR;

  /* get reg base addr */
  GpioI2cInit();

  GpioI2cWrite(DevAddr, 0xc7, 0x00);
  GpioI2cRead(DevAddr, 0x1b, &Id0);
  GpioI2cRead(DevAddr, 0x1c, &Id1);
  GpioI2cRead(DevAddr, 0x1d, &id2);

  if (Id0 != 0xb0 || Id1 != 0x2 || id2 != 0x3) {
    DbgPrint(EFI_D_ERROR, "id err\n");
    return;
  }

  GpioI2cRead(DevAddr, 0x1e, &Data);
  Data &= ~(0x3);
  GpioI2cWrite(DevAddr, 0x1e, Data);

  GpioI2cRead(DevAddr, 0x1a, &Data);
  Data &= ~(1 << 4);
  GpioI2cWrite(DevAddr, 0x1a, Data);

}
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////
UINT64 DcBase = 0;
UINT64 GpioValBase = 0;
UINT64 GpioDirBase = 0;


#define LOW            (0)
#define HIGH           (1)
#define DVO_CH0        (0)
#define DVO_CH1        (1)
#define DVO_0_SDA      (0)
#define DVO_0_SCL      (1)
#define DVO_1_SDA      (2)
#define DVO_1_SCL      (3)
#define GPIO_DIR_OUT   (0)
#define GPIO_DIR_IN    (1)

#define CHIP_7034_ADDR         0xea
#define CHIP_9022_ADDR         0x72
#define CHIP_NCS8805_ADDR1     0xE0
#define CHIP_NCS8805_ADDR2     0xEA
#define CHIP_NCS8805_ADDR3     0x80
#define GPIO_IN_REG_OFFSET     0x1650
#define GPIO_OUT_REG_OFFSET    0x1650
#define GPIO_EN_OFFSET         0x1660
#define HT1_MEM_BASE_ADDR      0x90000e0000000000
#define HT_CONF_TYPE0_ADDR     0x90000efdfe000000
#define writel(addr,Value)     (*(volatile UINT32 *)(addr))=Value
#define READL(addr)            (*(volatile UINT32 *)(addr))
#define WRITEL(addr,Value)     (*(volatile UINT32 *)(addr))=Value

/////////////////////////////////////////////////////////////////////////////
#define I2C_KHZ         (200)
#define I2C_DELAY       (gBS->Stall((1000000/(I2C_KHZ*1000)/2)))
#define I2C_DELAY_HAlF  (gBS->Stall((1000000/(I2C_KHZ*1000)/4)))
#define EDP_1024_768    // choose 1024*768 show
#define DEBUG_NCS8805   1
/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef  EDP_1024_768
// use 1024*768 reg list
#define NCS8805_EDP_REG_TABLE        NCS8805_EDP_REG_TABLE_1024_768
#define NCS8805_REGMAP_LENGTH_EDP    NCS8805_REGMAP_LENGTH_EDP_1024_768

#elif defined   EDP_1920_1080
// use 1920*1080 reg list
#define NCS8805_EDP_REG_TABLE        NCS8805_EDP_REG_TABLE_1920_1080
#define NCS8805_REGMAP_LENGTH_EDP    NCS8805_REGMAP_LENGTH_EDP_1920_1080

#else
//The default 1024*768
#define NCS8805_EDP_REG_TABLE        NCS8805_EDP_REG_TABLE_1024_768
#define NCS8805_REGMAP_LENGTH_EDP    NCS8805_REGMAP_LENGTH_EDP_1024_768
#endif

////////////////////////////////////////////////////////////////////////////////

UINT8 CH7034_VGA_REG_TABLE2[][131][2] = {
  {//VGA
   //MYIN 800x600,out 800x600,ch7034,bypassmode vga,mode_idx=1
    { 0x03, 0x04 },//page 4
    { 0x52, 0xC3 },
    { 0x5A, 0x06 },
    { 0x5A, 0x04 },
    { 0x5A, 0x06 },
    { 0x52, 0xC1 },
    { 0x52, 0xC3 },
    { 0x5A, 0x04 },

    { 0x03, 0x00 },//page 1
    { 0x07, 0xD9 },
    { 0x08, 0xF1 },
    { 0x09, 0x13 },
    { 0x0A, 0xBE },
    { 0x0B, 0x23 },
    { 0x0C, 0x20 },
    { 0x0D, 0x20 },
    { 0x0E, 0x00 },
    { 0x0F, 0x28 },
    { 0x10, 0x80 },
    { 0x11, 0x12 },
    { 0x12, 0x58 },
    { 0x13, 0x74 },
    { 0x14, 0x00 },
    { 0x15, 0x01 },
    { 0x16, 0x04 },
    { 0x17, 0x00 },
    { 0x18, 0x00 },//modify 888RGB
   // { 0x18, 0x20 },//565RGB
   // { 0x18, 0x02 },//888GRB
    { 0x19, 0xF8 },//freq
    { 0x1A, 0x9B },
    { 0x1B, 0x78 },
    { 0x1C, 0x69 },
    { 0x1D, 0x78 },
    { 0x1E, 0x00 },//output is  progressive
    { 0x1F, 0x23 },
    { 0x20, 0x20 },
    { 0x21, 0x20 },
    { 0x22, 0x00 },
    { 0x23, 0x10 },
    { 0x24, 0x60 },
    { 0x25, 0x12 },
    { 0x26, 0x58 },
    { 0x27, 0x74 },
    { 0x28, 0x00 },
    { 0x29, 0x0A },
    { 0x2A, 0x02 },
    { 0x2B, 0x09 },//vga output format:bypass mode
    { 0x2C, 0x00 },
    { 0x2D, 0x00 },
    { 0x2E, 0x3D },
    { 0x2F, 0x00 },//??
    { 0x32, 0xC0 },//???
    { 0x36, 0x40 },
    { 0x38, 0x47 },
    { 0x3D, 0x86 },
    { 0x3E, 0x00 },
    { 0x40, 0x0E },
    { 0x4B, 0x40 },//pwm control
    { 0x4C, 0x40 },//lvds output channel order register
    { 0x4D, 0x80 },
    { 0x54, 0x80 },//lvds
    { 0x55, 0x28 },//lvds
    { 0x56, 0x80 },//lvds
    { 0x57, 0x00 },//lvds
    { 0x58, 0x01 },//lvds
    { 0x59, 0x04 },//lvds
    { 0x5A, 0x02 },
    { 0x5B, 0xF2 },
    { 0x5C, 0xB9 },
    { 0x5D, 0xD6 },
    { 0x5E, 0x54 },
    { 0x60, 0x00 },
    { 0x61, 0x00 },
    { 0x64, 0x2D },
    { 0x68, 0x44 },
    { 0x6A, 0x40 },
    { 0x6B, 0x00 },
    { 0x6C, 0x10 },
    { 0x6D, 0x00 },
    { 0x6E, 0xA0 },
    { 0x70, 0x98 },
    { 0x74, 0x30 },//scaling control
    { 0x75, 0x80 },//scaling control
    { 0x7E, 0x0F },//de and pwm control
    { 0x7F, 0x00 },//test pattern

    { 0x03, 0x01 },//page 2
    { 0x08, 0x05 },
    { 0x09, 0x04 },//diffen register
    { 0x0B, 0x65 },
    { 0x0C, 0x4A },
    { 0x0D, 0x29 },
    { 0x0F, 0x9C },
    { 0x12, 0xD4 },
    { 0x13, 0x28 },
    { 0x14, 0x83 },
    { 0x15, 0x00 },
    { 0x16, 0x00 },
    { 0x1A, 0x6C },//DAC termination control register
    { 0x1B, 0x00 },
    { 0x1C, 0x00 },
    { 0x1D, 0x00 },
    { 0x23, 0x63 },
    { 0x24, 0xB4 },
    { 0x28, 0x54 },
    { 0x29, 0x60 },
    { 0x41, 0x60 },//lvds
    { 0x63, 0x2D },//DE polarity
    { 0x6B, 0x11 },
    { 0x6C, 0x06 },

    { 0x03, 0x03 },//page3
    { 0x26, 0x00 },
    { 0x28, 0x08 },//output control:DAC output is VGA
    { 0x2A, 0x00 },//output control:HDTV output through scaler

    { 0x03, 0x04 },//page 4
    { 0x10, 0x00 },
    { 0x11, 0x9B },
    { 0x12, 0x78 },
    { 0x13, 0x02 },
    { 0x14, 0x88 },
    { 0x15, 0x70 },
    { 0x20, 0x00 },
    { 0x21, 0x00 },
    { 0x22, 0x00 },
    { 0x23, 0x00 },
    { 0x24, 0x00 },
    { 0x25, 0x00 },
    { 0x26, 0x00 },
    { 0x54, 0xC4 },
    { 0x55, 0x5B },
    { 0x56, 0x4D },
    { 0x60, 0x01 },
    { 0x61, 0x62 },
  },
};
#define CH7034_REGMAP_LENGTH_VGA (sizeof(CH7034_VGA_REG_TABLE2[0]) / (2*sizeof(UINT8)))


// NCS8805 EDP 1024*768 reg list
UINT8 NCS8805_EDP_REG_TABLE_1024_768[][3] = {
  {CHIP_NCS8805_ADDR1,0x0f,0x01},
  {CHIP_NCS8805_ADDR1,0x00,0x04},
  {CHIP_NCS8805_ADDR1,0x02,0x00},
  {CHIP_NCS8805_ADDR1,0x06,0x07},
  {CHIP_NCS8805_ADDR1,0x07,0xe2},
  {CHIP_NCS8805_ADDR1,0x09,0x01},
  {CHIP_NCS8805_ADDR1,0x0b,0x00},
  {CHIP_NCS8805_ADDR1,0x10,0x05},
  {CHIP_NCS8805_ADDR1,0x11,0x41},
  {CHIP_NCS8805_ADDR1,0x12,0x01},
  {CHIP_NCS8805_ADDR1,0x13,0x08},
  {CHIP_NCS8805_ADDR1,0x14,0x04},
  {CHIP_NCS8805_ADDR1,0x15,0x00},
  {CHIP_NCS8805_ADDR1,0x16,0x03},
  {CHIP_NCS8805_ADDR1,0x17,0x1b},
  {CHIP_NCS8805_ADDR1,0x18,0x00},
  {CHIP_NCS8805_ADDR1,0x19,0x19},
  {CHIP_NCS8805_ADDR1,0x1a,0x03},
  {CHIP_NCS8805_ADDR1,0x1b,0x00},
  {CHIP_NCS8805_ADDR1,0x1c,0x00},
  {CHIP_NCS8805_ADDR1,0x1d,0x68},
  {CHIP_NCS8805_ADDR1,0x1e,0x00},
  {CHIP_NCS8805_ADDR1,0x1f,0x03},
  {CHIP_NCS8805_ADDR1,0x20,0x09},
  {CHIP_NCS8805_ADDR1,0x21,0xd8},
  {CHIP_NCS8805_ADDR1,0x22,0x01},
  {CHIP_NCS8805_ADDR1,0x23,0xef},
  {CHIP_NCS8805_ADDR1,0x24,0x07},
  {CHIP_NCS8805_ADDR1,0x25,0x80},
  {CHIP_NCS8805_ADDR1,0x26,0x04},
  {CHIP_NCS8805_ADDR1,0x27,0x5e},
  {CHIP_NCS8805_ADDR1,0x28,0x00},
  {CHIP_NCS8805_ADDR1,0x29,0x23},
  {CHIP_NCS8805_ADDR1,0x2a,0x04},
  {CHIP_NCS8805_ADDR1,0x2b,0x38},
  {CHIP_NCS8805_ADDR1,0x2c,0x00},
  {CHIP_NCS8805_ADDR1,0x2d,0xc3},
  {CHIP_NCS8805_ADDR1,0x2e,0x00},
  {CHIP_NCS8805_ADDR1,0x2f,0x04},
  {CHIP_NCS8805_ADDR1,0x55,0x04},
  {CHIP_NCS8805_ADDR1,0x56,0x44},
  {CHIP_NCS8805_ADDR1,0x57,0x44},
  {CHIP_NCS8805_ADDR1,0x58,0x05},
  {CHIP_NCS8805_ADDR1,0x59,0xb0},
  {CHIP_NCS8805_ADDR1,0x5a,0x5b},
  {CHIP_NCS8805_ADDR1,0x5d,0x2a},
  {CHIP_NCS8805_ADDR1,0x5e,0xff},
  {CHIP_NCS8805_ADDR1,0x5f,0x50},
  {CHIP_NCS8805_ADDR1,0x60,0x00},
  {CHIP_NCS8805_ADDR1,0x71,0x01},
  {CHIP_NCS8805_ADDR1,0x73,0x80},
  {CHIP_NCS8805_ADDR1,0x74,0x00},

  {CHIP_NCS8805_ADDR2,0x00,0xb0},
  {CHIP_NCS8805_ADDR2,0x01,0x00},
  {CHIP_NCS8805_ADDR2,0x02,0x5c},
  {CHIP_NCS8805_ADDR2,0x0b,0x47},
  {CHIP_NCS8805_ADDR2,0x0e,0x08},
  {CHIP_NCS8805_ADDR2,0x0f,0x08},
  {CHIP_NCS8805_ADDR2,0x11,0x88},
  {CHIP_NCS8805_ADDR2,0x22,0x00},
  {CHIP_NCS8805_ADDR2,0x84,0x23},
  {CHIP_NCS8805_ADDR2,0x85,0x01},
  {CHIP_NCS8805_ADDR2,0x00,0xb1},

  {CHIP_NCS8805_ADDR1,0x0f,0x00},

};
#define NCS8805_REGMAP_LENGTH_EDP_1024_768 (sizeof(NCS8805_EDP_REG_TABLE_1024_768) / sizeof(NCS8805_EDP_REG_TABLE_1024_768[0]))


// NCS8805 EDP 1920*1080 reg list
UINT8 NCS8805_EDP_REG_TABLE_1920_1080[][3] = {
  {CHIP_NCS8805_ADDR1,0x0f,0x01},
  {CHIP_NCS8805_ADDR1,0x00,0x04},
  {CHIP_NCS8805_ADDR1,0x02,0x07},
  {CHIP_NCS8805_ADDR1,0x06,0x07},
  {CHIP_NCS8805_ADDR1,0x07,0xe2},
  {CHIP_NCS8805_ADDR1,0x09,0x01},
  {CHIP_NCS8805_ADDR1,0x0b,0x00},
  {CHIP_NCS8805_ADDR1,0x60,0x00},
  {CHIP_NCS8805_ADDR1,0x71,0x01},
  {CHIP_NCS8805_ADDR1,0x73,0x80},
  {CHIP_NCS8805_ADDR1,0x74,0x00},

  {CHIP_NCS8805_ADDR2,0x00,0xb0},
  {CHIP_NCS8805_ADDR2,0x01,0x00},
  {CHIP_NCS8805_ADDR2,0x02,0x5c},
  {CHIP_NCS8805_ADDR2,0x0b,0x47},
  {CHIP_NCS8805_ADDR2,0x0e,0x06},
  {CHIP_NCS8805_ADDR2,0x0f,0x06},
  {CHIP_NCS8805_ADDR2,0x11,0x89},
  {CHIP_NCS8805_ADDR2,0x22,0x00},
  {CHIP_NCS8805_ADDR2,0x84,0x23},
  {CHIP_NCS8805_ADDR2,0x85,0x01},

  {CHIP_NCS8805_ADDR2,0x00,0xb1},
  {CHIP_NCS8805_ADDR1,0x0f,0x00},
};
#define NCS8805_REGMAP_LENGTH_EDP_1920_1080 (sizeof(NCS8805_EDP_REG_TABLE_1920_1080) / sizeof(NCS8805_EDP_REG_TABLE_1920_1080[0]))


VOID
GpioI2cInit (
  VOID
  )
{
//  DcBase = *(volatile UINT32 *)0xba003110;
  DcBase = READL(HT_CONF_TYPE0_ADDR | 0x3110);
  DcBase &= 0xfffffff0;
//  DcBase |= 0x80000000;
  DcBase |= HT1_MEM_BASE_ADDR;

  GpioValBase = DcBase + GPIO_OUT_REG_OFFSET;
  GpioDirBase = DcBase + GPIO_EN_OFFSET;
#if 0
  DbgPrint(EFI_D_INFO, "Gpioi2c init regs base\n");
  DbgPrint(EFI_D_INFO, "GpioValBase 0x%llx\n", GpioValBase);
  DbgPrint(EFI_D_INFO, "GpioDirBase 0x%llx\n", GpioDirBase);
#endif
}

VOID
SetGpioValue(
  INTN Gpio,
  INTN Val
  )
{
  if (Val == 1)
    READL(GpioValBase) |= (1U << Gpio);
  else
    READL(GpioValBase) &= ~(1U << Gpio);
}

UINT32
GetGpioValue(
  INTN Gpio
  )
{
  return (READL(GpioValBase) & (1U << Gpio)) ? 1 : 0;
}

VOID
SetGpioDirection(
  UINT32 Gpio,
  INTN Val
  )
{
  if (Val == 1)
    READL(GpioDirBase) |= (1 << Gpio);
  else
    READL(GpioDirBase) &= ~(1 << Gpio);
}

////////////////////////////////////////////////////////////////////////////////
#define SDA_SET_HIGHT(GPIO_SDA)   (SetGpioValue(GPIO_SDA, HIGH))
#define SDA_SET_LOW(GPIO_SDA)     (SetGpioValue(GPIO_SDA, LOW))
#define SDA_SET_OUT(GPIO_SDA)     (SetGpioDirection(GPIO_SDA, GPIO_DIR_OUT))
#define SDA_SET_IN(GPIO_SDA)      (SetGpioDirection(GPIO_SDA, GPIO_DIR_IN))
#define SDA_GET_VAL(GPIO_SDA)     (GetGpioValue(GPIO_SDA))

#define SCL_SET_HIGHT(GPIO_SCL)   (SetGpioValue(GPIO_SCL, HIGH))
#define SCL_SET_LOW(GPIO_SCL)     (SetGpioValue(GPIO_SCL, LOW))
#define SCL_SET_OUT(GPIO_SCL)     (SetGpioDirection(GPIO_SCL, GPIO_DIR_OUT))
#define SCL_SET_IN(GPIO_SCL)      (SetGpioDirection(GPIO_SCL, GPIO_DIR_IN))
#define SCL_GET_VAL(GPIO_SCL)     (GetGpioValue(GPIO_SCL))


#define DVO_CH_SELECT_CODE(DVO_CH,SCL,SDA)  \
  if(DVO_CH == DVO_CH0){            \
    SCL = DVO_0_SCL; SDA = DVO_0_SDA;   \
  }else if(DVO_CH == DVO_CH1){      \
    SCL = DVO_1_SCL; SDA = DVO_1_SDA;   \
  }


VOID I2CSend_Data(UINT8 DvoCh, UINT8 DevAddr,UINT8 RegAddr,UINT8 Data);
VOID I2CSend_Data_Block(UINT8 DvoCh, UINT8 DevAddr,UINT8 *Data,UINT8 Size);
UINT8 I2CReceive_Data(UINT8 DvoCh, UINT8 DevAddr,UINT8 RegAddr);
VOID I2CReceive_Data_Block(UINT8 DvoCh, UINT8 DevAddr, UINT8 RegAddr, UINT8 *RecvDate, UINT8 Size);

/**I2CStart**/
VOID I2CStart(UINT8 DvoCh)
{

  UINT8 SCL = 0, SDA = 0;
  DVO_CH_SELECT_CODE(DvoCh,SCL,SDA);

  SDA_SET_OUT(SDA);
  SCL_SET_OUT(SCL);
  I2C_DELAY;

  SCL_SET_HIGHT(SCL);
  SDA_SET_HIGHT(SDA);
  I2C_DELAY;

  SDA_SET_LOW(SDA);
  I2C_DELAY;

  SCL_SET_LOW(SCL);
}


/**I2CSendByte**/
VOID I2CSendByte(UINT8 DvoCh, UINT8 Data)
{
  UINT8 i;
  UINT8 SCL = 0, SDA = 0;
  DVO_CH_SELECT_CODE(DvoCh,SCL,SDA);

  SDA_SET_OUT(SDA);

  for(i=0; i<8; i++){
    I2C_DELAY_HAlF;
    if((Data >> 7) & 0x01){
      SDA_SET_HIGHT(SDA);
    }else{
      SDA_SET_LOW(SDA);
    }
    I2C_DELAY_HAlF;

    SCL_SET_HIGHT(SCL);
    I2C_DELAY;

    Data <<= 1;
    SCL_SET_LOW(SCL);
  }
}

/**I2CReceiveACK**/
VOID I2CReceiveACK(UINT8 DvoCh)
{
  UINT8 Val,i;
  UINT8 SCL = 0, SDA = 0;
  DVO_CH_SELECT_CODE(DvoCh,SCL,SDA);

  SDA_SET_IN(SDA);
  I2C_DELAY;

  SCL_SET_HIGHT(SCL);
  for(i=0; i < 3; i++){
    I2C_DELAY_HAlF;
    Val = SDA_GET_VAL(SDA);
    if(!Val){
      break;
    }
    I2C_DELAY_HAlF;
  }
  SCL_SET_LOW(SCL);
}

/**I2CStop**/
VOID I2CStop(UINT8 DvoCh)
{
  UINT8 SCL = 0, SDA = 0;
  DVO_CH_SELECT_CODE(DvoCh,SCL,SDA);

  SDA_SET_OUT(SDA);

  SDA_SET_LOW(SDA);
  I2C_DELAY;

  SCL_SET_HIGHT(SCL);
  I2C_DELAY_HAlF;

  SDA_SET_HIGHT(SDA);
}

/**I2CReceiveByte**/
UINT8 I2CReceiveByte(UINT8 DvoCh)
{
  UINT8 i;
  UINT8 Data = 0;
  UINT8 SCL = 0, SDA = 0;
  DVO_CH_SELECT_CODE(DvoCh,SCL,SDA);

  SDA_SET_IN(SDA);

  for(i = 0;i < 8;i++){
    I2C_DELAY;
    SCL_SET_HIGHT(SCL);

    I2C_DELAY_HAlF;
    Data = ((Data << 1) | SDA_GET_VAL(SDA));

    I2C_DELAY_HAlF;
    SCL_SET_LOW(SCL);
  }
  return(Data);
}

/**I2CSendACK**/
VOID I2CSendACK(UINT8 DvoCh)
{
  UINT8 SCL = 0,SDA = 0;
  DVO_CH_SELECT_CODE(DvoCh,SCL,SDA);

  SDA_SET_OUT(SDA);

  I2C_DELAY_HAlF;
  SDA_SET_LOW(SDA);

  I2C_DELAY_HAlF;
  SCL_SET_HIGHT(SCL);

  I2C_DELAY;
  SCL_SET_LOW(SCL);
}

/**I2CSendNACK**/
VOID I2CSendNACK(UINT8 DvoCh)
{
  UINT8 SCL = 0, SDA = 0;
  DVO_CH_SELECT_CODE(DvoCh,SCL,SDA);

  SDA_SET_OUT(SDA);

  I2C_DELAY_HAlF;
  SDA_SET_HIGHT(SDA);

  I2C_DELAY_HAlF;
  SCL_SET_HIGHT(SCL);

  I2C_DELAY;
  SCL_SET_LOW(SCL);
}

VOID I2CSend_Data(UINT8 DvoCh, UINT8 DevAddr,UINT8 RegAddr,UINT8 Data)
{
  I2CStart(DvoCh);
  I2CSendByte(DvoCh, DevAddr);
  I2CReceiveACK(DvoCh);

  I2CSendByte(DvoCh, RegAddr);
  I2CReceiveACK(DvoCh);

  I2CSendByte(DvoCh,Data);
  I2CReceiveACK(DvoCh);

  I2CStop(DvoCh);
}

VOID I2CSend_Data_Block(UINT8 DvoCh, UINT8 DevAddr,UINT8 *Data,UINT8 Size)
{
  UINT8 Count;

  I2CStart(DvoCh);
  I2CSendByte(DvoCh, DevAddr);
  I2CReceiveACK(DvoCh);

  for(Count = 0;Count < Size; Count++){
    I2CSendByte(DvoCh, Data[Count]);
    I2CReceiveACK(DvoCh);
  }

  I2CStop(DvoCh);
}

UINT8 I2CReceive_Data(UINT8 DvoCh, UINT8 DevAddr,UINT8 RegAddr)
{
  UINT8 RecvDate;

  //启动送地址
  I2CStart(DvoCh);
  I2CSendByte(DvoCh,DevAddr);
  I2CReceiveACK(DvoCh);

  I2CSendByte(DvoCh, RegAddr);
  I2CReceiveACK(DvoCh);

  //启动改为读操作
  I2CStart(DvoCh);
  I2CSendByte(DvoCh, DevAddr| 0x01);
  I2CReceiveACK(DvoCh);

  RecvDate = I2CReceiveByte(DvoCh);
  I2CSendNACK(DvoCh);
  I2CStop(DvoCh);

  return RecvDate;
}

VOID I2CReceive_Data_Block(UINT8 DvoCh, UINT8 DevAddr, UINT8 RegAddr, UINT8 *RecvDate, UINT8 Size)
{
  UINT8 Count;

  I2CStart(DvoCh);
  I2CSendByte(DvoCh,DevAddr);
  I2CReceiveACK(DvoCh);

  I2CSendByte(DvoCh, RegAddr);
  I2CReceiveACK(DvoCh);

  I2CStart(DvoCh);
  I2CSendByte(DvoCh, DevAddr| 0x01);
  I2CReceiveACK(DvoCh);

  for(Count = 0;Count < Size; Count++){
    RecvDate[Count] = I2CReceiveByte(DvoCh);
    if(Count == Size -1){
      I2CSendNACK(DvoCh);
    }else{
      I2CSendACK(DvoCh);
    }
  }

  I2CStop(DvoCh);
}

////////////////////////////////////////////////////////////////////
/**
 *  Read the Edid of Ncs8805.
 *
 *  @param  EdidDataBlock         The Buf of Edid.
 *
 *  @retval TRUE                   Success.
 *  @retval FALSE                  Read false.
 *
 **/

BOOLEAN
ReadEdidDataNcs8805 (
UINT8 *EdidDataBlock)
{
  UINT8 CountPer = 16;
  UINT8 Sum = 0;
  UINT8 Count;
  UINT8 SendData[]={0x10, 0x00, 0x00, 0x01};

  SendData[2] = CountPer;

  for(Count = 0; Count < 128; ){
    I2CSend_Data_Block(DVO_CH1, 0x80, SendData, 4);
    gBS->Stall(500);

    if(0xFF != I2CReceive_Data(DVO_CH1, 0x80, 0x13)){
      continue;
    }
    gBS->Stall(100);

    I2CReceive_Data_Block(DVO_CH1, 0x80, 0x30, EdidDataBlock+Count, CountPer);
    gBS->Stall(100);
    Count += CountPer, SendData[1] += CountPer;
  }

  for(Count = 0; Count < 128; Count++){
    Sum += EdidDataBlock[Count];
  }
  if(Sum){
    return FALSE;
  }else{
    return TRUE;
  }
}

VOID
GpioI2cConfigNcs8805(
VOID
)
{
  UINTN Count;
  UINT8 DevAddr;
  UINT8 RegAddr;
  UINT8 Data;
  UINT8 Data2;
  UINT8 KeepAlive;

  /* get reg base addr */
  GpioI2cInit();

  for (Count = 0; Count < NCS8805_REGMAP_LENGTH_EDP; Count++){
    DevAddr  = NCS8805_EDP_REG_TABLE[Count][0];
    RegAddr  = NCS8805_EDP_REG_TABLE[Count][1];
    Data     = NCS8805_EDP_REG_TABLE[Count][2];
    KeepAlive = 0;
    do{
      I2CSend_Data(DVO_CH1, DevAddr, RegAddr, Data);
      Data2 = I2CReceive_Data(DVO_CH1, DevAddr, RegAddr);
      KeepAlive++;
#if DEBUG_NCS8805
      if(Data2 == Data){
               DbgPrint(EFI_D_INFO, "RedAddr=0x%02x,SetVal=0x%02x,GetVal=0x%02x.\n", RegAddr, Data, Data2);
      }else{
               DbgPrint(EFI_D_INFO, "RedAddr=0x%02x,SetVal=0x%02x,GetVal=0x%02x. not eq.\n", RegAddr, Data, Data2);
      }
#endif
    }while((Data2 != Data) && (KeepAlive < 3));
  }
}



VOID
GpioI2cConfigCh7034(
  VOID
  )
{
  INTN  Count;
  UINT8 DevAddr = CHIP_7034_ADDR;
  UINT8 DataAddr;
  UINT8 Data;

  /* get reg base addr */
  GpioI2cInit();

  for (Count = 0; Count < CH7034_REGMAP_LENGTH_VGA; Count++)
  {
    DataAddr = CH7034_VGA_REG_TABLE2[0][Count][0];
    Data = CH7034_VGA_REG_TABLE2[0][Count][1];

  I2CSend_Data(DVO_CH1, DevAddr, DataAddr, Data);
#if DEBUG_CH7034
  Data2 = I2CReceive_Data(DVO_CH1, DevAddr,DataAddr);

    if (Data != Data2)
      DbgPrint(EFI_D_INFO, "not eq, Data 0x%2x, Data2 0x%2x\n\n\n", Data, Data2);
#endif
  }
}



VOID
GpioI2cConfigSii9022a(
  VOID
  )
{
  UINT8 Id0;
  UINT8 Id1;
  UINT8 Id2;
  UINT8 Data;

  UINT8 DevAddr = CHIP_9022_ADDR;

  /* get reg base addr */
  GpioI2cInit();

  I2CSend_Data(DVO_CH0, DevAddr, 0xc7,0x00);
  Id0 = I2CReceive_Data(DVO_CH0, DevAddr, 0x1b);
  Id1 = I2CReceive_Data(DVO_CH0, DevAddr, 0x1c);
  Id2 = I2CReceive_Data(DVO_CH0, DevAddr, 0x1d);

  if (Id0 != 0xb0 || Id1 != 0x2 || Id2 != 0x3) {
    DbgPrint(EFI_D_ERROR, "id err\n");
    return;
  }

  Data = I2CReceive_Data(DVO_CH0, DevAddr, 0x1e);
  Data &= ~(0x3);
  I2CSend_Data(DVO_CH0, DevAddr, 0x1e, Data);

  Data = I2CReceive_Data(DVO_CH0, DevAddr, 0x1a);
  Data &= ~(1 << 4);
  I2CSend_Data(DVO_CH0, DevAddr, 0x1a, Data);

}
