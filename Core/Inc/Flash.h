#ifndef __FLASH__
#define __FLASH__


#define FLASH_SPI_SERVICE_INFO        (4)
#define FLASH_SPI_4SERVICE_INFO       (5)
#define FLASH_SPI_CMND_SERVICE_INFO   (1)

#define FLASH_SPI_OFFSET_CMND         (0)
#define FLASH_SPI_OFFSET_ADD2         (1) // MSB
#define FLASH_SPI_OFFSET_ADD1         (2)
#define FLASH_SPI_OFFSET_ADD0         (3)
#define FLASH_SPI_OFFSET_DATA         (4)

#define FLASH_SPI_4OFFSET_ADD3        (1) // MSB
#define FLASH_SPI_4OFFSET_ADD2        (2) 
#define FLASH_SPI_4OFFSET_ADD1        (3)
#define FLASH_SPI_4OFFSET_ADD0        (4)
#define FLASH_SPI_4OFFSET_DATA        (5)




//#define NAND_DEVICE_RESET             (0xFF)
//#define NAND_READ_CHIP_ID             (0x9F)
//#define NAND_PAGE_DATA_READ           (0x13) // DUMMY PA15-8 PA7-0
//#define NAND_PAGE_PROGRAM_EXECUTE     (0x10) // DUMMY PA15-8 PA7-0
//#define NAND_RANDOM_LOAD_PROGRAM_DATA (0x84)
//#define NAND_LOAD_PROGRAM_DATA        (0x02)
//#define NAND_READ_DATA                (0x03) // DUMMY OF15-8 OF7-0 d0 d1 d2 ...
//#define NAND_RESET_DEVICE             (0xFF)
//#define NAND_READ_STATUS_REGISTER     (0x0F)
//#define NAND_WRITE_STATUS_REGISTER    (0x1F)
//#define NAND_ENABLE_WRITE             (0x06)
//#define NAND_DISABLE_WRITE            (0x04)
//#define NAND_BLOCK_ERASE              (0xD8) // DUMMY PA15-8 PA7-0
//

typedef struct {
u32 u32GetNorFlashAdd;
u32 u32SetNorFlashAdd;
u32 u32NorFlashSize;
bool bSemNorFlashWrite;
bool bSemNorFlashPause;
bool bSemNorFlashFull;
bool bSemNorFlashErase;
u8 u8NorFlashFirstBuff[2100];
u8 u8NorFlashSecondBuff[2100];
//pNorFlashBuff;
u32 u32NorFlashFrame;
u32 u32SemNorReadTime;
u16 u16CountTimeNextErase;
} NOR_STRUCT;


#define SZ_NOR_FLASH_PAGE 256

#endif