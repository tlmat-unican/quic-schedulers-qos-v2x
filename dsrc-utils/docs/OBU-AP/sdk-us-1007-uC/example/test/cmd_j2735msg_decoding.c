#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <linuxapi.h>
#include <tx_api.h>
#include <craton/debug.h>
#include "error_code.h"

#include "j2735r63_map_api.h"
#include "j2735r63_spat_api.h"
#include "j2735r63_common.h"

#include <libcli.h>
#include <craton/cli.h>
#include <atlk/nav_service.h>
#include <nxd_bsd.h>

#include <dot3_common.h>
#include <dot3_api.h>

#define SHOW_DECODED 1
#define DECODE_PRINT_LEVEL 1

static int stop_msg;

void stop_msg_func(ULONG input)
{
	(void)input;
	stop_msg = 1;
	return;
}


int cmd_rx_Msg(cli_instance_t *cli, const char *command, char *argv[], int argc)
{
	/* Unused parameters */
	(void)cli;
	(void)command;
	(void)argv;
	(void)argc;

	map_standard_item *decoded_item_map = NULL;
	spat_standard_item *decoded_item_spat = NULL;

	uint32_t err_msg;	

	uint32_t err, ret;

	uint16_t msg_id;
	TX_TIMER stop_msg_timer;

	uint32_t psid;
	wme_handle_t handle;
	uint16_t serv_index;
	struct wsm_info wsm_info;
	struct in_wsm in_wsm;

	if(argc != 1) { /* In threadx, cmd is not included in argc */
		printf("rxSpatMap <psid>\n");
		printf("-- usage --\n");
		printf("<psid> psid of sending WSM packets, 1 - 127\n");
		printf("-- example --\n");
		printf("rxSpatMap <DSRCmsgID> Note: 18 is MAP, 19 is SPAT\n");
		return 1;
	}
	psid = atoi(argv[0]);

	err = wme_init(&handle);
	if(err != 0){
		printf("wme_init = %d\n", err);
		return 0;
	}
	printf("wme_init success, handle = %d\n", handle);

	/* Create a wsm service */
	memset(&wsm_info, 0, sizeof(wsm_info));
	wsm_info.psid = psid;
	ret = wme_wsm_serv_create(&handle, &serv_index, &wsm_info);
	if(ret < 0) {
		printf("wme_wsm_service = %d\n", ret);
		return 0;
	}
	printf("wme_wsm_service success\n");

	printf("waiting 5s to recv wsm packets\n");
	stop_msg = 0;
	tx_timer_create(&stop_msg_timer, NULL, stop_msg_func, 0, 5000, 1, TX_AUTO_ACTIVATE);
	while(stop_msg == 0) 
	{
		err = wme_wsm_recv(&handle, &in_wsm, 1000);
		if(err != 0) 
		{
			printf("wsm_recv = %d\n", err);
			continue;
		}
		printf("wsm_recv success\n");
		printf("wsm data len = %d\n", in_wsm.length);
						
		msg_id = j2735_get_msg_type(in_wsm.data);
		printf("\nMessage ID:%d\n", msg_id);		
		 
		if (msg_id == MAP_ID)
		{

			err_msg = j2735r63_map_decode(&decoded_item_map, in_wsm.data, MAX_MAP_BUFF_SIZE, DECODE_PRINT_LEVEL);
			if(err_msg < 0)
			{
				printf("decode error...\n");
				return -1;
			}
    
			if(SHOW_DECODED == 1){_show_map_standard_item(decoded_item_map);}
	    
			j2735r63_map_free_struct(decoded_item_map);
			decoded_item_map = NULL;
		}
		else if (msg_id == SPAT_ID)
		{

			err_msg = j2735r63_spat_decode(&decoded_item_spat, in_wsm.data, MAX_SPAT_BUFF_SIZE, DECODE_PRINT_LEVEL);
			if(err_msg < 0)
			{
				printf("decode error...\n");
				return -1;
			}
	    
			if(SHOW_DECODED == 1){_show_spat_standard_item(decoded_item_spat);}
	    
			j2735r63_spat_free_struct(decoded_item_spat);		
			decoded_item_spat = NULL;
		}
	}
	err  = wme_wsm_serv_delete(&handle, serv_index);
	if(err != 0) 
	{
		printf("wme_wsm_service = %d\n", err);
		wme_exit(&handle);	
		return 0;
	}
	printf("wme_wsm_service to del success\n");

	tx_timer_delete(&stop_msg_timer);
	wme_exit(&handle);

	return 0;
}



