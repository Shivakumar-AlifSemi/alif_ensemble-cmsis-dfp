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
#include "vsi_comm_video.h"
#include "vsi_comm_isp.h"
#include "vsi_isp_core.h"
#include "vsi_isp_math.h"

int VSI_ISP_AutoStrength(ISP_PORT IspPort, ISP_AUTO_STREGTH_S *pStrength, ISP_ISO_ATTR_S *pIsoAttr)
{
    VSI_ISP_CoreGetIsoAttr(IspPort, pIsoAttr);

    for (int i = 0; i < ISP_AUTO_STRENGTH_NUN - 1; i++) {
        if ((pIsoAttr->iso >= pIsoAttr->autoRoute[i]) && (pIsoAttr->iso <= pIsoAttr->autoRoute[i + 1])) {
            pStrength->iso = pIsoAttr->iso;
            pStrength->minIso = pIsoAttr->autoRoute[i];
            pStrength->maxIso = pIsoAttr->autoRoute[i + 1];
            pStrength->minLevel = i;
            pStrength->maxLevel = i + 1;
            break;
        }
    }

    return VSI_SUCCESS;
}

int VSI_ISP_U32LinearInterpolate(ISP_AUTO_STREGTH_S *pStrength,
            vsi_u32_t *pValue, vsi_u32_t min, vsi_u32_t max)
{
    if (pStrength->maxIso == pStrength->minIso) {
        *pValue = min;
    } else {
        *pValue = max > min ?
            (vsi_u64_t)(pStrength->iso - pStrength->minIso) *
                    (max - min) /
                    (pStrength->maxIso - pStrength->minIso) + min :
            min - (vsi_u64_t)(pStrength->iso - pStrength->minIso) *
                    (min - max) /
                    (pStrength->maxIso - pStrength->minIso);
    }

    return VSI_SUCCESS;
}

int VSI_ISP_S32LinearInterpolate(ISP_AUTO_STREGTH_S *pStrength,
            vsi_s32_t *ps32Value, vsi_s32_t min, vsi_s32_t max)
{
    if (pStrength->maxIso == pStrength->minIso) {
        *ps32Value = min;
    } else {
        *ps32Value = (vsi_s64_t)(pStrength->iso - pStrength->minIso) *
                    (max - min) /
                    (pStrength->maxIso - pStrength->minIso) + min;
    }

    return VSI_SUCCESS;
}

int VSI_ISP_U16LinearInterpolate(ISP_AUTO_STREGTH_S *pStrength,
            vsi_u16_t *pValue, vsi_u16_t min, vsi_u16_t max)
{
    if (pStrength->maxIso == pStrength->minIso) {
        *pValue = min;
    } else {
        *pValue = max > min ?
            (vsi_u32_t)(pStrength->iso - pStrength->minIso) *
                    (max - min) /
                    (pStrength->maxIso - pStrength->minIso) + min :
            min - (vsi_u32_t)(pStrength->iso - pStrength->minIso) *
                    (min - max ) /
                    (pStrength->maxIso - pStrength->minIso);
    }

    return VSI_SUCCESS;
}

int VSI_ISP_S16LinearInterpolate(ISP_AUTO_STREGTH_S *pStrength,
            vsi_s16_t *ps16Value, vsi_s16_t min, vsi_s16_t max)
{
    if (pStrength->maxIso == pStrength->minIso) {
        *ps16Value = min;
    } else {
        *ps16Value = (vsi_s32_t)(pStrength->iso - pStrength->minIso) *
                    (max - min) /
                    (pStrength->maxIso - pStrength->minIso) + min;
    }

    return VSI_SUCCESS;
}

int VSI_ISP_U8LinearInterpolate(ISP_AUTO_STREGTH_S *pStrength,
            vsi_u8_t *pValue, vsi_u8_t min, vsi_u8_t max)
{
    if (pStrength->maxIso == pStrength->minIso) {
        *pValue = min;
    } else {
        *pValue = max > min ?
            (vsi_u32_t)(pStrength->iso - pStrength->minIso) *
                    (max - min) /
                    (pStrength->maxIso - pStrength->minIso) + min :
            min - (vsi_u32_t)(pStrength->iso - pStrength->minIso) *
                    (min - max) /
                    (pStrength->maxIso - pStrength->minIso);
    }

    return VSI_SUCCESS;
}

int VSI_ISP_S8LinearInterpolate(ISP_AUTO_STREGTH_S *pStrength,
            vsi_s8_t *ps8Value, vsi_s8_t min, vsi_s8_t max)
{
    if (pStrength->maxIso == pStrength->minIso) {
        *ps8Value = min;
    } else {
        *ps8Value = (vsi_s32_t)(pStrength->iso - pStrength->minIso) *
                    (max - min) /
                    (pStrength->maxIso - pStrength->minIso) + min;
    }

    return VSI_SUCCESS;
}

