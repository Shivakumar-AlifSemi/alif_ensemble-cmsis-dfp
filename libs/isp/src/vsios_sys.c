/****************************************************************************
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2024 Vivante Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include "vsios_sys.h"
#include "sys_utils.h"

void *vsios_malloc(vsi_u32_t size)
{
    return malloc(size);
}

void vsios_free(void *ptr)
{
    free(ptr);
    return;
}

void *vsios_memset(void *s, int c, vsi_u32_t n)
{
    return memset(s, c, n);
}

void *vsios_memcpy(void *dest, const void *src, vsi_u32_t n)
{
    return memcpy(dest, src, n);
}

int vsios_memcmp(const void *s1, const void *s2, size_t n)
{
    return memcmp(s1, s2, n);
}

int vsios_snprintf(char *str, vsi_u32_t size, const char *format, ...)
{
    va_list args;
    int i;

    va_start(args, format);
    i = vsnprintf(str, size, format, args);
    va_end(args);

    return i;
}

void vsios_usleep(vsi_u32_t usec)
{
	sys_busy_loop_us(usec);
    return;
}

/* CMSIS project does not require mutexes or semaphores.
   This code is from Verisilicon and has been commented out accordingly. */
#if 0
int vsios_mutex_create(vsios_mutex_t *os_mutex)
{
    return pthread_mutex_init(&os_mutex->mutex, NULL);
}

int vsios_mutex_destroy(vsios_mutex_t *os_mutex)
{
    return pthread_mutex_destroy(&os_mutex->mutex);
}

int vsios_mutex_lock(vsios_mutex_t *os_mutex)
{
    return pthread_mutex_lock(&os_mutex->mutex);
}

int vsios_mutex_unlock(vsios_mutex_t *os_mutex)
{
    return pthread_mutex_unlock(&os_mutex->mutex);
}

int vsios_sem_init(vsios_sem_t *os_sem, vsi_u32_t value)
{
    return sem_init(&os_sem->sem, 0, value);
}

int vsios_sem_post(vsios_sem_t *os_sem)
{
    return sem_post(&os_sem->sem);
}

int vsios_sem_wait(vsios_sem_t *os_sem, vsi_u32_t ms)
{
    /*struct timespec ts;
    long tv_nsec;

    clock_gettime(CLOCK_REALTIME, &ts);
    tv_nsec = ts.tv_nsec + ms * 1000 * 1000;

    ts.tv_sec += tv_nsec / 1000000000;
    ts.tv_nsec += tv_nsec % 1000000000;


    return sem_timedwait(&os_sem->sem, &ts);*/
    int ret;
    vsi_u32_t time = ms * 100;
    do {
        ret = sem_trywait(&os_sem->sem);
        if (ret == 0) {
            return ret;
        }

        if (time) {
            vsios_usleep(10);
            time--;
        }
    } while (time);

    return ret;
}

int vsios_sem_destroy(vsios_sem_t *os_sem)
{
    return sem_destroy(&os_sem->sem);
}

int vsios_thread_attr_init(vsios_thread_attr_t *os_attr)
{
    return pthread_attr_init(&os_attr->pthread_attr);
}

int vsios_thread_attr_destroy(vsios_thread_attr_t *os_attr)
{
    return pthread_attr_destroy(&os_attr->pthread_attr);
}

int vsios_thread_attr_setstacksize(vsios_thread_attr_t *os_attr, vsi_u32_t stacksize)
{
    os_attr->stacksize = stacksize;
    return pthread_attr_setstacksize(&os_attr->pthread_attr, stacksize);
}

int vsios_thread_attr_setpriority(vsios_thread_attr_t *os_attr, int priority)
{
    struct sched_param schedparam;
    schedparam.sched_priority = priority;

    return pthread_attr_setschedparam(&os_attr->pthread_attr, &schedparam);
}

int vsios_thread_create(vsios_thread_t *os_thread,
                        const vsios_thread_attr_t *os_attr,
                        vsios_thread_exit_t (*start_routine) (void *),
                        void *arg)
{
    return pthread_create(&os_thread->thread,
                        &os_attr->pthread_attr, start_routine, arg);
}

int vsios_thread_destroy(vsios_thread_t *os_thread)
{
    return pthread_join(os_thread->thread, NULL);
}
#endif
