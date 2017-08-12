/*
 * =====================================================================================
 *
 *       Filename:  navilnux_msg.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/03/2017 01:21:22 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  david (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <navilnux.h>

Navil_msg_mng	msgmng;

int msg_itc_send(int itcnum, int data)
{
	if(itcnum >= MAXMSG || itcnum < 0){
		return -1;
	}
	msgmng.free_msg_pool[itcnum].data = data;
	msgmng.free_msg_pool[itcnum].flag = 1;

	return itcnum;
}
int msg_itc_get(int itcnum, int* data)
{
	if(itcnum >= MAXMSG || itcnum < 0){
		return -1;
	}
	if(msgmng.free_msg_pool[itcnum].flag == 0){
			return -2;
	}
	*data = msgmng.free_msg_pool[itcnum].data;
	msgmng.free_msg_pool[itcnum].flag = 0;
	msgmng.free_msg_pool[itcnum].data = 0;
	return 0;

}

void msg_init(void)
{
	int i;
	for(i=0; i< MAXMSG; i++){
		msgmng.free_msg_pool[i].data = 0;
		msgmng.free_msg_pool[i].flag= 0;
	}
	msgmng.init = msg_init;
	msgmng.itc_send = msg_itc_send;
	msgmng.itc_get = msg_itc_get;
}
