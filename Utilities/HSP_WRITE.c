#include <stdint.h>
#include "gd32f4xx_usart.h"

#if (defined (__ICCARM__) && (__VER__ < 9000000))
/**
 * @brief  Re-target fputc function.
 * @param  [in] ch
 * @param  [in] f
 * @retval int32_t
 */
int32_t fputc(int32_t ch, FILE *f)
{
    usart_data_transmit(USART2, (uint8_t)ch);
    while(RESET == usart_flag_get(USART2, USART_FLAG_TBE));
    return ch;
}

#elif (defined (__ICCARM__) && (__VER__ >= 9000000))
#include <LowLevelIOInterface.h>


#pragma module_name = "?__write"
size_t __dwrite(int handle, const unsigned char *buffer, size_t size)
{
    if (buffer == 0)
  {
    return 0;
  }

  for (/* Empty */; size != 0; --size)
  {
    usart_data_transmit(USART2, (uint8_t)*buffer++);
    while(RESET == usart_flag_get(USART2, USART_FLAG_TBE));
  }

  return size;
}
#endif