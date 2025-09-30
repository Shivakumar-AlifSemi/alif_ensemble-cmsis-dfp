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
#include "vsi_isp_reg_def.h"
#include "vsi_comm_video.h"
#include "vsi_comm_isp.h"
#include "vsi_isp_core.h"
#include "mpi_isp_dmsc.h"
#include "vsi_isp_dmsc_v10.h"
#include "vsi_isp_math.h"
#include "isp_conf.h"

/* ISP_DEMOSAIC SLICE */
#define MRV_DEMOSAIC_THRESH_HOLD
#define MRV_DEMOSAIC_THRESH_HOLD_MASK       0x0000000FU
#define MRV_DEMOSAIC_THRESH_HOLD_SHIFT      0U

#define MRV_DEMOSAIC_BYPASS
#define MRV_DEMOSAIC_BYPASS_MASK            0x00000400U
#define MRV_DEMOSAIC_BYPASS_SHIFT           10U

/* ISP_FILT_MODE SLICE */
#define MRV_FILT_ENABEL
#define MRV_FILT_ENABEL_MASK                0x00000001U
#define MRV_FILT_ENABEL_SHIFT               0U

#define MRV_FILT_MODE
#define MRV_FILT_MODE_MASK                  0x00000002U
#define MRV_FILT_MODE_SHIFT                 1U

#define MRV_FILT_CHR_V_MODE
#define MRV_FILT_CHR_V_MODE_MASK            0x00000030U
#define MRV_FILT_CHR_V_MODE_SHIFT           4U

#define MRV_FILT_CHR_H_MODE
#define MRV_FILT_CHR_H_MODE_MASK            0x000000C0U
#define MRV_FILT_CHR_H_MODE_SHIFT           6U

#define MRV_FILT_LP_SELSECT
#define MRV_FILT_LP_SELSECT_MASK            0x00000F00U
#define MRV_FILT_LP_SELSECT_SHIFT           8U

/* ISP_CAC_CTRL SLICE */
#define MRV_CAC_ENABLE
#define MRV_CAC_ENABLE_MASK                 0x00000001U
#define MRV_CAC_ENABLE_SHIFT                0U

#define MRV_CAC_VCLIP_MODE
#define MRV_CAC_VCLIP_MODE_MASK             0x00000006U
#define MRV_CAC_VCLIP_MODE_SHIFT            1U

#define MRV_CAC_HCLIP_MODE
#define MRV_CAC_HCLIP_MODE_MASK             0x00000008U
#define MRV_CAC_HCLIP_MODE_SHIFT            3U

/* ISP_CAC_COUNT_START SLICE */
#define MRV_CAC_HSTART
#define MRV_CAC_HSTART_MASK                 0x000001FFU
#define MRV_CAC_HSTART_SHIFT                0U

#define MRV_CAC_VSTART
#define MRV_CAC_VSTART_MASK                 0x01FF0000U
#define MRV_CAC_VSTART_SHIFT                16U

/* ISP_CAC_A/ISP_CAC_B/ISP_CAC_C SLICE */
#define MRV_CAC_RED
#define MRV_CAC_RED_MASK                    0x000001FFU
#define MRV_CAC_RED_SHIFT                   0U

#define MRV_CAC_BLUE
#define MRV_CAC_BLUE_MASK                   0x01FF0000U
#define MRV_CAC_BLUE_SHIFT                  16U

/* ISP_CAC_X_NORM/ISP_CAC_Y_NORM SLICE */
#define MRV_CAC_NORM_FAC
#define MRV_CAC_NORM_FAC_MASK               0x0000001FU
#define MRV_CAC_NORM_FAC_SHIFT              0U

#define MRV_CAC_NORM_SH
#define MRV_CAC_NORM_SH_MASK                0x000F0000U
#define MRV_CAC_NORM_SH_SHIFT               16U

typedef struct vsiISP_DMSC_S {
    vsi_bool_t enable;
    vsi_u8_t threshold;
    ISP_CAC_ATTR_S cacAttr;
} ISP_DMSC_S;

typedef struct vsiISP_DMSC_CTRL_ATTR_S {
    ISP_DMSC_ATTR_S dmscAttr;
    ISP_DMSC_S new;
    ISP_DMSC_S cur;
    vsi_bool_t cfgUpd;
    vsi_u32_t iso;
} ISP_DMSC_CTRL_ATTR_S;

ISP_DMSC_CTRL_ATTR_S dmscCtrlAttr[ISP_DEV_CNT][ISP_PORT_CNT];

static ISP_DMSC_CTRL_ATTR_S *VSI_ISP_DmscV10GetCtrlAttr(ISP_PORT IspPort)
{
    return &dmscCtrlAttr[IspPort.devId][IspPort.portId];
}

static int VSI_ISP_DmscV10SetCacCtrl(ISP_PORT IspPort)
{
    ISP_DMSC_CTRL_ATTR_S *pDmscCtrlAttr = VSI_ISP_DmscV10GetCtrlAttr(IspPort);
    ISP_DMSC_S *pNew = &pDmscCtrlAttr->new;

    vsi_reg_t cacCtrl = 0;
    vsi_reg_t cacCntStart = 0;
    vsi_reg_t cacCntA = 0;
    vsi_reg_t cacCntB = 0;
    vsi_reg_t cacCntC = 0;
    vsi_reg_t cacXNorm = 0;
    vsi_reg_t cacYNorm = 0;

    REG_SET_SLICE(cacXNorm, MRV_CAC_NORM_FAC, pNew->cacAttr.xNormFactor);
    REG_SET_SLICE(cacXNorm, MRV_CAC_NORM_SH, pNew->cacAttr.xNormShift);
    VSI_ISP_CoreWriteReg(IspPort, ISP_CAC_X_NORM, cacXNorm);

    REG_SET_SLICE(cacYNorm, MRV_CAC_NORM_FAC, pNew->cacAttr.yNormFactor);
    REG_SET_SLICE(cacYNorm, MRV_CAC_NORM_SH, pNew->cacAttr.yNormShift);
    VSI_ISP_CoreWriteReg(IspPort, ISP_CAC_Y_NORM, cacYNorm);

    REG_SET_SLICE(cacCntA, MRV_CAC_RED, pNew->cacAttr.aRed);
    REG_SET_SLICE(cacCntA, MRV_CAC_BLUE, pNew->cacAttr.aBlue);
    VSI_ISP_CoreWriteReg(IspPort, ISP_CAC_COUNT_A, cacCntA);

    REG_SET_SLICE(cacCntB, MRV_CAC_RED, pNew->cacAttr.bRed);
    REG_SET_SLICE(cacCntB, MRV_CAC_BLUE, pNew->cacAttr.bBlue);
    VSI_ISP_CoreWriteReg(IspPort, ISP_CAC_COUNT_B, cacCntB);

    REG_SET_SLICE(cacCntC, MRV_CAC_RED, pNew->cacAttr.cRed);
    REG_SET_SLICE(cacCntC, MRV_CAC_BLUE, pNew->cacAttr.cBlue);
    VSI_ISP_CoreWriteReg(IspPort, ISP_CAC_COUNT_C, cacCntC);

    REG_SET_SLICE(cacCntStart, MRV_CAC_HSTART, pNew->cacAttr.hStart);
    REG_SET_SLICE(cacCntStart, MRV_CAC_VSTART, pNew->cacAttr.vStart);
    VSI_ISP_CoreWriteReg(IspPort, ISP_CAC_COUNT_START, cacCntStart);

    REG_SET_SLICE(cacCtrl, MRV_CAC_ENABLE, pNew->cacAttr.enable);
    REG_SET_SLICE(cacCtrl, MRV_CAC_VCLIP_MODE, pNew->cacAttr.vClipMode);
    REG_SET_SLICE(cacCtrl, MRV_CAC_HCLIP_MODE, pNew->cacAttr.hClipMode);
    VSI_ISP_CoreWriteReg(IspPort, ISP_CAC_CTRL, cacCtrl);

    return VSI_SUCCESS;
}

static int VSI_ISP_DmscV10SetCtrl(ISP_PORT IspPort)
{
    ISP_DMSC_CTRL_ATTR_S *pDmscCtrlAttr = VSI_ISP_DmscV10GetCtrlAttr(IspPort);
    ISP_DMSC_S *pNew = &pDmscCtrlAttr->new;
    ISP_DMSC_S *pCur = &pDmscCtrlAttr->cur;

    vsi_reg_t ispDmsc = 0;

    VSI_ISP_DmscV10SetCacCtrl(IspPort);

    REG_SET_SLICE(ispDmsc, MRV_DEMOSAIC_BYPASS, !pNew->enable);
    REG_SET_SLICE(ispDmsc, MRV_DEMOSAIC_THRESH_HOLD, pNew->threshold);
    VSI_ISP_CoreWriteReg(IspPort, ISP_DEMOSAIC, ispDmsc);

    vsios_memcpy(pCur, pNew, sizeof(*pNew));

    return VSI_SUCCESS;
}

int VSI_ISP_DmscV10Init(ISP_PORT IspPort)
{
    ISP_DMSC_CTRL_ATTR_S *pDmscCtrlAttr = VSI_ISP_DmscV10GetCtrlAttr(IspPort);
    ISP_DMSC_ATTR_S *pDmscAttr;

    vsios_memset(pDmscCtrlAttr, 0, sizeof(ISP_DMSC_CTRL_ATTR_S));

    pDmscAttr = &pDmscCtrlAttr->dmscAttr;

    pDmscAttr->enable               = 1;
    pDmscAttr->threshold            = 4;

    pDmscAttr->cacAttr.enable       = 0;
    pDmscAttr->cacAttr.hClipMode    = 0;
    pDmscAttr->cacAttr.vClipMode    = 0;
    pDmscAttr->cacAttr.hStart       = 1;
    pDmscAttr->cacAttr.vStart       = 1;
    pDmscAttr->cacAttr.aBlue        = 0;
    pDmscAttr->cacAttr.aRed         = 0;
    pDmscAttr->cacAttr.bBlue        = 0;
    pDmscAttr->cacAttr.bRed         = 0;
    pDmscAttr->cacAttr.cBlue        = 0;
    pDmscAttr->cacAttr.cRed         = 0;
    pDmscAttr->cacAttr.xNormShift   = 7;
    pDmscAttr->cacAttr.xNormFactor  = 29;
    pDmscAttr->cacAttr.yNormShift   = 7;
    pDmscAttr->cacAttr.yNormFactor  = 29;

    VSI_ISP_DmscV10SetAttr(IspPort, pDmscAttr);

    return VSI_SUCCESS;
}

int VSI_ISP_DmscV10DeInit(ISP_PORT IspPort)
{
    ISP_DMSC_CTRL_ATTR_S *pDmscCtrlAttr = VSI_ISP_DmscV10GetCtrlAttr(IspPort);

    return VSI_SUCCESS;
}

int VSI_ISP_DmscV10GetAttr(ISP_PORT IspPort, ISP_DMSC_ATTR_S *pDmscAttr)
{
    ISP_DMSC_CTRL_ATTR_S *pDmscCtrlAttr = VSI_ISP_DmscV10GetCtrlAttr(IspPort);

    vsios_memcpy(pDmscAttr, &pDmscCtrlAttr->dmscAttr, sizeof(*pDmscAttr));

    return VSI_SUCCESS;
}

static int VSI_ISP_DmscV10ParamsCheck(ISP_DMSC_ATTR_S *pDmscAttr)
{
    pDmscAttr->threshold =
        vsios_clamp(vsi_u8_t, pDmscAttr->threshold, 0, 255);

    pDmscAttr->cacAttr.hClipMode =
        vsios_clamp(vsi_u8_t, pDmscAttr->cacAttr.hClipMode, 0, 1);
    pDmscAttr->cacAttr.vClipMode =
        vsios_clamp(vsi_u8_t, pDmscAttr->cacAttr.vClipMode, 0, 2);
    pDmscAttr->cacAttr.hStart =
        vsios_clamp(vsi_u16_t, pDmscAttr->cacAttr.hStart, 1, 4095);
    pDmscAttr->cacAttr.vStart =
        vsios_clamp(vsi_u16_t, pDmscAttr->cacAttr.vStart, 1, 4095);
    pDmscAttr->cacAttr.aBlue =
        vsios_clamp(vsi_s16_t, pDmscAttr->cacAttr.aBlue, -256, 255);
    pDmscAttr->cacAttr.aRed =
        vsios_clamp(vsi_s16_t, pDmscAttr->cacAttr.aRed, -256, 255);
    pDmscAttr->cacAttr.bBlue =
        vsios_clamp(vsi_s16_t, pDmscAttr->cacAttr.bBlue, -256, 255);
    pDmscAttr->cacAttr.bRed =
        vsios_clamp(vsi_s16_t, pDmscAttr->cacAttr.bRed, -256, 255);
    pDmscAttr->cacAttr.cBlue =
        vsios_clamp(vsi_s16_t, pDmscAttr->cacAttr.cBlue, -256, 255);
    pDmscAttr->cacAttr.cRed =
        vsios_clamp(vsi_s16_t, pDmscAttr->cacAttr.cRed, -256, 255);
    pDmscAttr->cacAttr.xNormShift =
        vsios_clamp(vsi_u8_t, pDmscAttr->cacAttr.xNormShift, 0, 15);
    pDmscAttr->cacAttr.xNormFactor =
        vsios_clamp(vsi_u8_t, pDmscAttr->cacAttr.xNormFactor, 0, 31);
    pDmscAttr->cacAttr.yNormShift =
        vsios_clamp(vsi_u8_t, pDmscAttr->cacAttr.yNormShift, 0, 15);
    pDmscAttr->cacAttr.yNormFactor =
        vsios_clamp(vsi_u8_t, pDmscAttr->cacAttr.yNormFactor, 0, 31);

    return VSI_SUCCESS;
}

int VSI_ISP_DmscV10SetAttr(ISP_PORT IspPort, ISP_DMSC_ATTR_S *pDmscAttr)
{
    ISP_STATUS_E state;
    ISP_DMSC_CTRL_ATTR_S *pDmscCtrlAttr = VSI_ISP_DmscV10GetCtrlAttr(IspPort);
    ISP_DMSC_S *pNew = &pDmscCtrlAttr->new;
    int ret;

    ret = VSI_ISP_DmscV10ParamsCheck(pDmscAttr);
    if (ret)
        return ret;

    state = VSI_ISP_CoreGetPortStatus(IspPort);

    vsios_memcpy(&pDmscCtrlAttr->dmscAttr, pDmscAttr, sizeof(*pDmscAttr));

    pNew->enable = pDmscAttr->enable;
    pNew->threshold = pDmscAttr->threshold;
    vsios_memcpy(&pNew->cacAttr, &pDmscAttr->cacAttr, sizeof(pNew->cacAttr));

    if (state == ISP_STATE_STREAMOFF) {
        VSI_ISP_DmscV10SetCtrl(IspPort);
        pDmscCtrlAttr->cfgUpd = 0;
    } else {
        pDmscCtrlAttr->cfgUpd = 1;
    }

    return VSI_SUCCESS;
}

int VSI_ISP_DmscV10IrqProcess(ISP_PORT IspPort)
{
    ISP_DMSC_CTRL_ATTR_S *pDmscCtrlAttr = VSI_ISP_DmscV10GetCtrlAttr(IspPort);

    if (pDmscCtrlAttr->cfgUpd) {
        pDmscCtrlAttr->cfgUpd = 0;
        VSI_ISP_DmscV10SetCtrl(IspPort);
    }

    return VSI_SUCCESS;
}
