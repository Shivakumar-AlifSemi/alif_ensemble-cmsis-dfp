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
#include "vsi_isp_tpg.h"
#include <vsi_isp_reg_def.h>

#define MRV_ISP_TPG_RESOLUTION
#define MRV_ISP_TPG_RESOLUTION_MASK 0x00000C00U
#define MRV_ISP_TPG_RESOLUTION_SHIFT 10U

#define MRV_ISP_TPG_MAX_SYNC
#define MRV_ISP_TPG_MAX_SYNC_MASK 0x00000200U
#define MRV_ISP_TPG_MAX_SYNC_SHIFT 9U

#define MRV_ISP_TPG_DEF_SYNC
#define MRV_ISP_TPG_DEF_SYNC_MASK 0x00000100U
#define MRV_ISP_TPG_DEF_SYNC_SHIFT 8U

#define MRV_ISP_TPG_COLOR_DEPTH
#define MRV_ISP_TPG_COLOR_DEPTH_MASK 0x000000C0U
#define MRV_ISP_TPG_COLOR_DEPTH_SHIFT 6U

#define MRV_ISP_TPG_CFA_PAT
#define MRV_ISP_TPG_CFA_PAT_MASK 0x00000030U
#define MRV_ISP_TPG_CFA_PAT_SHIFT 4U

#define MRV_ISP_TPG_IMG_NUM
#define MRV_ISP_TPG_IMG_NUM_MASK 0x0000000EU
#define MRV_ISP_TPG_IMG_NUM_SHIFT 1U

#define MRV_ISP_TPG_ENABLE
#define MRV_ISP_TPG_ENABLE_MASK 0x00000001U
#define MRV_ISP_TPG_ENABLE_SHIFT 0U

#define MRV_ISP_TPG_HTOTAL_IN
#define MRV_ISP_TPG_HTOTAL_IN_MASK 0x0FFFC000U
#define MRV_ISP_TPG_HTOTAL_IN_SHIFT 14U

#define MRV_ISP_TPG_VTOTAL_IN
#define MRV_ISP_TPG_VTOTAL_IN_MASK 0x00003FFFU
#define MRV_ISP_TPG_VTOTAL_IN_SHIFT 0U

#define MRV_ISP_TPG_HACT_IN
#define MRV_ISP_TPG_HACT_IN_MASK 0x0FFFC000U
#define MRV_ISP_TPG_HACT_IN_SHIFT 14U

#define MRV_ISP_TPG_VACT_IN
#define MRV_ISP_TPG_VACT_IN_MASK 0x00003FFFU
#define MRV_ISP_TPG_VACT_IN_SHIFT 0U

#define MRV_ISP_TPG_FP_H_IN
#define MRV_ISP_TPG_FP_H_IN_MASK 0x0FFFC000U
#define MRV_ISP_TPG_FP_H_IN_SHIFT 14U

#define MRV_ISP_TPG_FP_V_IN
#define MRV_ISP_TPG_FP_V_IN_MASK 0x00003FFFU
#define MRV_ISP_TPG_FP_V_IN_SHIFT 0U

#define MRV_ISP_TPG_BP_H_IN
#define MRV_ISP_TPG_BP_H_IN_MASK 0x0FFFC000U
#define MRV_ISP_TPG_BP_H_IN_SHIFT 14U

#define MRV_ISP_TPG_BP_V_IN
#define MRV_ISP_TPG_BP_V_IN_MASK 0x00003FFFU
#define MRV_ISP_TPG_BP_V_IN_SHIFT 0U

#define MRV_ISP_TPG_HS_W_IN
#define MRV_ISP_TPG_HS_W_IN_MASK 0x0FFFC000U
#define MRV_ISP_TPG_HS_W_IN_SHIFT 14U

#define MRV_ISP_TPG_VS_W_IN
#define MRV_ISP_TPG_VS_W_IN_MASK 0x00003FFFU
#define MRV_ISP_TPG_VS_W_IN_SHIFT 0U

#define MRV_ISP_TPG_PIX_GAP_IN
#define MRV_ISP_TPG_PIX_GAP_IN_MASK 0x0FFFC000U
#define MRV_ISP_TPG_PIX_GAP_IN_SHIFT 14U

#define MRV_ISP_TPG_LINE_GAP_IN
#define MRV_ISP_TPG_LINE_GAP_IN_MASK 0x00003FFFU
#define MRV_ISP_TPG_LINE_GAP_IN_SHIFT 0U

#define MRV_ISP_TPG_PIX_GAP_STD_IN
#define MRV_ISP_TPG_PIX_GAP_STD_IN_MASK 0x00003FFFU
#define MRV_ISP_TPG_PIX_GAP_STD_IN_SHIFT 0U

#define MRV_ISP_TPG_RANDOM_SEED
#define MRV_ISP_TPG_RANDOM_SEED_MASK 0xFFFFFFFFU
#define MRV_ISP_TPG_RANDOM_SEED_SHIFT 0U

#define MRV_ISP_TPG_FRAME_NUM
#define MRV_ISP_TPG_FRAME_NUM_MASK 0x0000FFFFU
#define MRV_ISP_TPG_FRAME_NUM_SHIFT 0U


int VSI_ISP_CoreSetTpg(ISP_PORT IspPort, ISP_PORT_ATTR_S *pPortAttr, ISP_BAYER_PAT_E bayerPattern, vsi_u8_t bitWidth)
{
    RECT_S *pInFormRect = &pPortAttr->inFormRect;
    vsi_u8_t resolution = 0;

    if (pInFormRect->width <= 1280 && pInFormRect->height <= 720) {
        resolution = 1;
    } else if (pInFormRect->width <= 1920 && pInFormRect->height <= 1080) {
        resolution = 0;
    } else if (pInFormRect->width <= 3840 && pInFormRect->height <= 2160) {
        resolution = 2;
    } else {
        return VSI_FAILURE;
    }

    vsi_reg_t val = 0x00;
    /* set isp tpg */
    REG_SET_SLICE(val, MRV_ISP_TPG_PIX_GAP_IN, 360);
    REG_SET_SLICE(val, MRV_ISP_TPG_LINE_GAP_IN, 640);
    VSI_ISP_CoreWriteReg(IspPort, ISP_TPG_GAP_IN, val);
    val = 0;
    REG_SET_SLICE(val, MRV_ISP_TPG_PIX_GAP_STD_IN, 240);
    VSI_ISP_CoreWriteReg(IspPort, ISP_TPG_GAP_STD_IN, val);
    VSI_ISP_CoreWriteReg(IspPort, ISP_TPG_RANDOM_SEED, 2863311530);
    val = 0;
    REG_SET_SLICE(val, MRV_ISP_TPG_HTOTAL_IN, 2844);
    REG_SET_SLICE(val, MRV_ISP_TPG_VTOTAL_IN, 1968);
    VSI_ISP_CoreWriteReg(IspPort, ISP_TPG_TOTAL_IN, val);
    val = 0;
    REG_SET_SLICE(val, MRV_ISP_TPG_HACT_IN, 1920);
    REG_SET_SLICE(val, MRV_ISP_TPG_VACT_IN, 1080);
    VSI_ISP_CoreWriteReg(IspPort, ISP_TPG_ACT_IN, val);
    val = 0;
    REG_SET_SLICE(val, MRV_ISP_TPG_FP_H_IN, 308);
    REG_SET_SLICE(val, MRV_ISP_TPG_FP_V_IN, 5);
    VSI_ISP_CoreWriteReg(IspPort, ISP_TPG_FP_IN, val);
    val = 0;
    REG_SET_SLICE(val, MRV_ISP_TPG_BP_H_IN, 308);
    REG_SET_SLICE(val, MRV_ISP_TPG_BP_V_IN, 2);
    VSI_ISP_CoreWriteReg(IspPort, ISP_TPG_BP_IN, val);
    val = 0;
    REG_SET_SLICE(val, MRV_ISP_TPG_HS_W_IN, 308);
    REG_SET_SLICE(val, MRV_ISP_TPG_VS_W_IN, 881);
    VSI_ISP_CoreWriteReg(IspPort, ISP_TPG_W_IN, val);
    VSI_ISP_CoreWriteReg(IspPort, ISP_TPG_FRAME_NUM, 0);
    val = 0;
    REG_SET_SLICE(val, MRV_ISP_TPG_IMG_NUM, 0);
    REG_SET_SLICE(val, MRV_ISP_TPG_COLOR_DEPTH, bitWidth);
    REG_SET_SLICE(val, MRV_ISP_TPG_RESOLUTION, resolution);
    REG_SET_SLICE(val, MRV_ISP_TPG_CFA_PAT, bayerPattern);
    REG_SET_SLICE(val, MRV_ISP_TPG_ENABLE, 0);
    VSI_ISP_CoreWriteReg(IspPort, ISP_TPG_CTRL, val);

    return VSI_SUCCESS;
}

int VSI_ISP_CoreSetTpgCtrl(ISP_PORT IspPort, vsi_bool_t tpgEnable)
{
    vsi_reg_t ispTpg  = 0x00;

    VSI_ISP_CoreReadReg(IspPort, ISP_TPG_CTRL, &ispTpg);
    REG_SET_SLICE(ispTpg, MRV_ISP_TPG_ENABLE, tpgEnable);
    VSI_ISP_CoreWriteReg(IspPort, ISP_TPG_CTRL, ispTpg);

    return VSI_SUCCESS;
}