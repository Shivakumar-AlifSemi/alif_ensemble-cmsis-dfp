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

#ifndef __VSI_ISP_MATH_H__
#define __VSI_ISP_MATH_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

typedef struct vsiISP_AUTO_STREGTH_S {
    vsi_u32_t iso;
    vsi_u32_t minIso;
    vsi_u32_t maxIso;
    vsi_u32_t minLevel;
    vsi_u32_t maxLevel;
} ISP_AUTO_STREGTH_S;

int VSI_ISP_AutoStrength(ISP_PORT IspPort, ISP_AUTO_STREGTH_S *pStrength, ISP_ISO_ATTR_S *pIsoAttr);

int VSI_ISP_U32LinearInterpolate(ISP_AUTO_STREGTH_S *pStrength,
            vsi_u32_t *pValue, vsi_u32_t min, vsi_u32_t max);
int VSI_ISP_S32LinearInterpolate(ISP_AUTO_STREGTH_S *pStrength,
            vsi_s32_t *ps32Value, vsi_s32_t min, vsi_s32_t max);
int VSI_ISP_U16LinearInterpolate(ISP_AUTO_STREGTH_S *pStrength,
            vsi_u16_t *pValue, vsi_u16_t min, vsi_u16_t max);
int VSI_ISP_S16LinearInterpolate(ISP_AUTO_STREGTH_S *pStrength,
            vsi_s16_t *ps16Value, vsi_s16_t min, vsi_s16_t max);
int VSI_ISP_U8LinearInterpolate(ISP_AUTO_STREGTH_S *pStrength,
            vsi_u8_t *pValue, vsi_u8_t min, vsi_u8_t max);
int VSI_ISP_S8LinearInterpolate(ISP_AUTO_STREGTH_S *pStrength,
            vsi_s8_t *ps8Value, vsi_s8_t min, vsi_s8_t max);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif