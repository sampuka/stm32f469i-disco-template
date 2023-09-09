/*
 * Unified Cortex Startup - C startup file
 *
 * This file is in public domain
 *
 * Put together by Paul Sokolovsky based on article by Vanya Sergeev
 * http://dev.frozeneskimo.com/notes/cortex_cmsis/ , GNU ld documentation
 * and numerous other public resources.
 *
 */

#include <stdint.h>

/* Declare linker-defined symbols. The only thing of interest
   regarding these symbols is their *address*. uint32_t hints
   of alignment. */
extern uint32_t _end_text;
extern uint32_t _start_data;
extern uint32_t _end_data;
extern uint32_t _start_bss;
extern uint32_t _end_bss;
extern uint32_t _start_stack;
extern uint32_t _end_stack;
extern void (*__init_array_start[])(void) __attribute__((weak));
extern void (*__init_array_end[])(void) __attribute__((weak));
extern void (*__preinit_array_start[])(void) __attribute__((weak));
extern void (*__preinit_array_end[])(void) __attribute__((weak));

/* C main function */
extern int main(void);
/* Device-specific initialization function. Optional, any Cortex-M
   should be able to start up in its default mode on its own, though
   some may have errata for some peripherals (including PLL) which
   this function may "patch". */
extern void SystemInit(void);

void Dummy_Handler(void);

/* Cortex-M core interrupt handlers */
#define ALIAS(sym) __attribute__((weak, alias(sym)))

void Reset_Handler(void);
void NMI_Handler(void) ALIAS("Dummy_Handler");
void HardFault_Handler(void) ALIAS("Dummy_Handler");
void MemManage_Handler(void) ALIAS("Dummy_Handler");
void BusFault_Handler(void) ALIAS("Dummy_Handler");
void UsageFault_Handler(void) ALIAS("Dummy_Handler");
void SVC_Handler(void) ALIAS("Dummy_Handler");
void DebugMon_Handler(void) ALIAS("Dummy_Handler");
void PendSV_Handler(void) ALIAS("Dummy_Handler");
void SysTick_Handler(void) ALIAS("Dummy_Handler");

/* 16 standard Cortex-M vectors - these are present in every MCU */
void *core_vector_table[16] __attribute__((section(".cortex_vectors"))) = {
	// See http://sourceware.org/binutils/docs/ld/Source-Code-Reference.html
	// why the address is used here (if not intuitive)
	&_end_stack, Reset_Handler, NMI_Handler, HardFault_Handler, MemManage_Handler, BusFault_Handler, UsageFault_Handler, 0, 0, 0, 0, SVC_Handler, DebugMon_Handler, 0, PendSV_Handler, SysTick_Handler,
};

void WWDG_Handler(void) ALIAS("Dummy_Handler");
void PVD_Handler(void) ALIAS("Dummy_Handler");
void TAMP_STAMP_Handler(void) ALIAS("Dummy_Handler");
void RTC_WKUP_Handler(void) ALIAS("Dummy_Handler");
void FLASH_Handler(void) ALIAS("Dummy_Handler");
void RCC_Handler(void) ALIAS("Dummy_Handler");
void EXTI0_Handler(void) ALIAS("Dummy_Handler");
void EXTI1_Handler(void) ALIAS("Dummy_Handler");
void EXTI2_Handler(void) ALIAS("Dummy_Handler");
void EXTI3_Handler(void) ALIAS("Dummy_Handler");
void EXTI4_Handler(void) ALIAS("Dummy_Handler");
void DMA1_Stream0_Handler(void) ALIAS("Dummy_Handler");
void DMA1_Stream1_Handler(void) ALIAS("Dummy_Handler");
void DMA1_Stream2_Handler(void) ALIAS("Dummy_Handler");
void DMA1_Stream3_Handler(void) ALIAS("Dummy_Handler");
void DMA1_Stream4_Handler(void) ALIAS("Dummy_Handler");
void DMA1_Stream5_Handler(void) ALIAS("Dummy_Handler");
void DMA1_Stream6_Handler(void) ALIAS("Dummy_Handler");
void ADC_Handler(void) ALIAS("Dummy_Handler");
void CAN1_TX_Handler(void) ALIAS("Dummy_Handler");
void CAN1_RX0_Handler(void) ALIAS("Dummy_Handler");
void CAN1_RX1_Handler(void) ALIAS("Dummy_Handler");
void CAN1_SCE_Handler(void) ALIAS("Dummy_Handler");
void EXTI9_5_Handler(void) ALIAS("Dummy_Handler");
void TIM1_BRK_TIM9_Handler(void) ALIAS("Dummy_Handler");
void TIM1_UP_TIM10_Handler(void) ALIAS("Dummy_Handler");
void TIM1_TRG_COM_TIM11_Handler(void) ALIAS("Dummy_Handler");
void TIM1_CC_Handler(void) ALIAS("Dummy_Handler");
void TIM2_Handler(void) ALIAS("Dummy_Handler");

/* 16 standard Cortex-M vectors - these are present in every MCU */
void *vendor_vector_table[29] __attribute__((section(".vendor_vectors"))) = {
	WWDG_Handler,    PVD_Handler,     TAMP_STAMP_Handler,   RTC_WKUP_Handler,     FLASH_Handler,        RCC_Handler,          EXTI0_Handler,         EXTI1_Handler,         EXTI2_Handler,
	EXTI3_Handler,   EXTI4_Handler,   DMA1_Stream0_Handler, DMA1_Stream1_Handler, DMA1_Stream2_Handler, DMA1_Stream3_Handler, DMA1_Stream4_Handler,  DMA1_Stream5_Handler,  DMA1_Stream6_Handler,
	ADC_Handler,     CAN1_TX_Handler, CAN1_RX0_Handler,     CAN1_RX1_Handler,     CAN1_SCE_Handler,     EXTI9_5_Handler,      TIM1_BRK_TIM9_Handler, TIM1_UP_TIM10_Handler, TIM1_TRG_COM_TIM11_Handler,
	TIM1_CC_Handler, TIM2_Handler};

/* Based on http://sourceware.org/binutils/docs/ld/Output-Section-LMA.html */
void Reset_Handler(void)
{
	register uint32_t *src, *dst;

	/* Copy data section from flash to RAM */
	src = &_end_text;
	dst = &_start_data;
	while (dst < &_end_data) *dst++ = *src++;

	/* Clear the bss section, assumes .bss goes directly after .data */
	dst = &_start_bss;
	while (dst < &_end_bss) *dst++ = 0;

	uint32_t preinitcount = __preinit_array_end - __preinit_array_start;

	for (uint32_t i = 0; i < preinitcount; i++)
	{
		__preinit_array_start[i]();
	}

	uint32_t initcount = __init_array_end - __init_array_start;

	for (uint32_t i = 0; i < initcount; i++)
	{
		__init_array_start[i]();
	}

#ifndef NO_SYSTEMINIT
	SystemInit();
#endif
	main();
	while (1)
		;
}

void Dummy_Handler(void)
{
	/* Receiving an unexpected interrupt is considered a bug
	   in the program. Let's make it very visible by just
	   hanging the processing. Ignoring it by just returning
	   may result in very complicated debugging sessions. */
	while (1)
		;
}
