/*******************************************************************

********************************************************************/
#include "data_handle.h"
/*******************************************************************/
extern uint16_t gatts_if_1;
extern uint16_t connet_id;
extern uint16_t whandle;

void msg_handle_(uint8_t *data,uint32_t len )
{
    uint8_t  tmp[len];
    uint32_t i;
    
    memset(tmp,0,len);
    memcpy(tmp,data,len);
	
    esp_ble_gatts_send_indicate(gatts_if_1,
		connet_id,
		whandle,
		len,tmp,false);
	    printf("bobo_recv: %d",len);
	    for(i=0;i<len;i++)
	    {
		printf(" %02x",tmp[i]);
	    }
	    printf("\n");
 /*  if((data[0]==0xFF))
       {
	    switch data[1]:
		switch data[2]:
	    }
	    */
    }





