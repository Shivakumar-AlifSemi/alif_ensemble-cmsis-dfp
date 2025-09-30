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

#ifndef __VSI_ISP_SNS_H__
#define __VSI_ISP_SNS_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

int VSI_ISP_SnsRegCallBack(ISP_PORT IspPort, ISP_SNS_OBJ_S *pSnsObj, vsi_u8_t snsDev);
int VSI_ISP_SnsUnRegCallBack(ISP_PORT IspPort);
int VSI_ISP_SnsWriteReg(ISP_PORT IspPort, vsi_u32_t addr, vsi_u32_t data);
int VSI_ISP_SnsReadReg(ISP_PORT IspPort, vsi_u32_t addr, vsi_u32_t *pData);
int VSI_ISP_SnsSetMode(ISP_PORT IspPort, ISP_PORT_ATTR_S *pPortAttr);
int VSI_ISP_SnsStreamOn(ISP_PORT IspPort);
int VSI_ISP_SnsStreamOff(ISP_PORT IspPort);

int VSI_ISP_SnsGetAeDefault(ISP_PORT IspPort, AE_SNS_DEFAULT_S *pAeSnsDft);
int VSI_ISP_SnsSetFps(ISP_PORT IspPort, vsi_u32_t fps);
int VSI_ISP_SnsSlowFrameRate(ISP_PORT IspPort, vsi_u32_t fullLines);
int VSI_ISP_SnsIntTimeUpdate(ISP_PORT IspPort, vsi_u32_t *pIntTime);
int VSI_ISP_SnsGainUpdate(ISP_PORT IspPort, vsi_u32_t *pAgain, vsi_u32_t *pDgain);
int VSI_ISP_SnsSetExpRatio(ISP_PORT IspPort, ISP_EXP_RATIO_S *pExpRatio);
int VSI_ISP_GetSnsRegInfo(ISP_PORT IspPort, ISP_SNS_REGS_INFO_S *pSnsRegsInfo);
int VSI_ISP_SnsRegUpdate(ISP_PORT IspPort);
vsi_bool_t VSI_ISP_SnsCfgIsBusy(ISP_PORT IspPort);
vsi_bool_t VSI_ISP_SnsStreamStatus(ISP_PORT IspPort);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif