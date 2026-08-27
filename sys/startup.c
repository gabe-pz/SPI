#include <stdint.h>


//some values to note: F401 = 96K, F407 = 192K, F411 = 128K
#define SRAM_START (0x20000000U)
#define SRAM_SIZE  (128U * 1024U)
#define SRAM_END   (SRAM_START + SRAM_SIZE)
#define STACK_POINTER_INIT_ADDRESS (SRAM_END)


#define ISR_VECTOR_SIZE_WORDS 114

extern uint32_t _etext, _sdata, _edata, _sbss, _ebss;
void main(void);

void reset_handler(void);
void default_handler(void);
void nmi_handler(void) __attribute__((weak, alias("default_handler")));
void hard_fault_handler(void) __attribute__((weak, alias("default_handler")));
void mem_manage_handler(void) __attribute__((weak, alias("default_handler")));
void bus_fault_handler(void) __attribute__((weak, alias("default_handler")));
void usage_fault_handler(void) __attribute__((weak, alias("default_handler")));
void svcall_handler(void) __attribute__((weak, alias("default_handler")));
void debug_monitor_handler(void) __attribute__((weak, alias("default_handler")));
void pendsv_handler(void) __attribute__((weak, alias("default_handler")));
void systick_handler(void) __attribute__((weak, alias("default_handler")));


uintptr_t isr_vector[ISR_VECTOR_SIZE_WORDS] __attribute__((section(".isr_vector"))) = {
  STACK_POINTER_INIT_ADDRESS,
  (uintptr_t)&reset_handler,
  (uintptr_t)&nmi_handler,
  (uintptr_t)&hard_fault_handler,
  (uintptr_t)&mem_manage_handler,
  (uintptr_t)&bus_fault_handler,
  (uintptr_t)&usage_fault_handler,
  0,
  0,
  0,
  0,
  (uintptr_t)&svcall_handler,
  (uintptr_t)&debug_monitor_handler,
  0,
  (uintptr_t)&pendsv_handler,
  (uintptr_t)&systick_handler,

 
};

void reset_handler(void)
{
  //Copy .data from FLASH to SRAM 
  uint32_t data_size = (uint32_t)&_edata - (uint32_t)&_sdata;
  uint8_t *flash_data = (uint8_t*) &_etext;
  uint8_t *sram_data = (uint8_t*) &_sdata;

  for (uint32_t i = 0; i < data_size; i++)
  {
    sram_data[i] = flash_data[i];
  }

  //Zero-fill .bss section in SRAM 
  uint32_t bss_size = (uint32_t)&_ebss - (uint32_t)&_sbss;
  uint8_t *bss = (uint8_t*) &_sbss;

  for (uint32_t i = 0; i < bss_size; i++)
  {
    bss[i] = 0;
  }


  main();

  while (1) {
    //trap if main() returns 
  }
}

void default_handler(void)
{
  while(1);
}