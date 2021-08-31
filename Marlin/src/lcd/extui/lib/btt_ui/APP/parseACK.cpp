#include "../../../../../inc/MarlinConfig.h"
#include "../TSC_Menu.h"
#if ENABLED(BTT_UI_SPI)

#define DMA_TRANS_LEN  ACK_MAX_SIZE

typedef struct
{
  char cache[DMA_TRANS_LEN];
  uint16_t wIndex;
  uint16_t rIndex;
}DMA_CIRCULAR_BUFFER;

char dmaL2Cache[ACK_MAX_SIZE];
// dma rx buffer
DMA_CIRCULAR_BUFFER dmaL1Data;

static uint16_t ack_index=0;

bool dmaL1NotEmpty(uint8_t port)
{
  return dmaL1Data.rIndex != dmaL1Data.wIndex;
}

void syncL2CacheFromL1(uint8_t port)
{
  uint16_t i = 0;
  while (dmaL1NotEmpty(port))
  {
    dmaL2Cache[i] = dmaL1Data.cache[dmaL1Data.rIndex];
    dmaL1Data.rIndex = (dmaL1Data.rIndex + 1) % DMA_TRANS_LEN;
    if (dmaL2Cache[i++] == '\n') break;
  }
  dmaL2Cache[i] = 0; // End character
}

static char ack_seen(const char *str)
{
  uint16_t i;
  for(ack_index=0; ack_index<ACK_MAX_SIZE && dmaL2Cache[ack_index]!=0; ack_index++)
  {
    for(i=0; str[i]!=0 && dmaL2Cache[ack_index+i]!=0 && dmaL2Cache[ack_index+i]==str[i]; i++)
    {}
    if(str[i]==0)
    {
      ack_index += i;
      return true;
    }
  }
  return false;
}

void parseACK(void)
{

}


#endif
