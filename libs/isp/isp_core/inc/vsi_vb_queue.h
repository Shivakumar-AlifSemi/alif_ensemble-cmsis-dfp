/*****************************************************************************
 * Copyright (c) 2024 by VeriSilicon Holdings Co., Ltd. ("VeriSilicon")
 * All Rights Reserved.
 *
 * The material in this file is confidential and contains trade secrets of
 * of VeriSilicon.  This is proprietary information owned or licensed by
 * VeriSilicon.  No part of this work may be disclosed, reproduced, copied,
 * transmitted, or used in any way for any purpose, without the express
 * written permission of VeriSilicon.
 *
 *****************************************************************************/

#ifndef __VSI_VB_QUEUE_H__
#define __VSI_VB_QUEUE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#include <RTE_Device.h>
#include "vsi_list.h"
#include "vsios_error.h"

#define VB_MAX_FRAME (RTE_ISP_BUFFER_COUNT)

typedef struct vsiVB_BUF_S {
    struct ListHead_s list;
    VIDEO_BUF_S buf;
} VB_BUF_S;

typedef enum vsiVB_STATUS_E {
    VB_STATE_STREAMOFF = 0,
    VB_STATE_STREAMON  = 1,
} VB_STATUS_E;

typedef struct vsiVB_QUEUE_S {
    struct ListHead_s queueList;
    struct ListHead_s doneList;
    VB_STATUS_E state;
    VB_BUF_S buf[VB_MAX_FRAME];
} VB_QUEUE_S;

int VSI_VB_QueueInit(VB_QUEUE_S *pQueue);
int VSI_VB_QueueDestroy(VB_QUEUE_S *pQueue);
int VSI_VB_QueueBuf(VB_QUEUE_S *pQueue, VIDEO_BUF_S *pBuf);
int VSI_VB_DeQueueBuf(VB_QUEUE_S *pQueue, VIDEO_BUF_S *pBuf, vsi_u32_t timeMs);
int VSI_VB_BufDone(VB_QUEUE_S *pQueue, VIDEO_BUF_S *pBuf);
int VSI_VB_GetEmptyBuf(VB_QUEUE_S *pQueue, VIDEO_BUF_S **pBuf);
int VSI_VB_StreamOn(VB_QUEUE_S *pQueue);
int VSI_VB_StreamOff(VB_QUEUE_S *pQueue);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
