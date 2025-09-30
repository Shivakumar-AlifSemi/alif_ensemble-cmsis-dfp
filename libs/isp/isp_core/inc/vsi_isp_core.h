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

#ifndef __VSI_ISP_CORE_H__

#define __VSI_ISP_CORE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#include <RTE_Device.h>

#include "vsi_comm_sns.h"
#include "vsi_vb_queue.h"
#include <vsios_type.h>
#include "isp_conf.h"


#define REG_SET_SLICE(reg, name, value) \
    ((reg) = (((reg) & ~(name##_MASK)) | (((value) << (name##_SHIFT)) & (name##_MASK))));

#define REG_GET_SLICE(reg, name) \
    (((reg) & (name##_MASK)) >> (name##_SHIFT));


typedef struct vsiISP_CORE_CHN_S {
    vsi_u32_t chnId;
    ISP_TRANS_BUS_E transBus;
    FORMAT_S chnFormat;
    VB_QUEUE_S vbQueue;
    VIDEO_BUF_S *pBuf;
    VIDEO_BUF_S *pShdBuf;
    vsi_u32_t refCnt;
    ISP_STATUS_E state;
} ISP_CORE_CHN_S;

typedef struct vsiISP_ISO_ATTR_S {
    vsi_u32_t         iso;
    vsi_bool_t        autoRouteUpd;
    vsi_u32_t         autoRoute[ISP_AUTO_STRENGTH_NUN];
} ISP_ISO_ATTR_S;

typedef struct vsiISP_CORE_PORT_S {
    vsi_u32_t         portId;
    ISP_INPUT_TYPE_E  ispInputType;
    ISP_MODE_E        ispMode;
    ISP_HDR_MODE_E    hdrMode;
    ISP_STICH_MODE_E  stichMode;
    PIXEL_FORMAT_E    pixelFormat;
    ISP_SYNC_CONFIG_S syncCfg;
    RECT_S            snsRect;
    vsi_u32_t         snsFps;
    RECT_S            inFormRect;
    RECT_S            outFormRect;
    SIZE_S            ispCoreSize;
    RECT_S            iSRect;
    ISP_ISO_ATTR_S    isoAttr;
    ISP_CORE_CHN_S    ispChns[1];

    vsi_u32_t refCnt;
    ISP_STATUS_E state;

    ISP_SNS_FUNC_S ispSnsFunc;
    AE_SNS_FUNC_S  aeSnsFunc;

    vsi_u32_t colorTemp;
} ISP_CORE_PORT_S;

typedef struct vsiISP_CORE_DEV_S {
    vsi_u32_t        devId;
    ISP_WORK_MODE_E  ispWorkMode;
    ISP_CORE_PORT_S  ispPorts[1];

    vsi_u32_t refCnt;
    ISP_STATUS_E state;
} ISP_CORE_DEV_S;

typedef struct vsiISP_MODULE_NODE_S {
    int (*ModuleInit)       (ISP_PORT IspPort);
    int (*ModuleDeInit)     (ISP_PORT IspPort);
    int (*ModuleIrqProcess) (ISP_PORT IspPort);
    int (*ModuleInitBySize) (ISP_PORT IspPort, SIZE_S ispSize);
} ISP_MODULE_NODE_S;

ISP_CORE_DEV_S  *VSI_ISP_CoreGetDevice(ISP_DEV IspDev);
ISP_CORE_PORT_S *VSI_ISP_CoreGetPort(ISP_PORT IspPort);
ISP_CORE_CHN_S  *VSI_ISP_CoreGetChn(ISP_CHN IspChn);

int VSI_ISP_CoreDevInit(ISP_DEV IspDev);
int VSI_ISP_CoreDevExit(ISP_DEV IspDev);

int VSI_ISP_CoreSetDevAttr(ISP_DEV IspDev, ISP_DEV_ATTR_S *pDevAttr);
int VSI_ISP_CoreGetDevAttr(ISP_DEV IspDev, ISP_DEV_ATTR_S *pDevAttr);
int VSI_ISP_CoreEnableDev(ISP_DEV IspDev);
int VSI_ISP_CoreDisableDev(ISP_DEV IspDev);

int VSI_ISP_CoreSetPortAttr(ISP_PORT IspPort, ISP_PORT_ATTR_S *pPortAttr);
int VSI_ISP_CoreGetPortAttr(ISP_PORT IspPort, ISP_PORT_ATTR_S *pPortAttr);
int VSI_ISP_CoreEnablePort(ISP_PORT IspPort);
int VSI_ISP_CoreDisablePort(ISP_PORT IspPort);
ISP_STATUS_E VSI_ISP_CoreGetPortStatus(ISP_PORT IspPort);
int VSI_ISP_CoreSetIsoAttr(ISP_PORT IspPort, ISP_ISO_ATTR_S* isoAttr);
int VSI_ISP_CoreGetIsoAttr(ISP_PORT IspPort, ISP_ISO_ATTR_S* isoAttr);

int VSI_ISP_CoreWriteReg(ISP_PORT IspPort, vsi_u32_t reg, vsi_u32_t val);
int VSI_ISP_CoreReadReg(ISP_PORT IspPort, vsi_u32_t reg, vsi_u32_t *pVal);

int VSI_ISP_CoreSetChnAttr(ISP_CHN IspChn, ISP_CHN_ATTR_S *pChnAttr);
int VSI_ISP_CoreGetChnAttr(ISP_CHN IspChn, ISP_CHN_ATTR_S *pChnAttr);
int VSI_ISP_CoreEnableChnAttr(ISP_CHN IspChn);
int VSI_ISP_CoreDisableChnAttr(ISP_CHN IspChn);

int VSI_ISP_CoreChnQBUF(ISP_CHN IspChn, VIDEO_BUF_S *pBuf);
int VSI_ISP_CoreChnDQBUF(ISP_CHN IspChn, VIDEO_BUF_S *pBuf, vsi_u32_t timeMs);

extern int VSI_MPI_ISP_SetMiChnAttr(ISP_CHN IspChn, ISP_CHN_ATTR_S *pChnAttr);
extern int VSI_MPI_ISP_SetMiChnStream(ISP_CHN IspChn, vsi_u8_t stream);
extern int VSI_MPI_ISP_MiIrqProcess(ISP_DEV IspDev, vsi_u32_t miMis);

#if defined(ISP_SCALE_V10) || defined(ISP_SCALE_V20)
extern int VSI_MPI_ISP_SetScaleAttr(ISP_CHN IspChn, ISP_CHN_ATTR_S *pChnAttr);
#endif

#ifdef ISP_BINNING_V10
extern int VSI_ISP_BinV10SetNum(ISP_PORT IspPort);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
