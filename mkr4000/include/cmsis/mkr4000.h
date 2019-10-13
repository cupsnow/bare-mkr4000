/** @author joelai */

#ifndef _H_MKR4000_CMSIS
#define _H_MKR4000_CMSIS

/** @defgroup MKR4000_CMSIS
 * CMSIS driver for mkr4000.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup MKR4000_CMSIS_IRQ IRQ
 *
 * @ingroup MKR4000_CMSIS
 */

/** @addtogroup MKR4000_CMSIS_IRQ
 * @{
 */

/** IRQ number. */
typedef enum IRQn {
	Reset_IRQn = -15, /**< Reset_IRQn */
	NonMaskableInt_IRQn = -14, /**< NonMaskableInt_IRQn */
	HardFault_IRQn = -13, /**< HardFault_IRQn */
	SVCall_IRQn = -5, /**< SVCall_IRQn */
	PendSV_IRQn = -2, /**< PendSV_IRQn */
	SysTick_IRQn = -1, /**< SysTick_IRQn */

	PM_IRQn = 0, /**< PM_IRQn */
	SYSCTRL_IRQn = 1, /**< SYSCTRL_IRQn */
	WDT_IRQn = 2, /**< WDT_IRQn */
	RTC_IRQn = 3, /**< RTC_IRQn */
	EIC_IRQn = 4, /**< EIC_IRQn */
	NVMCTRL_IRQn = 5, /**< NVMCTRL_IRQn */
	DMAC_IRQn = 6, /**< DMAC_IRQn */
	USB_IRQn = 7, /**< USB_IRQn */
	EVSYS_IRQn = 8, /**< EVSYS_IRQn */
	SERCOM0_IRQn = 9, /**< SERCOM0_IRQn */
	SERCOM1_IRQn = 10, /**< SERCOM1_IRQn */
	SERCOM2_IRQn = 11, /**< SERCOM2_IRQn */
	SERCOM3_IRQn = 12, /**< SERCOM3_IRQn */
	SERCOM4_IRQn = 13, /**< SERCOM4_IRQn */
	SERCOM5_IRQn = 14, /**< SERCOM5_IRQn */
	TCC0_IRQn = 15, /**< TCC0_IRQn */
	TCC1_IRQn = 16, /**< TCC1_IRQn */
	TCC2_IRQn = 17, /**< TCC2_IRQn */
	TC3_IRQn = 18, /**< TC3_IRQn */
	TC4_IRQn = 19, /**< TC4_IRQn */
	TC5_IRQn = 20, /**< TC5_IRQn */
	ADC_IRQn = 23, /**< ADC_IRQn */
	AC_IRQn = 24, /**< AC_IRQn */
	DAC_IRQn = 25, /**< DAC_IRQn */
	PTC_IRQn = 26, /**< PTC_IRQn */
	I2S_IRQn = 27, /**< I2S_IRQn */

	PERIPH_COUNT_IRQn = 28 /**< PERIPH_COUNT_IRQn */
} IRQn_Type;

/** @} MKR4000_CMSIS_IRQ */

/** @defgroup MKR4000_CMSIS_CORE_CFG Core configuration
 *
 * @ingroup MKR4000_CMSIS
 *
 * @{
 */

#define __CM0PLUS_REV 0x0001 /**< Core Revision r0p1 */
#define __MPU_PRESENT 0 /**< MPU is absent */
#define __VTOR_PRESENT 1 /**< VTOR is present */
#define __NVIC_PRIO_BITS 2 /**< Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig 0 /**< default SysTick Config is used */
#define __FPU_PRESENT 0 /**< FPU is absent */
#define __FPU_DP 0 /**< FPU is absent */
#define __ICACHE_PRESENT 0 /**< ICACHE is absent */
#define __DCACHE_PRESENT 0 /**< DCACHE is absent */
#define __DTCM_PRESENT 0 /**< DTCM is absent */

/** @} MKR4000_CMSIS_CORE_CFG */

#include <core_cm0plus.h>

/** @defgroup MKR4000_CMSIS_MEM_MAP Memory map
 *
 * @ingroup MKR4000_CMSIS
 *
 * @{
 */

#define MKR4000_FLASH_BASE 0x00000000lu /**< Flash base, length 0x2000. */
#define MKR4000_SRAM_BASE 0x20000000lu /**< SRAM base. */
#define MKR4000_PBA_BASE 0x40000000lu /**< AHB-APB Bridge A base. */
#define MKR4000_PM_BASE 0x40000400lu /**< Power manager base. */
#define MKR4000_SYSCTRL_BASE 0x40000800lu /**< System controller base. */
#define MKR4000_GCLK_BASE 0x40000c00lu /**< GCLK base. */
#define MKR4000_PBB_BASE 0x41000000lu /**< AHB-APB Bridge B base. */
#define MKR4000_NVMCTRL_BASE 0x41004000lu /**< NVMCTRL base */
#define MKR4000_PORT_BASE 0x41004400lu /**< PORT base, length 2 * 0x80 */
#define MKR4000_PBC_BASE 0x42000000lu /**< AHB-APB Bridge C base. */
#define MKR4000_SERCOM_BASE 0x42000800lu /**< SERCOM base, length 6 * 0x400 */

/** @} MKR4000_CMSIS_MEM_MAP */

#ifdef __cplusplus
}
#endif

#endif /* _H_MKR4000_CMSIS */
