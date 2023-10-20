/* Exemplo de arquivo de script de ligação para ARM Cortex-M */

MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K
  RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
  .text :
  {
    KEEP(*(.isr_vector))  /* Vetor de interrupções */
    *(.text)             /* Código do programa */
  } > FLASH

  .data :
  {
    *(.data)             /* Dados inicializados */
  } > RAM

  .bss :
  {
    *(.bss)              /* Dados não inicializados */
  } > RAM

  /* Outras seções podem ser definidas aqui, se necessário */

  /* Defina símbolos de início e fim da memória RAM */
  __bss_start__ = ADDR(.bss);
  __bss_end__ = ADDR(.bss) + SIZEOF(.bss);
}
