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

#ifndef __ISP_MIV10_H__
#define __ISP_MIV10_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

int VSI_ISP_SetMiV10ChnAttr(ISP_CHN IspChn, ISP_CHN_ATTR_S *pChnAttr);
int VSI_ISP_SetMiV10ChnBuf(ISP_CHN IspChn, VIDEO_BUF_S *pBuf);
int VSI_ISP_SetMiV10ChnStream(ISP_CHN IspChn, vsi_u8_t stream);
int VSI_ISP_MiV10MiMisProcess(ISP_DEV IspDev, vsi_u32_t miMis);
int VSI_ISP_MiV10MiMisProcess_Alif(ISP_DEV IspDev, vsi_u32_t miMis);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
