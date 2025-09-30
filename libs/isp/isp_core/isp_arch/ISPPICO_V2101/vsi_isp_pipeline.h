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

#ifndef __VSI_ISP_PIPELINE_H__
#define __VSI_ISP_PIPELINE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

int VSI_ISP_CreatePipeLine(ISP_DEV IspDev);
int VSI_ISP_DestroyPipeline(ISP_DEV IspDev);

int VSI_ISP_PipelineInit(ISP_PORT IspPort);
int VSI_ISP_PipelineInitBySize(ISP_PORT IspPort, SIZE_S ispSize);
int VSI_ISP_PipelineDeInit(ISP_PORT IspPort);
int VSI_ISP_PipelineIrqProcess(ISP_PORT IspPort);
int VSI_ISP_PipelineGetStatistics(ISP_PORT IspPort);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif