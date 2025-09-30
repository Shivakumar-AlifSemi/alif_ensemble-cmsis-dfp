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

#ifndef __VSI_ISP_DMSC_V10_H__
#define __VSI_ISP_DMSC_V10_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

int VSI_ISP_DmscV10Init(ISP_PORT IspPort);
int VSI_ISP_DmscV10DeInit(ISP_PORT IspPort);
int VSI_ISP_DmscV10GetAttr(ISP_PORT IspPort, ISP_DMSC_ATTR_S *pDmscAttr);
int VSI_ISP_DmscV10SetAttr(ISP_PORT IspPort, ISP_DMSC_ATTR_S *pDmscAttr);
int VSI_ISP_DmscV10IrqProcess(ISP_PORT IspPort);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
