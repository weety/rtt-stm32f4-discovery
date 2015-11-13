/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 * 2014-04-27     Bernard      make code cleanup. 
 */

#include <board.h>
#include <rtthread.h>

#ifdef RT_USING_LWIP
#include <lwip/sys.h>
#include <lwip/api.h>
#include <netif/ethernetif.h>
#include "stm32f4xx_eth.h"
#endif

#ifdef RT_USING_GDB
#include <gdb_stub.h>
#endif

void rt_init_thread_entry(void* parameter)
{
    /* GDB STUB */
#ifdef RT_USING_GDB
    gdb_set_device("uart6");
    gdb_start();
#endif

/* Filesystem Initialization */
#ifdef RT_USING_DFS
		/* init the device filesystem */
		dfs_init();

#if defined(RT_USING_DFS_DEVFS)
		devfs_init();
		if (dfs_mount(RT_NULL, "/dev", "devfs", 0, 0) == 0)
			rt_kprintf("Device File System initialized!\n");
		else
			rt_kprintf("Device File System initialzation failed!\n");

		#ifdef RT_USING_NEWLIB 
		/* init libc */
		libc_system_init(CONSOLE_DEVICE);
		#endif

		#ifdef RT_USING_CPLUSPLUS
		/* init cplusplus */
		cplusplus_system_init();
		#endif
#endif
#endif

#ifdef RT_USING_I2C
    rt_hw_i2c_init();
#endif

    /* LwIP Initialization */
#ifdef RT_USING_LWIP
    {
        extern void lwip_sys_init(void);

        /* register ethernetif device */
        eth_system_device_init();

        rt_hw_stm32_eth_init();

        /* init lwip system */
        lwip_sys_init();
        rt_kprintf("TCP/IP initialized!\n");
    }
#endif
}

int rt_application_init()
{
    rt_thread_t tid;

    tid = rt_thread_create("init",
        rt_init_thread_entry, RT_NULL,
        2048, RT_THREAD_PRIORITY_MAX/3, 20);

    if (tid != RT_NULL)
        rt_thread_startup(tid);

    return 0;
}

/*@}*/
