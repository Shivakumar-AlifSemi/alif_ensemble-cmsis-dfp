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

#include <stdbool.h>
#include "vsios_sys.h"
#include "vsios_isp.h"
#include "vsi_isp_reg_def.h"
#include "vsi_comm_video.h"
#include "vsi_comm_isp.h"
#include "vsi_isp_core.h"
#include "vsi_isp_miv10.h"
#include "mpi_isp_mi.h"

int VSI_MPI_ISP_SetMiChnAttr(ISP_CHN IspChn, ISP_CHN_ATTR_S *pChnAttr)
{
    return VSI_ISP_SetMiV10ChnAttr(IspChn, pChnAttr);
}

int VSI_MPI_ISP_SetMiChnStream(ISP_CHN IspChn, vsi_u8_t stream)
{
    return VSI_ISP_SetMiV10ChnStream(IspChn, stream);
}

int VSI_MPI_ISP_MiIrqProcess(ISP_DEV IspDev, vsi_u32_t miMis)
{
    return VSI_ISP_MiV10MiMisProcess(IspDev, miMis);
}
