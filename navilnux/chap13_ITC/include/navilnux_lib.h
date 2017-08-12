/*
 * =====================================================================================
 *
 *       Filename:  navilnux_lib.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/02/2017 01:03:16 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  david (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _NAVIL_LIB
#define _NAVIL_LIB


extern int mysyscall(int, int, int);
extern int mysyscall4(int, int, int,int);
extern int itc_send(int, int);
extern int itc_get(int, int*);
extern void call_scheduler(void);

int navilnux_itc_send(int, int);
int navilnux_itc_get(int);
#endif
