#include "../TSC_Menu.h"
#include "../../../../../inc/MarlinConfig.h"

#if ENABLED(BTT_UI_SPI)

void float2char(float slope,char*buffer,int n)  
{
    int temp,i,j;
    if(slope>=0)
        buffer[0] = ' ';
    else
    {
        buffer[0] = '-';
        slope = -slope;
    }
    temp = (int)slope;
    for(i=0;temp!=0;i++)
        temp /=10;
    temp =(int)slope;
    for(j=i;j>0;j--)
    {
        buffer[j] = temp%10+'0';
        temp /=10;
    }
    buffer[i+1] = '.';
    slope -=(int)slope;
    for(i=i+2;i<n-1;i++)
    {
        slope*=10;
        buffer[i]=(int)slope+'0';
        slope-=(int)slope;
    }
    buffer[n-1] = '\0';
}
#endif
