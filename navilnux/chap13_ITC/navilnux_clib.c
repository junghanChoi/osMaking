/*
 * =====================================================================================
 *
 *       Filename:  navilnux_clib.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/03/2017 01:38:30 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  david (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <navilnux.h>

int navilnux_itc_send(int itcnum, int data)
{
	return itc_send(itcnum, data);
}

int navilnux_itc_get(int itcnum)
{
	int ret_value = 0;
	int data = 0;

	while(1){
		ret_value = itc_get(itcnum, &data);
		if( 0 == ret_value ){
			return data;
		}
		else if( -1 == ret_value){
			return ret_value;
		}else{
			// block till profer data comes.
			call_scheduler();
		}

	}
}
