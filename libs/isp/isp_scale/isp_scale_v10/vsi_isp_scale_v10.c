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
#include "vsi_isp_scale_v10.h"


#define MRV_RSZ_AUTO_UPD
#define MRV_RSZ_AUTO_UPD_MASK 0x00000200U
#define MRV_RSZ_AUTO_UPD_SHIFT 9U

#define MRV_RSZ_CFG_UPD
#define MRV_RSZ_CFG_UPD_MASK 0x00000100U
#define MRV_RSZ_CFG_UPD_SHIFT 8U

#define MRV_RSZ_SCALE_VC_UP
#define MRV_RSZ_SCALE_VC_UP_MASK 0x00000080U
#define MRV_RSZ_SCALE_VC_UP_SHIFT 7U

#define MRV_RSZ_SCALE_VY_UP
#define MRV_RSZ_SCALE_VY_UP_MASK 0x00000040U
#define MRV_RSZ_SCALE_VY_UP_SHIFT 6U

#define MRV_RSZ_SCALE_HC_UP
#define MRV_RSZ_SCALE_HC_UP_MASK 0x00000020U
#define MRV_RSZ_SCALE_HC_UP_SHIFT 5U

#define MRV_RSZ_SCALE_HY_UP
#define MRV_RSZ_SCALE_HY_UP_MASK 0x00000010U
#define MRV_RSZ_SCALE_HY_UP_SHIFT 4U

#define MRV_RSZ_SCALE_VC_ENABLE
#define MRV_RSZ_SCALE_VC_ENABLE_MASK 0x00000008U
#define MRV_RSZ_SCALE_VC_ENABLE_SHIFT 3U

#define MRV_RSZ_SCALE_VY_ENABLE
#define MRV_RSZ_SCALE_VY_ENABLE_MASK 0x00000004U
#define MRV_RSZ_SCALE_VY_ENABLE_SHIFT 2U

#define MRV_RSZ_SCALE_HC_ENABLE
#define MRV_RSZ_SCALE_HC_ENABLE_MASK 0x00000002U
#define MRV_RSZ_SCALE_HC_ENABLE_SHIFT 1U

#define MRV_RSZ_SCALE_HY_ENABLE
#define MRV_RSZ_SCALE_HY_ENABLE_MASK 0x00000001U
#define MRV_RSZ_SCALE_HY_ENABLE_SHIFT 0U

#define MRV_RSZ_SCALE_HY
#define MRV_RSZ_SCALE_HY_MASK 0x0000FFFFU
#define MRV_RSZ_SCALE_HY_SHIFT 0U

#define MRV_RSZ_SCALE_HCB
#define MRV_RSZ_SCALE_HCB_MASK 0x0000FFFFU
#define MRV_RSZ_SCALE_HCB_SHIFT 0U

#define MRV_RSZ_SCALE_VY
#define MRV_RSZ_SCALE_VY_MASK 0x0000FFFFU
#define MRV_RSZ_SCALE_VY_SHIFT 0U

#define MRV_RSZ_SCALE_VC
#define MRV_RSZ_SCALE_VC_MASK 0x0000FFFFU
#define MRV_RSZ_SCALE_VC_SHIFT 0U

#define MRV_RSZ_PHASE_HY
#define MRV_RSZ_PHASE_HY_MASK 0x0000FFFFU
#define MRV_RSZ_PHASE_HY_SHIFT 0U

#define MRV_RSZ_PHASE_HC
#define MRV_RSZ_PHASE_HC_MASK 0x0000FFFFU
#define MRV_RSZ_PHASE_HC_SHIFT 0U

#define MRV_RSZ_PHASE_VY
#define MRV_RSZ_PHASE_VY_MASK 0x0000FFFFU
#define MRV_RSZ_PHASE_VY_SHIFT 0U

#define MRV_RSZ_PHASE_VC
#define MRV_RSZ_PHASE_VC_MASK 0x0000FFFFU
#define MRV_RSZ_PHASE_VC_SHIFT 0U

#define MRV_RSZ_SCALE_LUT_ADDR
#define MRV_RSZ_SCALE_LUT_ADDR_MASK 0x0000003FU
#define MRV_RSZ_SCALE_LUT_ADDR_SHIFT 0U

#define MRV_RSZ_SCALE_LUT
#define MRV_RSZ_SCALE_LUT_MASK 0x0000003FU
#define MRV_RSZ_SCALE_LUT_SHIFT 0U


static vsi_u32_t VSI_ISP_ScaleV10GetScaleFactor(vsi_u32_t src, vsi_u32_t dst)
{
    if (dst > src) {
        return ((65536 * (src - 1)) / (dst - 1));
    } else if (dst < src) {
        return ((65536 * (dst - 1)) / (src - 1)) + 1;
    }
    return 65536;
}


int VSI_ISP_ScaleV10SetAttr(ISP_CHN IspChn, ISP_CHN_ATTR_S *pChnAttr)
{
    vsi_bool_t rezEnable = 0;
    vsi_bool_t hScale = 0, vScale = 0;
    vsi_u32_t rszCtrlAddr = 0x00;
    vsi_u32_t rszCtrl = 0x00;
    vsi_u32_t iw, ih, ow, oh;
    vsi_u32_t inWidth, inHeight, outWidth, outHeight;
    vsi_u32_t scale_hy, scale_hcb, scale_hcr, scale_vy, scale_vc;
    ISP_PORT IspPort;
    ISP_CORE_PORT_S *ispPort;

    FORMAT_S *pFormat = &pChnAttr->chnFormat;

    IspPort.devId = IspChn.devId;
    IspPort.portId = IspChn.portId;
    ispPort = VSI_ISP_CoreGetPort(IspPort);

#ifdef ISP_IS_CROP_V10
    inWidth = ispPort->iSRect.width;
    inHeight = ispPort->iSRect.height;
#else
    inWidth = ispPort->ispCoreSize.width;
    inHeight = ispPort->ispCoreSize.height;
#endif

    outWidth = pFormat->width;
    outHeight = pFormat->height;

    if (outWidth > inWidth) {
        return VSI_ERR_ILLEGAL_PARAM;
    } else if (outWidth < inWidth) {
        hScale = 1;
    }

    if (outHeight > inHeight) {
        return VSI_ERR_ILLEGAL_PARAM;
    } else if (outHeight < inHeight) {
        vScale = 1;
    }

    if (pFormat->pixelFormat == PIXEL_FORMAT_NV12) {
        vScale = 1;
    }

    if (!(hScale || vScale)) {
        return 0;
    }

    iw = inWidth >> 1;
    ih = inHeight;
    ow = outWidth >> 1;

    switch (pFormat->pixelFormat) {
    case PIXEL_FORMAT_NV16:
    case PIXEL_FORMAT_YUYV:
    case PIXEL_FORMAT_YUV422P:
    case PIXEL_FORMAT_YUV444:
    case PIXEL_FORMAT_YUV400:
    case PIXEL_FORMAT_RGB888:
    case PIXEL_FORMAT_RGB888P:
        oh = outHeight;
        break;
    case PIXEL_FORMAT_YUV420P:
    case PIXEL_FORMAT_NV12:
        oh = outHeight >> 1;	/*  scale cbcr */
        break;
    default:
        return VSI_ERR_ILLEGAL_PARAM;
    }

    scale_hy = VSI_ISP_ScaleV10GetScaleFactor(inWidth, outWidth);
    scale_vy = VSI_ISP_ScaleV10GetScaleFactor(inHeight, outHeight);
    scale_hcr = VSI_ISP_ScaleV10GetScaleFactor(iw, ow);
    scale_hcb = scale_hcr;
    scale_vc = VSI_ISP_ScaleV10GetScaleFactor(ih, oh);

    switch (IspChn.chnId) {
#ifdef ISP_MP_SCALE
        case CHN_ID_MP:
            rszCtrlAddr = MRSZ_CTRL;
            VSI_ISP_CoreReadReg(IspPort, MRSZ_CTRL, &rszCtrl);
            break;
#endif
#ifdef ISP_SP1_SCALE
        case CHN_ID_SP1:
            rszCtrlAddr = SRSZ_CTRL;
            VSI_ISP_CoreReadReg(IspPort, SRSZ_CTRL, &rszCtrl);
            break;
#endif
        default:
            return VSI_ERR_ILLEGAL_PARAM;
    }

    REG_SET_SLICE(rszCtrl, MRV_RSZ_AUTO_UPD, 1);
    REG_SET_SLICE(rszCtrl, MRV_RSZ_CFG_UPD, 1);
    REG_SET_SLICE(rszCtrl, MRV_RSZ_SCALE_VC_UP, 0);
    REG_SET_SLICE(rszCtrl, MRV_RSZ_SCALE_VY_UP, 0);
    REG_SET_SLICE(rszCtrl, MRV_RSZ_SCALE_HC_UP, 0);
    REG_SET_SLICE(rszCtrl, MRV_RSZ_SCALE_HY_UP, 0);
    REG_SET_SLICE(rszCtrl, MRV_RSZ_SCALE_VC_ENABLE, ih != oh);
    REG_SET_SLICE(rszCtrl, MRV_RSZ_SCALE_VY_ENABLE, outHeight < inHeight);
    REG_SET_SLICE(rszCtrl, MRV_RSZ_SCALE_HC_ENABLE, iw != ow);
    REG_SET_SLICE(rszCtrl, MRV_RSZ_SCALE_HY_ENABLE, hScale);

	VSI_ISP_CoreWriteReg(IspPort, rszCtrlAddr + 0x24U, 0);
	VSI_ISP_CoreWriteReg(IspPort, rszCtrlAddr + 0x20U, 0);
	VSI_ISP_CoreWriteReg(IspPort, rszCtrlAddr + 0x1cU, 0);
	VSI_ISP_CoreWriteReg(IspPort, rszCtrlAddr + 0x18U, 0);
    VSI_ISP_CoreWriteReg(IspPort, rszCtrlAddr + 0x14U, scale_vc);
    VSI_ISP_CoreWriteReg(IspPort, rszCtrlAddr + 0x10U, scale_vy);
    VSI_ISP_CoreWriteReg(IspPort, rszCtrlAddr + 0xcU, scale_hcr);
    VSI_ISP_CoreWriteReg(IspPort, rszCtrlAddr + 0x8U, scale_hcb);
    VSI_ISP_CoreWriteReg(IspPort, rszCtrlAddr + 0x4U, scale_hy);
    VSI_ISP_CoreWriteReg(IspPort, rszCtrlAddr, rszCtrl);

    return VSI_SUCCESS;
}
