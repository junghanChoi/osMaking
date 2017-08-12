/*
 * =====================================================================================
 *
 *       Filename:  navilnux_drv.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/10/2017 03:47:15 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  david (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <navilnux.h>

Navil_drv_mng drvmng;

int drv_register_drv(int drvnum, const char* name, fops* drvfops)
{
	if(drvnum > DRVLIMIT || drvnum < 0){
		return -1;
	}
	if(drvmng.free_drv_pool[drvnum].usecount >= 0){
		return -1;
		//it's already used. 
	}


	drvmng.free_drv_pool[drvnum].navil_fops = drvfops;
	drvmng.free_drv_pool[drvnum].usecount = 0;
	drvmng.free_drv_pool[drvnum].drvname = name;

	return 0;
}

void drv_init()
{
	int i;
	for(i = 0; i< DRVLIMIT; i++){
		drvmng.free_drv_pool[i].navil_fops = (fops*)0;
		drvmng.free_drv_pool[i].usecount = -1;
		drvmng.free_drv_pool[i].drvname = (const char*) 0;
	}
	drvmng.init = drv_init;
	drvmng.register_drv = drv_register_drv;
}
