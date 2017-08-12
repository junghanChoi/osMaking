/*
 * =====================================================================================
 *
 *       Filename:  navilnux.h
 *
 *    Description:  header sets
 *
 *        Version:  1.0
 *        Created:  07/20/2017 09:29:04 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  david (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  _KERNEL_H_
#define  _KERNEL_H_

#include <pxa255.h>
#include <time.h>
#include <gpio.h>
#include <stdio.h>
#include <string.h>

#include <navilnux_memory.h>
#include <navilnux_task.h>
#include <navilnux_user.h>
#include <navilnux_sys.h>
#include <navilnux_msg.h>
void navilnux_init(void);
void navilnux_user(void);
#endif     /* -----  not _KERNEL_H_  ----- */
