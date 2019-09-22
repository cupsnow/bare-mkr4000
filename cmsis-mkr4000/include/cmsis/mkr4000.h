#ifndef _H_TEST26_CMSIS_MKR4000
#define _H_TEST26_CMSIS_MKR4000

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup TEST26
 * @{
 */

/** @addtogroup MKR4000
 * @{
 */

/** @addtogroup Configuration_of_CMSIS
 * @{
 */

typedef enum IRQn {
	Reset_IRQn = -15, /*!< -15 Reset Vector, invoked on Power up and warm reset */
	NonMaskableInt_IRQn = -14, /*!< -14 Non maskable Interrupt, cannot be stopped or preempted */
	HardFault_IRQn = -13, /*!< -13 Hard Fault, all classes of Fault */
	SVCall_IRQn = -5, /*!< -5 System Service Call via SVC instruction */
	PendSV_IRQn = -2, /*!< -2 Pendable request for system service */
	SysTick_IRQn = -1, /*!< -1 System Tick Timer */

	PM_IRQn = 0, /**<  0 SAMD21G18A Power Manager (PM) */
	SYSCTRL_IRQn = 1, /**<  1 SAMD21G18A System Control (SYSCTRL) */
	WDT_IRQn = 2, /**<  2 SAMD21G18A Watchdog Timer (WDT) */
	RTC_IRQn = 3, /**<  3 SAMD21G18A Real-Time Counter (RTC) */
	EIC_IRQn = 4, /**<  4 SAMD21G18A External Interrupt Controller (EIC) */
	NVMCTRL_IRQn = 5, /**<  5 SAMD21G18A Non-Volatile Memory Controller (NVMCTRL) */
	DMAC_IRQn = 6, /**<  6 SAMD21G18A Direct Memory Access Controller (DMAC) */
	USB_IRQn = 7, /**<  7 SAMD21G18A Universal Serial Bus (USB) */
	EVSYS_IRQn = 8, /**<  8 SAMD21G18A Event System Interface (EVSYS) */
	SERCOM0_IRQn = 9, /**<  9 SAMD21G18A Serial Communication Interface 0 (SERCOM0) */
	SERCOM1_IRQn = 10, /**< 10 SAMD21G18A Serial Communication Interface 1 (SERCOM1) */
	SERCOM2_IRQn = 11, /**< 11 SAMD21G18A Serial Communication Interface 2 (SERCOM2) */
	SERCOM3_IRQn = 12, /**< 12 SAMD21G18A Serial Communication Interface 3 (SERCOM3) */
	SERCOM4_IRQn = 13, /**< 13 SAMD21G18A Serial Communication Interface 4 (SERCOM4) */
	SERCOM5_IRQn = 14, /**< 14 SAMD21G18A Serial Communication Interface 5 (SERCOM5) */
	TCC0_IRQn = 15, /**< 15 SAMD21G18A Timer Counter Control 0 (TCC0) */
	TCC1_IRQn = 16, /**< 16 SAMD21G18A Timer Counter Control 1 (TCC1) */
	TCC2_IRQn = 17, /**< 17 SAMD21G18A Timer Counter Control 2 (TCC2) */
	TC3_IRQn = 18, /**< 18 SAMD21G18A Basic Timer Counter 3 (TC3) */
	TC4_IRQn = 19, /**< 19 SAMD21G18A Basic Timer Counter 4 (TC4) */
	TC5_IRQn = 20, /**< 20 SAMD21G18A Basic Timer Counter 5 (TC5) */
	ADC_IRQn = 23, /**< 23 SAMD21G18A Analog Digital Converter (ADC) */
	AC_IRQn = 24, /**< 24 SAMD21G18A Analog Comparators (AC) */
	DAC_IRQn = 25, /**< 25 SAMD21G18A Digital Analog Converter (DAC) */
	PTC_IRQn = 26, /**< 26 SAMD21G18A Peripheral Touch Controller (PTC) */
	I2S_IRQn = 27, /**< 27 SAMD21G18A Inter-IC Sound Interface (I2S) */

	PERIPH_COUNT_IRQn = 28 /**< Number of peripheral IDs */
} IRQn_Type;

#define __CM0PLUS_REV 0x0001 /*!< Core Revision r0p1 */
#define __MPU_PRESENT 0 /*!< Set to 1 if MPU is present */
#define __VTOR_PRESENT 1 /*!< Set to 1 if VTOR is present */
#define __NVIC_PRIO_BITS 2 /*!< Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig 0 /*!< Set to 1 if different SysTick Config is used */
#define __FPU_PRESENT 0 /*!< Set to 1 if FPU is present */
#define __FPU_DP 0 /*!< Set to 1 if FPU is double precision FPU (default is single precision FPU) */
#define __ICACHE_PRESENT 0 /*!< Set to 1 if I-Cache is present */
#define __DCACHE_PRESENT 0 /*!< Set to 1 if D-Cache is present */
#define __DTCM_PRESENT 0 /*!< Set to 1 if DTCM is present */

/** @} */ /* End of group Configuration_of_CMSIS */

#include <core_cm0plus.h>

#if defined (__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined (__ICCARM__)
  #pragma language=extended
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wc11-extensions"
  #pragma clang diagnostic ignored "-Wreserved-id-macro"
#elif defined (__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined (__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined (__TASKING__)
  #pragma warning 586
#elif defined (__CSMC__)
  /* anonymous unions are enabled by default */
#else
#warning Not supported compiler type
#endif

/** @addtogroup Device_Peripheral_peripherals
 * @{
 */

typedef struct {
	__IM uint32_t DIR; // 0
	__IOM uint32_t DIRCLR, DIRSET, DIRTGL; // 0x4..0xf
	__IM uint32_t OUT; // 0x10
	__IOM uint32_t OUTCLR, OUTSET, OUTTGL; // 0x14..0x1f
	__IM uint32_t IN; // 0x20
	__IOM uint32_t CTRL, WRCONFIG; // 0x24..0x2b
	__IM uint8_t RESERVED1[4]; // 0x2c..0x2f
	__IOM uint8_t PMUX[16], PINCFG[32]; // 0x30..0x5f
} MKR4000_PORT_TypeDef;

/** @} */ /* end of group Device_Peripheral_peripherals */

#if defined (__CC_ARM)
  #pragma pop
#elif defined (__ICCARM__)
  /* leave anonymous unions enabled */
#elif (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic pop
#elif defined (__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined (__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined (__TASKING__)
  #pragma warning restore
#elif defined (__CSMC__)
  /* anonymous unions are enabled by default */
#else
#warning Not supported compiler type
#endif

/** @addtogroup Device_Peripheral_peripheralAddr
 * @{
 */

#define MKR4000_FLASH_BASE (0x00000000lu + 0x2000lu)
#define MKR4000_SRAM_BASE (0x20000000lu)
#define MKR4000_PB_BASE (0x40000000lu) // length 3 * 0x01000000
#define MKR4000_PORT_BASE (0x41004400lu) // length 2 * 0x80

/** @} */ /* End of group Device_Peripheral_peripheralAddr */

/** @addtogroup Device_Peripheral_declaration
 * @{
 */

#define MKR4000_PORTA ((MKR4000_PORT_TypeDef*)MKR4000_PORT_BASE)
#define MKR4000_PORTB ((MKR4000_PORT_TypeDef*)(MKR4000_PORT_BASE + 0x80))

#define MKR4000_PORT_PMUX_PMUXE_SHIFT 0
#define MKR4000_PORT_PMUX_PMUXO_SHIFT 4
#define MKR4000_PORT_PMUX_PMUX_BITS 4

#define MKR4000_PORT_PINCFG_PMUXEN_SHIFT 0
#define MKR4000_PORT_PINCFG_INEN_SHIFT 1

/** @} */ /* End of group MKR4000 */

/** @} */ /* End of group TEST26 */

#ifdef __cplusplus
}
#endif

#endif /* _H_TEST26_CMSIS_MKR4000 */
