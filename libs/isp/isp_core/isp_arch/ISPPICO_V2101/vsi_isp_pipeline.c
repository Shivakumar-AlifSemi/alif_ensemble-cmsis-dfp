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

#include "vsi_comm_video.h"
#include "vsi_comm_isp.h"
#include "../../inc/vsi_isp_core.h"
#include "vsi_isp_pipeline.h"
#include <vsi_isp_reg_def.h>

#if RTE_ISP_BLS_MODULE
extern int VSI_ISP_BlsV10Init(ISP_PORT IspPort);
extern int VSI_ISP_BlsV10DeInit(ISP_PORT IspPort);
extern int VSI_ISP_BlsV10IrqProcess(ISP_PORT IspPort);
static ISP_MODULE_NODE_S  blsModule = {
    .ModuleInit       = VSI_ISP_BlsV10Init,
    .ModuleDeInit     = VSI_ISP_BlsV10DeInit,
    .ModuleIrqProcess = VSI_ISP_BlsV10IrqProcess,
};
#endif /* RTE_ISP_BLS_MODULE */

#if RTE_ISP_DMSC_MODULE
extern int VSI_ISP_DmscV10Init(ISP_PORT IspPort);
extern int VSI_ISP_DmscV10DeInit(ISP_PORT IspPort);
extern int VSI_ISP_DmscV10IrqProcess(ISP_PORT IspPort);
static ISP_MODULE_NODE_S  dmscModule = {
    .ModuleInit       = VSI_ISP_DmscV10Init,
    .ModuleDeInit     = VSI_ISP_DmscV10DeInit,
    .ModuleIrqProcess = VSI_ISP_DmscV10IrqProcess,
};
#endif /* RTE_ISP_DMSC_MODULE */

#if RTE_ISP_FLT_MODULE
extern int VSI_ISP_FltV10Init(ISP_PORT IspPort);
extern int VSI_ISP_FltV10DeInit(ISP_PORT IspPort);
extern int VSI_ISP_FltV10IrqProcess(ISP_PORT IspPort);
static ISP_MODULE_NODE_S  fltModule = {
    .ModuleInit       = VSI_ISP_FltV10Init,
    .ModuleDeInit     = VSI_ISP_FltV10DeInit,
    .ModuleIrqProcess = VSI_ISP_FltV10IrqProcess,
};
#endif /* RTE_ISP_FLT_MODULE */

#if RTE_ISP_CCM_MODULE
extern int VSI_ISP_CcmV10Init(ISP_PORT IspPort);
extern int VSI_ISP_CcmV10DeInit(ISP_PORT IspPort);
extern int VSI_ISP_CcmV10IrqProcess(ISP_PORT IspPort);
static ISP_MODULE_NODE_S  ccmModule = {
    .ModuleInit       = VSI_ISP_CcmV10Init,
    .ModuleDeInit     = VSI_ISP_CcmV10DeInit,
    .ModuleIrqProcess = VSI_ISP_CcmV10IrqProcess,
};
#endif /* RTE_ISP_CCM_MODULE */

#if RTE_ISP_GAMMAOUT_MODULE
extern int VSI_ISP_GammaOutV10Init(ISP_PORT IspPort);
extern int VSI_ISP_GammaOutV10DeInit(ISP_PORT IspPort);
extern int VSI_ISP_GammaOutV10IrqProcess(ISP_PORT IspPort);
static ISP_MODULE_NODE_S  gammaOutModule = {
    .ModuleInit       = VSI_ISP_GammaOutV10Init,
    .ModuleDeInit     = VSI_ISP_GammaOutV10DeInit,
    .ModuleIrqProcess = VSI_ISP_GammaOutV10IrqProcess,
};
#endif /* RTE_ISP_GAMMAOUT_MODULE */

#if RTE_ISP_CSM_MODULE
extern int VSI_ISP_CsmV10Init(ISP_PORT IspPort);
extern int VSI_ISP_CsmV10DeInit(ISP_PORT IspPort);
extern int VSI_ISP_CsmV10IrqProcess(ISP_PORT IspPort);
static ISP_MODULE_NODE_S  csmModule = {
    .ModuleInit       = VSI_ISP_CsmV10Init,
    .ModuleDeInit     = VSI_ISP_CsmV10DeInit,
    .ModuleIrqProcess = VSI_ISP_CsmV10IrqProcess,
};
#endif /* RTE_ISP_CSM_MODULE */

#if RTE_ISP_WB_MODULE
extern int VSI_ISP_WbV101Init(ISP_PORT IspPort);
extern int VSI_ISP_WbV101DeInit(ISP_PORT IspPort);
extern int VSI_ISP_WbV101IrqProcess(ISP_PORT IspPort);
static ISP_MODULE_NODE_S  wbModule = {
    .ModuleInit       = VSI_ISP_WbV101Init,
    .ModuleDeInit     = VSI_ISP_WbV101DeInit,
    .ModuleIrqProcess = VSI_ISP_WbV101IrqProcess,
};
#endif /* RTE_ISP_WB_MODULE */

#if RTE_ISP_EXPM_MODULE
extern int VSI_ISP_ExpmV10Init(ISP_PORT IspPort);
extern int VSI_ISP_ExpmV10DeInit(ISP_PORT IspPort);
extern int VSI_ISP_ExpmV10IrqProcess(ISP_PORT IspPort);
extern int VSI_ISP_ExpmV10ReadBin(ISP_PORT IspPort);
extern int VSI_ISP_ExpmV10InitBySize(ISP_PORT IspPort, SIZE_S ispSize);
static ISP_MODULE_NODE_S  expmModule = {
    .ModuleInit       = VSI_ISP_ExpmV10Init,
    .ModuleDeInit     = VSI_ISP_ExpmV10DeInit,
    .ModuleIrqProcess = VSI_ISP_ExpmV10IrqProcess,
    .ModuleInitBySize = VSI_ISP_ExpmV10InitBySize,
};
#endif /* RTE_ISP_EXPM_MODULE */

#if RTE_ISP_AE_MODULE
extern int VSI_ISP_ExpV10Init(ISP_PORT IspPort);
extern int VSI_ISP_ExpV10DeInit(ISP_PORT IspPort);
extern int VSI_ISP_ExpV10IrqProcess(ISP_PORT IspPort);
static ISP_MODULE_NODE_S  aeModule = {
    .ModuleInit       = VSI_ISP_ExpV10Init,
    .ModuleDeInit     = VSI_ISP_ExpV10DeInit,
    .ModuleIrqProcess = VSI_ISP_ExpV10IrqProcess,
};
#endif /* RTE_ISP_AE_MODULE */

#if RTE_ISP_WBM_MODULE
extern int VSI_ISP_WbmV10Init(ISP_PORT IspPort);
extern int VSI_ISP_WbmV10DeInit(ISP_PORT IspPort);
extern int VSI_ISP_WbmV10IrqProcess(ISP_PORT IspPort);
extern int VSI_ISP_WbmV10ReadBin(ISP_PORT IspPort);
extern int VSI_ISP_WbmV10InitBySize(ISP_PORT IspPort, SIZE_S ispSize);
static ISP_MODULE_NODE_S  wbmModule = {
    .ModuleInit       = VSI_ISP_WbmV10Init,
    .ModuleDeInit     = VSI_ISP_WbmV10DeInit,
    .ModuleIrqProcess = VSI_ISP_WbmV10IrqProcess,
    .ModuleInitBySize = VSI_ISP_WbmV10InitBySize,
};
#endif /* RTE_ISP_WBM_MODULE */

int VSI_ISP_CreatePipeLine(ISP_DEV IspDev)
{
    return VSI_SUCCESS;
}

int VSI_ISP_DestroyPipeline(ISP_DEV IspDev)
{
    return VSI_SUCCESS;
}

int VSI_ISP_PipelineInit(ISP_PORT IspPort)
{
#if RTE_ISP_AE_MODULE
    if(aeModule.ModuleInit)
        aeModule.ModuleInit(IspPort);
#endif /* RTE_ISP_AE_MODULE */

#if RTE_ISP_BLS_MODULE
    if(blsModule.ModuleInit)
        blsModule.ModuleInit(IspPort);
#endif /* RTE_ISP_BLS_MODULE */

#if RTE_ISP_DMSC_MODULE
    if(dmscModule.ModuleInit)
        dmscModule.ModuleInit(IspPort);
#endif /* RTE_ISP_DMSC_MODULE */

#if RTE_ISP_FLT_MODULE
    if(fltModule.ModuleInit)
        fltModule.ModuleInit(IspPort);
#endif /* RTE_ISP_FLT_MODULE */

#if RTE_ISP_CCM_MODULE
    if(ccmModule.ModuleInit)
        ccmModule.ModuleInit(IspPort);
#endif /* RTE_ISP_CCM_MODULE */

#if RTE_ISP_CSM_MODULE
    if(csmModule.ModuleInit)
        csmModule.ModuleInit(IspPort);
#endif /* RTE_ISP_CSM_MODULE */

#if RTE_ISP_WB_MODULE
    if(wbModule.ModuleInit)
        wbModule.ModuleInit(IspPort);
#endif /* RTE_ISP_WB_MODULE */

#if RTE_ISP_EXPM_MODULE
    if(expmModule.ModuleInit)
        expmModule.ModuleInit(IspPort);
#endif /* RTE_ISP_EXPM_MODULE */

#if RTE_ISP_GAMMAOUT_MODULE
    if(gammaOutModule.ModuleInit)
        gammaOutModule.ModuleInit(IspPort);
#endif /* RTE_ISP_GAMMAOUT_MODULE */

#if RTE_ISP_WBM_MODULE
    if(wbmModule.ModuleInit)
        wbmModule.ModuleInit(IspPort);
#endif /* RTE_ISP_WBM_MODULE */

    return VSI_SUCCESS;
}

int VSI_ISP_PipelineInitBySize(ISP_PORT IspPort, SIZE_S ispSize)
{
#if RTE_ISP_AE_MODULE
    if(aeModule.ModuleInitBySize)
        aeModule.ModuleInitBySize(IspPort, ispSize);
#endif /* RTE_ISP_AE_MODULE */

#if RTE_ISP_BLS_MODULE
    if(blsModule.ModuleInitBySize)
        blsModule.ModuleInitBySize(IspPort, ispSize);
#endif /* RTE_ISP_BLS_MODULE */

#if RTE_ISP_DMSC_MODULE
    if(dmscModule.ModuleInitBySize)
        dmscModule.ModuleInitBySize(IspPort, ispSize);
#endif /* RTE_ISP_DMSC_MODULE */

#if RTE_ISP_FLT_MODULE
    if(fltModule.ModuleInitBySize)
        fltModule.ModuleInitBySize(IspPort, ispSize);
#endif /* RTE_ISP_FLT_MODULE */

#if RTE_ISP_CCM_MODULE
    if(ccmModule.ModuleInitBySize)
        ccmModule.ModuleInitBySize(IspPort, ispSize);
#endif /* RTE_ISP_CCM_MODULE */

#if RTE_ISP_CSM_MODULE
    if(csmModule.ModuleInitBySize)
        csmModule.ModuleInitBySize(IspPort, ispSize);
#endif /* RTE_ISP_CSM_MODULE */

#if RTE_ISP_WB_MODULE
    if(wbModule.ModuleInitBySize)
        wbModule.ModuleInitBySize(IspPort, ispSize);
#endif /* RTE_ISP_WB_MODULE */

#if RTE_ISP_EXPM_MODULE
    if(expmModule.ModuleInitBySize)
        expmModule.ModuleInitBySize(IspPort, ispSize);
#endif /* RTE_ISP_EXPM_MODULE */

#if RTE_ISP_GAMMAOUT_MODULE
    if(gammaOutModule.ModuleInitBySize)
        gammaOutModule.ModuleInitBySize(IspPort, ispSize);
#endif /* RTE_ISP_GAMMAOUT_MODULE */

#if RTE_ISP_WBM_MODULE
    if(wbmModule.ModuleInitBySize)
        wbmModule.ModuleInitBySize(IspPort, ispSize);
#endif /* RTE_ISP_WBM_MODULE */

    return VSI_SUCCESS;
}

int VSI_ISP_PipelineDeInit(ISP_PORT IspPort)
{
#if RTE_ISP_AE_MODULE
    if(aeModule.ModuleDeInit)
        aeModule.ModuleDeInit(IspPort);
#endif /* RTE_ISP_AE_MODULE */

#if RTE_ISP_BLS_MODULE
    if(blsModule.ModuleDeInit)
        blsModule.ModuleDeInit(IspPort);
#endif /* RTE_ISP_BLS_MODULE */

#if RTE_ISP_DMSC_MODULE
    if(dmscModule.ModuleDeInit)
        dmscModule.ModuleDeInit(IspPort);
#endif /* RTE_ISP_DMSC_MODULE */

#if RTE_ISP_FLT_MODULE
    if(fltModule.ModuleDeInit)
        fltModule.ModuleDeInit(IspPort);
#endif /* RTE_ISP_FLT_MODULE */

#if RTE_ISP_CCM_MODULE
    if(ccmModule.ModuleDeInit)
        ccmModule.ModuleDeInit(IspPort);
#endif /* RTE_ISP_CCM_MODULE */

#if RTE_ISP_CSM_MODULE
    if(csmModule.ModuleDeInit)
        csmModule.ModuleDeInit(IspPort);
#endif /* RTE_ISP_CSM_MODULE */

#if RTE_ISP_WB_MODULE
    if(wbModule.ModuleDeInit)
        wbModule.ModuleDeInit(IspPort);
#endif /* RTE_ISP_WB_MODULE */

#if RTE_ISP_EXPM_MODULE
    if(expmModule.ModuleDeInit)
        expmModule.ModuleDeInit(IspPort);
#endif /* RTE_ISP_EXPM_MODULE */

#if RTE_ISP_GAMMAOUT_MODULE
    if(gammaOutModule.ModuleDeInit)
        gammaOutModule.ModuleDeInit(IspPort);
#endif /* RTE_ISP_GAMMAOUT_MODULE */

#if RTE_ISP_WBM_MODULE
    if(wbmModule.ModuleDeInit)
        wbmModule.ModuleDeInit(IspPort);
#endif /* RTE_ISP_WBM_MODULE */

    return VSI_SUCCESS;
}

int VSI_ISP_PipelineIrqProcess(ISP_PORT IspPort)
{
    return VSI_SUCCESS;
}

int VSI_ISP_PipelineGetStatistics(ISP_PORT IspPort)
{
#if RTE_ISP_EXPM_MODULE
    VSI_ISP_ExpmV10ReadBin(IspPort);
#endif /* RTE_ISP_EXPM_MODULE */

#if RTE_ISP_WBM_MODULE
    VSI_ISP_WbmV10ReadBin(IspPort);
#endif /* RTE_ISP_WBM_MODULE */

    return VSI_SUCCESS;
}
