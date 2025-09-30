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

#include "vsios_sys.h"
#include "vsios_isp.h"
#include "vsi_comm_video.h"
#include "vsi_comm_isp.h"
#include "vsi_isp_core.h"
#include "vsi_comm_sns.h"
#include "vsi_isp_sns.h"
#include "isp_conf.h"

#include <vsi_isp_reg_def.h>
typedef struct vsiISP_SNS_CTRL_ATTR_S {
    ISP_SNS_REGS_INFO_S snsRegInfo;
    ISP_SNS_REGS_INFO_S cfgNode;
    vsi_bool_t stream;
    vsi_u32_t frmPos;
    vsi_bool_t busyCfg;
    vsi_u32_t maxDelay;
} ISP_SNS_CTRL_ATTR_S;

ISP_SNS_CTRL_ATTR_S snsCtrlAttr[ISP_DEV_CNT][ISP_PORT_CNT];

static ISP_SNS_CTRL_ATTR_S *VSI_ISP_SnsGetCtrlAttr(ISP_PORT IspPort)
{
    return &snsCtrlAttr[IspPort.devId][IspPort.portId];
}

int VSI_ISP_SnsRegCallBack(ISP_PORT IspPort, ISP_SNS_OBJ_S *pSnsObj, vsi_u8_t snsDev)
{
    ISP_SNS_CTRL_ATTR_S *pSnsCtrlAttr = VSI_ISP_SnsGetCtrlAttr(IspPort);
    ISP_CORE_PORT_S *pPort = VSI_ISP_CoreGetPort(IspPort);
    ISP_SNS_FUNC_S *pIspSnsFunc = &pPort->ispSnsFunc;
    AE_SNS_FUNC_S *pAeSnsFunc = &pPort->aeSnsFunc;
    int ret;

    vsios_memset(pSnsCtrlAttr, 0, sizeof(*pSnsCtrlAttr));

    if (pSnsObj->pfnInitIspSnsFunc) {
        pSnsObj->pfnInitIspSnsFunc(pIspSnsFunc);

        if (pIspSnsFunc->pfnSensorInit) {
            ret = pIspSnsFunc->pfnSensorInit(IspPort, snsDev);
            if (ret)
                return ret;
        } else {
            return VSI_ERR_NULL_PTR;
        }

        if (pIspSnsFunc->pfnSetIspDefault) {
            pIspSnsFunc->pfnSetIspDefault(IspPort);
        }
    } else {
        return VSI_ERR_NULL_PTR;
    }

    if (pSnsObj->pfnInitAeSnsFunc) {
        ret = pSnsObj->pfnInitAeSnsFunc(pAeSnsFunc);
        if (ret)
            return ret;
    } else {
        return VSI_ERR_NULL_PTR;
    }

    VSI_ISP_GetSnsRegInfo(IspPort, &pSnsCtrlAttr->snsRegInfo);

    return VSI_SUCCESS;
}

int VSI_ISP_SnsUnRegCallBack(ISP_PORT IspPort)
{
    ISP_CORE_PORT_S *pPort = VSI_ISP_CoreGetPort(IspPort);
    ISP_SNS_FUNC_S *pIspSnsFunc = &pPort->ispSnsFunc;

    if (pIspSnsFunc->pfnSensorExit) {
        pIspSnsFunc->pfnSensorExit(IspPort);
    }

    return VSI_SUCCESS;
}

int VSI_ISP_SnsWriteReg(ISP_PORT IspPort, vsi_u32_t addr, vsi_u32_t data)
{
    ISP_CORE_PORT_S *pPort = VSI_ISP_CoreGetPort(IspPort);
    ISP_SNS_FUNC_S *pIspSnsFunc = &pPort->ispSnsFunc;
    int ret;

    if (pIspSnsFunc->pfnWriteReg) {
        ret = pIspSnsFunc->pfnWriteReg(IspPort, addr, data);
        if (ret)
            return ret;
    } else {
        return VSI_ERR_NULL_PTR;
    }

    return VSI_SUCCESS;
}

int VSI_ISP_SnsReadReg(ISP_PORT IspPort, vsi_u32_t addr, vsi_u32_t *pData)
{
    ISP_CORE_PORT_S *pPort = VSI_ISP_CoreGetPort(IspPort);
    ISP_SNS_FUNC_S *pIspSnsFunc = &pPort->ispSnsFunc;
    int ret;

    if (pIspSnsFunc->pfnReadReg) {
        ret = pIspSnsFunc->pfnReadReg(IspPort, addr, pData);
        if (ret)
            return ret;
    } else {
        return VSI_ERR_NULL_PTR;
    }

    return VSI_SUCCESS;
}

int VSI_ISP_SnsSetMode(ISP_PORT IspPort, ISP_PORT_ATTR_S *pPortAttr)
{
    ISP_SNS_CTRL_ATTR_S *pSnsCtrlAttr = VSI_ISP_SnsGetCtrlAttr(IspPort);
    ISP_SNS_MODE_S sensorMode;
    ISP_CORE_PORT_S *pPort = VSI_ISP_CoreGetPort(IspPort);
    ISP_SNS_FUNC_S *pIspSnsFunc = &pPort->ispSnsFunc;
    int ret;

    sensorMode.width      = pPortAttr->snsRect.width;
    sensorMode.height     = pPortAttr->snsRect.height;
    sensorMode.fps        = pPortAttr->snsFps;
    sensorMode.pixelFormat = pPortAttr->pixelFormat;
    sensorMode.hdrMode     = pPortAttr->hdrMode;
    sensorMode.stichMode   = pPortAttr->stichMode;

    if (pIspSnsFunc->pfnSetMode) {
        ret = pIspSnsFunc->pfnSetMode(IspPort, &sensorMode);
        if (ret)
            return ret;
    } else {
        return VSI_ERR_NULL_PTR;
    }

    VSI_ISP_GetSnsRegInfo(IspPort, &pSnsCtrlAttr->snsRegInfo);
    VSI_ISP_SnsSetFps(IspPort, pPortAttr->snsFps);

    return VSI_SUCCESS;
}

int VSI_ISP_SnsStreamOn(ISP_PORT IspPort)
{
    ISP_SNS_CTRL_ATTR_S *pSnsCtrlAttr = VSI_ISP_SnsGetCtrlAttr(IspPort);
    ISP_CORE_PORT_S *pPort = VSI_ISP_CoreGetPort(IspPort);
    ISP_SNS_FUNC_S *pIspSnsFunc = &pPort->ispSnsFunc;
    int ret;

    if (pIspSnsFunc->pfnSetStream) {
        ret = pIspSnsFunc->pfnSetStream(IspPort, 1);
        if (ret)
            return ret;
    } else {
        return VSI_ERR_NULL_PTR;
    }
    pSnsCtrlAttr->stream = 1;

    return VSI_SUCCESS;
}

int VSI_ISP_SnsStreamOff(ISP_PORT IspPort)
{
    ISP_SNS_CTRL_ATTR_S *pSnsCtrlAttr = VSI_ISP_SnsGetCtrlAttr(IspPort);
    ISP_CORE_PORT_S *pPort = VSI_ISP_CoreGetPort(IspPort);
    ISP_SNS_FUNC_S *pIspSnsFunc = &pPort->ispSnsFunc;
    int ret;

    if (pIspSnsFunc->pfnSetStream) {
        ret = pIspSnsFunc->pfnSetStream(IspPort, 0);
        if (ret)
            return ret;
    } else {
        return VSI_ERR_NULL_PTR;
    }

    pSnsCtrlAttr->stream = 0;

    return VSI_SUCCESS;
}

int VSI_ISP_SnsGetAeDefault(ISP_PORT IspPort, AE_SNS_DEFAULT_S *pAeSnsDft)
{
    ISP_CORE_PORT_S *pPort = VSI_ISP_CoreGetPort(IspPort);
    AE_SNS_FUNC_S  *pAeSnsFunc = &pPort->aeSnsFunc;
    int ret;

    if (pAeSnsFunc->pfnGetAeDefault) {
        ret = pAeSnsFunc->pfnGetAeDefault(IspPort, pAeSnsDft);
        if (ret)
            return ret;
    } else {
        return VSI_ERR_NULL_PTR;
    }

    return VSI_SUCCESS;
}

int VSI_ISP_SnsSetFps(ISP_PORT IspPort, vsi_u32_t fps)
{
    ISP_CORE_PORT_S *pPort = VSI_ISP_CoreGetPort(IspPort);
    AE_SNS_FUNC_S  *pAeSnsFunc = &pPort->aeSnsFunc;
    int ret;

    if (pAeSnsFunc->pfnSetFps) {
        ret = pAeSnsFunc->pfnSetFps(IspPort, fps);
        if (ret)
            return ret;
    } else {
        return VSI_ERR_NULL_PTR;
    }

    return VSI_SUCCESS;
}

int VSI_ISP_SnsSlowFrameRate(ISP_PORT IspPort, vsi_u32_t fullLines)
{
    ISP_CORE_PORT_S *pPort = VSI_ISP_CoreGetPort(IspPort);
    AE_SNS_FUNC_S  *pAeSnsFunc = &pPort->aeSnsFunc;
    int ret;

    if (pAeSnsFunc->pfnSlowFrameRate) {
        ret = pAeSnsFunc->pfnSlowFrameRate(IspPort, fullLines);
        if (ret)
            return ret;
    } else {
        return VSI_ERR_NULL_PTR;
    }

    return VSI_SUCCESS;
}

int VSI_ISP_SnsIntTimeUpdate(ISP_PORT IspPort, vsi_u32_t *pIntTime)
{
    ISP_CORE_PORT_S *pPort = VSI_ISP_CoreGetPort(IspPort);
    AE_SNS_FUNC_S  *pAeSnsFunc = &pPort->aeSnsFunc;
    int ret;

    if (pAeSnsFunc->pfnIntTimeUpdate) {
        ret = pAeSnsFunc->pfnIntTimeUpdate(IspPort, pIntTime);
        if (ret)
            return ret;
    } else {
        return VSI_ERR_NULL_PTR;
    }

    return VSI_SUCCESS;
}

int VSI_ISP_SnsGainUpdate(ISP_PORT IspPort, vsi_u32_t *pAgain, vsi_u32_t *pDgain)
{
    ISP_CORE_PORT_S *pPort = VSI_ISP_CoreGetPort(IspPort);
    AE_SNS_FUNC_S  *pAeSnsFunc = &pPort->aeSnsFunc;
    int ret;

    if (pAeSnsFunc->pfnGainUpdate) {
        ret = pAeSnsFunc->pfnGainUpdate(IspPort, pAgain, pDgain);
        if (ret)
            return ret;
    } else {
        return VSI_ERR_NULL_PTR;
    }

    return VSI_SUCCESS;
}

int VSI_ISP_SnsSetExpRatio(ISP_PORT IspPort, ISP_EXP_RATIO_S *pExpRatio)
{
    ISP_CORE_PORT_S *pPort = VSI_ISP_CoreGetPort(IspPort);
    AE_SNS_FUNC_S  *pAeSnsFunc = &pPort->aeSnsFunc;
    int ret;

    if (pAeSnsFunc->pfnSetExpRatio) {
        ret = pAeSnsFunc->pfnSetExpRatio(IspPort, pExpRatio);
        if (ret)
            return ret;
    } else {
        return VSI_ERR_NULL_PTR;
    }

    return VSI_SUCCESS;
}

int VSI_ISP_GetSnsRegInfo(ISP_PORT IspPort, ISP_SNS_REGS_INFO_S *pSnsRegsInfo)
{
    ISP_CORE_PORT_S *pPort = VSI_ISP_CoreGetPort(IspPort);
    AE_SNS_FUNC_S  *pAeSnsFunc = &pPort->aeSnsFunc;
    int ret;

    if (pAeSnsFunc->pfnGetSnsRegInfo) {
        ret = pAeSnsFunc->pfnGetSnsRegInfo(IspPort, pSnsRegsInfo);
        if (ret)
            return ret;
    } else {
        return VSI_ERR_NULL_PTR;
    }

    return VSI_SUCCESS;
}

static int VSI_ISP_SnsSyncReg(ISP_PORT IspPort)
{
    ISP_SNS_CTRL_ATTR_S *pSnsCtrlAttr = VSI_ISP_SnsGetCtrlAttr(IspPort);
    ISP_SNS_REGS_INFO_S *pSnsRegInfo = &pSnsCtrlAttr->snsRegInfo;
    ISP_SNS_REGS_INFO_S *pSnsCfgNode = &pSnsCtrlAttr->cfgNode;
    int ret;
    int i;

    if (pSnsCtrlAttr->busyCfg == 0) {
        ret = VSI_ISP_GetSnsRegInfo(IspPort, pSnsRegInfo);
        if (ret)
            return ret;
        for (i = 0; i < pSnsRegInfo->regCnt; i++) {
            if (pSnsCfgNode->snsData[i].data != pSnsRegInfo->snsData[i].data) {
                pSnsCtrlAttr->busyCfg = 1;
                pSnsCtrlAttr->frmPos = 0;
                pSnsCtrlAttr->maxDelay = vsios_max(vsi_u8_t,
                                        pSnsCtrlAttr->maxDelay,
                                        pSnsRegInfo->snsData[i].delayFrameNum);

            }
        }
    }

    if (pSnsCtrlAttr->busyCfg == 1) {
        for (i = 0; i < pSnsRegInfo->regCnt; i++) {
            if ((pSnsCfgNode->snsData[i].data != pSnsRegInfo->snsData[i].data) &&
                (pSnsRegInfo->snsData[i].delayFrameNum == (pSnsRegInfo->delayMax - pSnsCtrlAttr->frmPos))) {
                VSI_ISP_SnsWriteReg(IspPort,
                                pSnsRegInfo->snsData[i].regAddr,
                                pSnsRegInfo->snsData[i].data);
                pSnsCfgNode->snsData[i].data = pSnsRegInfo->snsData[i].data;
            }
        }
        pSnsCtrlAttr->frmPos++;
        if (pSnsCtrlAttr->frmPos >= (pSnsRegInfo->delayMax + 1)) {
            pSnsCtrlAttr->busyCfg = 0;
        }
    }

    return VSI_SUCCESS;
}

int VSI_ISP_SnsRegUpdate(ISP_PORT IspPort)
{
    ISP_SNS_CTRL_ATTR_S *pSnsCtrlAttr = VSI_ISP_SnsGetCtrlAttr(IspPort);
    ISP_SNS_REGS_INFO_S snsRegInfo;
    ISP_SNS_REGS_INFO_S *pSnsRegInfo = &pSnsCtrlAttr->snsRegInfo;
    int ret;
    int i;

    if (pSnsCtrlAttr->stream == 0) {
        ret = VSI_ISP_GetSnsRegInfo(IspPort, &snsRegInfo);
        if (ret)
            return ret;

        for (i = 0; i < snsRegInfo.regCnt; i++) {
            if (snsRegInfo.snsData[i].data != pSnsRegInfo->snsData[i].data) {
                VSI_ISP_SnsWriteReg(IspPort,
                                snsRegInfo.snsData[i].regAddr,
                                snsRegInfo.snsData[i].data);
                pSnsRegInfo->snsData[i].data = snsRegInfo.snsData[i].data;
            }
        }

        pSnsCtrlAttr->busyCfg = 0;
        pSnsCtrlAttr->frmPos = 0;
        vsios_memcpy(&pSnsCtrlAttr->cfgNode, pSnsRegInfo, sizeof(ISP_SNS_REGS_INFO_S));
    } else {
        VSI_ISP_SnsSyncReg(IspPort);
    }

    return VSI_SUCCESS;
}

vsi_bool_t VSI_ISP_SnsCfgIsBusy(ISP_PORT IspPort)
{
    ISP_SNS_CTRL_ATTR_S *pSnsCtrlAttr = VSI_ISP_SnsGetCtrlAttr(IspPort);

    return pSnsCtrlAttr->busyCfg;
}

vsi_bool_t VSI_ISP_SnsStreamStatus(ISP_PORT IspPort)
{
    ISP_SNS_CTRL_ATTR_S *pSnsCtrlAttr = VSI_ISP_SnsGetCtrlAttr(IspPort);

    return pSnsCtrlAttr->stream;
}
