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

#include "vsi_list.h"
#include "vsios_sys.h"
#include "vsi_comm_video.h"
#include "vsi_vb_queue.h"

int VSI_VB_QueueInit(VB_QUEUE_S *pQueue)
{
    vsios_memset(pQueue, 0, sizeof(VB_QUEUE_S));

    InitListHead(&pQueue->queueList);
    InitListHead(&pQueue->doneList);

    return VSI_SUCCESS;
}

int VSI_VB_QueueDestroy(VB_QUEUE_S *pQueue)
{
    return VSI_SUCCESS;
}

int VSI_VB_QueueBuf(VB_QUEUE_S *pQueue, VIDEO_BUF_S *pBuf)
{
    VB_BUF_S *pVbBuf;

    pVbBuf = &pQueue->buf[pBuf->index];

    vsios_memcpy(&pVbBuf->buf, pBuf, sizeof(VIDEO_BUF_S));

    ListAddTail(&pVbBuf->list, &pQueue->queueList);

    return VSI_SUCCESS;

}

int VSI_VB_DeQueueBuf(VB_QUEUE_S *pQueue, VIDEO_BUF_S *pBuf, vsi_u32_t timeMs)
{
    int ret;

    VB_BUF_S *pVbBuf = NULL;

        pVbBuf = ListFirstEntry(&pQueue->doneList, VB_BUF_S, list);
        ListDel(&pVbBuf->list);
    }

    if (pVbBuf) {
        vsios_memcpy(pBuf, &pVbBuf->buf, sizeof(VIDEO_BUF_S));
        return VSI_SUCCESS;
    } else {
        return VSI_ERR_NOBUF;
    }
}

int VSI_VB_BufDone(VB_QUEUE_S *pQueue, VIDEO_BUF_S *pBuf)
{
    VB_BUF_S *pVbBuf;

    pVbBuf = &pQueue->buf[pBuf->index];

    ListAddTail(&pVbBuf->list, &pQueue->doneList);

    return VSI_SUCCESS;
}

int VSI_VB_GetEmptyBuf(VB_QUEUE_S *pQueue, VIDEO_BUF_S **pBuf)
{
    VB_BUF_S *pVbBuf = NULL;

    if (!ListEmpty(&pQueue->queueList)) {
        pVbBuf = ListFirstEntry(&pQueue->queueList, VB_BUF_S, list);
        ListDel(&pVbBuf->list);
    }

    if (!pVbBuf) {
        return VSI_ERR_NOBUF;
    }

    *pBuf = &pVbBuf->buf;

    return VSI_SUCCESS;
}

int VSI_VB_StreamOn(VB_QUEUE_S *pQueue)
{
    pQueue->state = VB_STATE_STREAMON;
    return VSI_SUCCESS;
}

int VSI_VB_StreamOff(VB_QUEUE_S *pQueue)
{

    pQueue->state = VB_STATE_STREAMOFF;

    InitListHead(&pQueue->queueList);
    InitListHead(&pQueue->doneList);

    return VSI_SUCCESS;
}

