#include "bsp_internalFlash.h"
#include "stm32f4xx_flash.h"

#define FLASH_KEY1		((uint32_t)0x45670123)
#define FLASH_KEY2		((uint32_t)0xCDEF89AB)

/*testing Data*/
#define DATA_32 		((uint32_t)0x00000000)
/* initial erasing address */
#define FLASH_USER_START_ADDR ADDR_FLASH_SECTOR_8
/* ending erasing address */
#define FLASH_USER_END_ADDR ADDR_FLASH_SECTOR_12


/**
* @brief Unlocks the FLASH control register access 
* @param None
* @retval None
*/
void FLASH_Unlock(void)
{
	if ((FLASH->CR & FLASH_CR_LOCK) != RESET) {
		/* Authorize the FLASH Registers access */
		FLASH->KEYR = FLASH_KEY1;
		FLASH->KEYR = FLASH_KEY2;
	}
}

/**
* @brief Locks the FLASH control register access 
* @param None
* @retval None
*/
void FLASH_Lock(void)
{
	/* Set the LOCK Bit to lock the FLASH Registers access */
	FLASH->CR |= FLASH_CR_LOCK;
}

/**
*@ brief Erases a specified FLASH Sector
*@ param FLASH_Sector: The Sector number to be erased 
*@ param VoltageRange: The device voltage range which define the erase parallelism.
*
*@ retval FLASH Status: The returned value can be: FLASH_BUSY, FLASH_ERROR_PROGRAM,FLASH_ERROR_WRP, 
						FLASH_ERROR_OPERATION or FLASH_COMPLETE.
*/
FLASH_Status FLASH_EraseSector(uint32_t FLASH_Sector, uint8_t VoltageRange)
{
	uint32_t tmp_psize = 0x0; 
	FLASH_Status status = FLASH_COMPLETE;
	
	/* Check the parameters */
	assert_param(IS_FLASH_SECTOR(FLASH_Sector)); 
	assert_param(IS_VOLTAGERANGE(VoltageRange));
	
	if (VoltageRange == VoltageRange_1) { 
		tmp_psize = FLASH_PSIZE_BYTE;
	}
	else if (VoltageRange == VoltageRange_2) { 
		tmp_psize = FLASH_PSIZE_HALF_WORD;
	} 
	else if (VoltageRange == VoltageRange_3) { 
		tmp_psize = FLASH_PSIZE_WORD;
	}
	else{
		tmp_psize = FLASH_PSIZE_DOUBLE_WORD;	
	}

	/* Wait for last operation to be completed */
	status = FLASH_WaitForLastOperation();
	
	if (status == FLASH_COMPLETE) {
		/* if the previous operation is completed, proceed to erase the sector */
		FLASH->CR &= CR_PSIZE_MASK; 
		FLASH->CR |= tmp_psize; 
		FLASH->CR &= SECTOR_MASK;
		FLASH->CR |= FLASH_CR_SER | FLASH_Sector;
		FLASH->CR |= FLASH_CR_STRT;

		/* Wait for last operation to be completed */
		status = FLASH_WaitForLastOperation();

		/* if the erase operation is completed, disable the SER Bit */
		FLASH->CR &= (~FLASH_CR_SER);
		FLASH->CR &= SECTOR_MASK;
	}

	/* Return the Erase Status */
	return status;
}

/**
* @brief Programs a word(32-bit) at a specified address.
* @param Address : specifies the address to be programmed.
*					This parameter can be any address in Program memory zone or in OTP zone.
* @param Data: specifies the data to be programmed.
* @retval FLASH Status: The returned value can be: FLASH_BUSY, FLASH_ERROR_PROGRAM, 
						FLASH_ERROR_WRP, FLASH_ERROR_OPERATION or FLASH_COMPLETE.
*/
FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data)
{
	FLASH_Status status = FLASH_COMPLETE; 19
	/* Check the parameters */
	assert_param(IS_FLASH_ADDRESS(Address));

	/* Wait for last operation to be completed */
	status = FLASH_WaitForLastOperation();

	if (status == FLASH_COMPLETE) {
		/* if the previous operation is completed, proceed to program the new data */
		FLASH->CR &= CR_PSIZE_MASK;
		FLASH->CR |= FLASH_PSIZE_WORD;
		FLASH->CR |= FLASH_CR_PG;

		*(__IO uint32_t*)Address = Data; 33
		/* Wait for last operation to be completed */
		status = FLASH_WaitForLastOperation();
		/* if the program operation is completed, disable the PG Bit */
		FLASH->CR &= (~FLASH_CR_PG);
		FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data)
		FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data)
	}
	/* Return the Program Status */
	return status;
}

/**
* @ brief return sector according to address
* @ param Address
* @ retval Sector
*/
static uint32_t GetSector(uint32_t Address)
{
	uint32_t sector = 0;

	if ((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0)) {
		sector = FLASH_SECTOR_0;
	}
	else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1)) {
		sector = FLASH_Sector_1;
	}
	else if((Address < ADDR_FLASH_SECTOR_23) && (Address >= ADDR_FLASH_SECTOR_22)) {
		sector = FLASH_Sector_22;
	}
	else{
		sector = FLASH_Sector_23;
	}

	return sector;
}

/**
* @brief Internalflash_test test read and write
* @param None
* @retval None
*/
int InternalFlash_Test(void)
{
	/*specifies which sector we want to erase*/
	uint32_t uwStartSector = 0;
	uint32_t uwEndSector = 0;

	uint32_t uwAddress = 0;
	uint32_t uwSectorCounter = 0;

	__IO uint32_t uwData32 = 0;
	__IO uint32_t uwMemoryProgramStatus = 0;
	
	/* Unlock Flash*/
	FLASH_Unlock();
	/* Erase user sector*/
	/* Clean Flash Flag*/
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
		FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	
	uwStartSector = GetSector(FLASH_USER_START_ADDR); 
	uwEndSector = GetSector(FLASH_USER_END_ADDR);
	
	/* Starting Erasing */
	uwSectorCounter = uwStartSector;
	while (uwSectorCounter <= uwEndSector) {
		/* VoltageRange_3 Process with word*/
		if (FLASH_EraseSector(uwSectorCounter, VoltageRange_3) != FLASH_COMPLETE){
			/*Error return -1*/
			return -1; 
		}
		
		/* Counter point to next sector */
		if (uwSectorCounter == FLASH_Sector_11) {
			uwSectorCounter += 40; 
		} 
		
		else {
			uwSectorCounter += 8; 
		}
	}
	/* Write data in words*/ 
	uwAddress = FLASH_USER_START_ADDR;

	while (uwAddress < FLASH_USER_END_ADDR) {
		if (FLASH_ProgramWord(uwAddress, DATA_32) == FLASH_COMPLETE) {
			uwAddress = uwAddress + 4; 
		} 
		else {
		/*Error return -1*/
			return -1; 
		}
	}
	/*Lock flash*/ 
	FLASH_Lock();
	/* Testing data from Flash*/ 
	/* MemoryProgramStatus = 0: Correct Data
	MemoryProgramStatus != 0: Error Data return the number of errors*/ 
	uwAddress = FLASH_USER_START_ADDR;
	uwMemoryProgramStatus = 0;

	while (uwAddress < FLASH_USER_END_ADDR) { 
		uwData32 = *(__IO uint32_t*) uwAddress;
		if (uwData32 != DATA_32) { uwMemoryProgramStatus++;
		}
		uwAddress = uwAddress + 4; 
	}
	/* Error return */
	if (uwMemoryProgramStatus) { 
		return -1;
	} 	
	else 
	{ 
	/*Correct return*/ 
		return 0;
	}
}