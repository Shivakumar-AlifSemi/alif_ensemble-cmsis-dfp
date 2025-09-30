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
#include "isp_conf.h"
#include "vsios_error.h"
#include <string.h>
#include "vsios_sys.h"
#include "vsi_comm_video.h"
#include "vsi_comm_isp.h"
#include "vsi_comm_sns.h"
#include "vsi_isp_sns.h"
#include "mpi_isp.h"
#include "vsi_isp_core.h"

int VSI_MPI_ISP_Init(ISP_DEV IspDev)
{
    int ret;

    if (IspDev >= ISP_DEV_CNT)
        return VSI_ERR_INVALID_DEVID;

    ret = VSI_ISP_CoreDevInit(IspDev);

    return ret;
}

int VSI_MPI_ISP_Exit(ISP_DEV IspDev)
{
    int ret;

    if (IspDev >= ISP_DEV_CNT)
        return VSI_ERR_INVALID_DEVID;

    ret = VSI_ISP_CoreDevExit(IspDev);

    return ret;
}

int VSI_MPI_ISP_SnsRegCallBack(ISP_PORT IspPort, ISP_SNS_OBJ_S *pSnsObj, vsi_u8_t snsDev)
{
    return VSI_ISP_SnsRegCallBack(IspPort, pSnsObj, snsDev);
}

int VSI_MPI_ISP_SnsUnRegCallBack(ISP_PORT IspPort)
{
    return VSI_ISP_SnsUnRegCallBack(IspPort);
}

int VSI_MPI_ISP_SetDevAttr(ISP_DEV IspDev, ISP_DEV_ATTR_S *pDevAttr)
{
    int ret;

    if (IspDev >= ISP_DEV_CNT)
        return VSI_ERR_INVALID_DEVID;

    ret = VSI_ISP_CoreSetDevAttr(IspDev, pDevAttr);

    return ret;
}

int VSI_MPI_ISP_GetDevAttr(ISP_DEV IspDev, ISP_DEV_ATTR_S *pDevAttr)
{
    int ret;

    if (IspDev >= ISP_DEV_CNT)
        return VSI_ERR_INVALID_DEVID;

    ret = VSI_ISP_CoreGetDevAttr(IspDev, pDevAttr);

    return ret;
}

int VSI_MPI_ISP_EnableDev(ISP_DEV IspDev)
{
    int ret;
    if (IspDev >= ISP_DEV_CNT)
        return VSI_ERR_INVALID_DEVID;

    ret = VSI_ISP_CoreEnableDev(IspDev);

    return ret;
}

int VSI_MPI_ISP_DisableDev(ISP_DEV IspDev)
{
    int ret;
    if (IspDev >= ISP_DEV_CNT)
        return VSI_ERR_INVALID_DEVID;

    ret = VSI_ISP_CoreDisableDev(IspDev);

    return ret;
}

int VSI_MPI_ISP_SetPortAttr(ISP_PORT IspPort, ISP_PORT_ATTR_S *pPortAttr)
{
    int ret;
    if (IspPort.devId >= ISP_DEV_CNT)
        return VSI_ERR_INVALID_DEVID;
    if (IspPort.portId >= ISP_PORT_CNT)
        return VSI_ERR_INVALID_PORTID;

    ret = VSI_ISP_CoreSetPortAttr(IspPort, pPortAttr);

    return ret;
}

int VSI_MPI_ISP_GetPortAttr(ISP_PORT IspPort, ISP_PORT_ATTR_S *pPortAttr)
{
    int ret;
    if (IspPort.devId >= ISP_DEV_CNT)
        return VSI_ERR_INVALID_DEVID;
    if (IspPort.portId >= ISP_PORT_CNT)
        return VSI_ERR_INVALID_PORTID;

    ret = VSI_ISP_CoreGetPortAttr(IspPort, pPortAttr);

    return ret;
}

int VSI_MPI_ISP_EnablePort(ISP_PORT IspPort)
{
    int ret;

    if (IspPort.devId >= ISP_DEV_CNT)
        return VSI_ERR_INVALID_DEVID;
    if (IspPort.portId >= ISP_PORT_CNT)
        return VSI_ERR_INVALID_PORTID;

    ret = VSI_ISP_CoreEnablePort(IspPort);

    return ret;
}

int VSI_MPI_ISP_DisablePort(ISP_PORT IspPort)
{
    int ret;

    if (IspPort.devId >= ISP_DEV_CNT)
        return VSI_ERR_INVALID_DEVID;
    if (IspPort.portId >= ISP_PORT_CNT)
        return VSI_ERR_INVALID_PORTID;

    ret = VSI_ISP_CoreDisablePort(IspPort);

    return ret;
}

int VSI_MPI_ISP_WriteReg(ISP_PORT IspPort, vsi_u32_t reg, vsi_u32_t val)
{
    int ret;

    if (IspPort.devId >= ISP_DEV_CNT)
        return VSI_ERR_INVALID_DEVID;

    if (IspPort.portId >= ISP_PORT_CNT)
        return VSI_ERR_INVALID_PORTID;

    ret = VSI_ISP_CoreWriteReg(IspPort, reg, val);

    return ret;
}

int VSI_MPI_ISP_ReadReg(ISP_PORT IspPort, vsi_u32_t reg, vsi_u32_t *pVal)
{
    int ret;

    if (IspPort.devId >= ISP_DEV_CNT)
        return VSI_ERR_INVALID_DEVID;

    if (IspPort.portId >= ISP_PORT_CNT)
        return VSI_ERR_INVALID_PORTID;

    ret = VSI_ISP_CoreReadReg(IspPort, reg, pVal);

    return ret;
}

int VSI_MPI_ISP_SetChnAttr(ISP_CHN IspChn, ISP_CHN_ATTR_S *pChnAttr)
{
    int ret;

    if (IspChn.devId >= ISP_DEV_CNT)
        return VSI_ERR_INVALID_DEVID;

    if (IspChn.portId >= ISP_PORT_CNT)
        return VSI_ERR_INVALID_PORTID;

    if (IspChn.chnId >= ISP_CHN_CNT)
        return VSI_ERR_INVALID_CHNID;

    ret = VSI_ISP_CoreSetChnAttr(IspChn, pChnAttr);
    if (ret != VSI_SUCCESS) {
        return ret;
    }

#if defined(ISP_SCALE_V10) || defined(ISP_SCALE_V20)
    ret = VSI_MPI_ISP_SetScaleAttr(IspChn, pChnAttr);
#endif

    return ret;
}

int VSI_MPI_ISP_GetChnAttr(ISP_CHN IspChn, ISP_CHN_ATTR_S *pChnAttr)
{
    int ret;

    if (IspChn.devId >= ISP_DEV_CNT)
        return VSI_ERR_INVALID_DEVID;

    if (IspChn.portId >= ISP_PORT_CNT)
        return VSI_ERR_INVALID_PORTID;

    if (IspChn.chnId >= ISP_CHN_CNT)
        return VSI_ERR_INVALID_CHNID;

    ret = VSI_ISP_CoreGetChnAttr(IspChn, pChnAttr);

    return ret;
}

int VSI_MPI_ISP_EnableChn(ISP_CHN IspChn)
{
    int ret;

    if (IspChn.devId >= ISP_DEV_CNT)
        return VSI_ERR_INVALID_DEVID;

    if (IspChn.portId >= ISP_PORT_CNT)
        return VSI_ERR_INVALID_PORTID;

    if (IspChn.chnId >= ISP_CHN_CNT)
        return VSI_ERR_INVALID_CHNID;

    ret = VSI_ISP_CoreEnableChnAttr(IspChn);

    return ret;
}

int VSI_MPI_ISP_DisableChn(ISP_CHN IspChn)
{
    int ret;

    if (IspChn.devId >= ISP_DEV_CNT)
        return VSI_ERR_INVALID_DEVID;

    if (IspChn.portId >= ISP_PORT_CNT)
        return VSI_ERR_INVALID_PORTID;

    if (IspChn.chnId >= ISP_CHN_CNT)
        return VSI_ERR_INVALID_CHNID;

    ret = VSI_ISP_CoreDisableChnAttr(IspChn);

    return ret;
}

int VSI_MPI_ISP_SetAutoRoute(ISP_PORT IspPort, ISP_AUTO_ROUTE_S *pAutoRoute)
{
    int ret;
    if (IspPort.devId >= ISP_DEV_CNT)
        return VSI_ERR_INVALID_DEVID;
    if (IspPort.portId >= ISP_PORT_CNT)
        return VSI_ERR_INVALID_PORTID;

    ISP_ISO_ATTR_S isoAttr;
    VSI_ISP_CoreGetIsoAttr(IspPort, &isoAttr);
    vsios_memcpy(&isoAttr.autoRoute, &pAutoRoute->autoRoute, sizeof(pAutoRoute->autoRoute));

    ret = VSI_ISP_CoreSetIsoAttr(IspPort, &isoAttr);

    return ret;
}

int VSI_MPI_ISP_GetAutoRoute(ISP_PORT IspPort, ISP_AUTO_ROUTE_S *pAutoRoute)
{
    int ret;
    if (IspPort.devId >= ISP_DEV_CNT)
        return VSI_ERR_INVALID_DEVID;
    if (IspPort.portId >= ISP_PORT_CNT)
        return VSI_ERR_INVALID_PORTID;

    ISP_ISO_ATTR_S isoAttr;
    ret = VSI_ISP_CoreGetIsoAttr(IspPort, &isoAttr);

    vsios_memcpy(&pAutoRoute->autoRoute, &isoAttr.autoRoute, sizeof(pAutoRoute->autoRoute));

    return ret;
}

int VSI_MPI_ISP_QBUF(ISP_CHN IspChn, VIDEO_BUF_S *pBuf)
{
    return VSI_ISP_CoreChnQBUF(IspChn, pBuf);
}

int VSI_MPI_ISP_DQBUF(ISP_CHN IspChn, VIDEO_BUF_S *pBuf,  vsi_u32_t timeMs)
{
    return VSI_ISP_CoreChnDQBUF(IspChn, pBuf, timeMs);
}
