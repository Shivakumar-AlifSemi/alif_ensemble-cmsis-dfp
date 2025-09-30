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

#ifndef __VSI_ISP_TPG_H__
#define __VSI_ISP_TPG_H__

int VSI_ISP_CoreSetTpg(ISP_PORT IspPort, ISP_PORT_ATTR_S *pPortAttr, ISP_BAYER_PAT_E bayerPattern, vsi_u8_t bitWidth);
int VSI_ISP_CoreSetTpgCtrl(ISP_PORT IspPort, vsi_bool_t tpgEnable);

#endif
