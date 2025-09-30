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
#include "mpi_isp_scale.h"

int VSI_MPI_ISP_SetScaleAttr(ISP_CHN IspChn, ISP_CHN_ATTR_S *pChnAttr)
{
    return VSI_ISP_ScaleV10SetAttr(IspChn, pChnAttr);
}
