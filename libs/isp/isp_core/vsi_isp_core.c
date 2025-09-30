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
#include "vsios_log.h"
#include "vsios_isp.h"
#include "vsi_comm_video.h"
#include "vsi_comm_isp.h"
#include "vsi_isp_core.h"
#include "vsios_error.h"
#include "isp_conf.h"

#include <vsi_isp_reg_def.h>

//#include "vsi_list.h"
//#include "vsi_isp_irq.h"
#include "vsi_isp_pipeline.h"
#include "vsi_isp_sns.h"
#include "vsi_isp_tpg.h"

/*ISP_CTRL SLICE*/

/* MRV_ISP_ENABLE
 * 0: Disable
 * 1: Enable
 */
#define MRV_ISP_ENABLE
#define MRV_ISP_ENABLE_MASK                 0x00000001U
#define MRV_ISP_ENABLE_SHIFT                0U

/* MRV_ISP_MODE
 * 000 - RAW picture with BT.601 sync (ISP bypass)
 * 001 - ITU-R BT.656 (YUV with embedded sync)
 * 010 - ITU-R BT.601 (YUV input with H and Vsync signals)
 * 011 - Bayer RGB processing with H and Vsync signals
 * 100 - data mode (ISP bypass, sync signals interpreted as data enable)
 * 101 - Bayer RGB processing with BT.656 synchronization
 * 110 - RAW picture with ITU-R BT.656 synchronization (ISP bypass)
 * 111 - reserved
 */
#define MRV_ISP_MODE
#define MRV_ISP_MODE_MASK                   0x0000000EU
#define MRV_ISP_MODE_SHIFT                  1U

/* MRV_INFORM_ENABLE
 * 0: Disable
 * 1: Enable
 */
#define MRV_INFORM_ENABLE
#define MRV_INFORM_ENABLE_MASK              0x00000010U
#define MRV_INFORM_ENABLE_SHIFT             4U

/* MRV_GAMMA_IN_ENABLE
 * 0: Disable
 * 1: Enable
 */
#define MRV_GAMMA_IN_ENABLE
#define MRV_GAMMA_IN_ENABLE_MASK            0x00000040U
#define MRV_GAMMA_IN_ENABLE_SHIFT           6U

/* MRV_AWB_ENABLE
 * 0: Disable
 * 1: Enable
 */
#define MRV_AWB_ENABLE
#define MRV_AWB_ENABLE_MASK                 0x00000080U
#define MRV_AWB_ENABLE_SHIFT                7U

/* MRV_GEN_CFG_UPD_FIX
 * 0: gen_cfg_up is a pulse signal,only effect for current frame
 * 1: gen_cfg_up is a levlel signal, effect since then
 */
#define MRV_GEN_CFG_UPD_FIX
#define MRV_GEN_CFG_UPD_FIX_MASK            0x00000100U
#define MRV_GEN_CFG_UPD_FIX_SHIFT           8U

/* MRV_CFG_UPD
 * 0: No effect
 * 1: Immediately configure shadow register
 */
#define MRV_CFG_UPD
#define MRV_CFG_UPD_MASK                    0x00000200U
#define MRV_CFG_UPD_SHIFT                   9U

/* MRV_GEN_CFG_UPD
 * 0: No effect
 * 1: Genrate frame sync config signale at the output of isp for shadow register
 */
#define MRV_GEN_CFG_UPD
#define MRV_GEN_CFG_UPD_MASK                0x00000400U
#define MRV_GEN_CFG_UPD_SHIFT               10U

/* MRV_GAMMA_OUT_ENABLE
 * 0: Disable
 * 1: Enable
 */
#define MRV_GAMMA_OUT_ENABLE
#define MRV_GAMMA_OUT_ENABLE_MASK           0x00000800U
#define MRV_GAMMA_OUT_ENABLE_SHIFT          11U

/* MRV_FLASH_MODE
 * 0: Sensor interface works independently from flash control unit
 * 1: One frame is captured when signaled by flash control unit
 */
#define MRV_FLASH_MODE
#define MRV_FLASH_MODE_MASK                 0x00001000U
#define MRV_FLASH_MODE_SHIFT                12U

/* MRV_CSM_Y_RANGE
 * 0: Limit range [16-235] or [64-960]
 * 1: Full range [0-255] or [0-1023]
 */
#define MRV_CSM_Y_RANGE
#define MRV_CSM_Y_RANGE_MASK                0x00002000U
#define MRV_CSM_Y_RANGE_SHIFT               13U

/* MRV_CSM_C_RANGE
 * 0: Limit range [16-240] or [64-960]
 * 1: Full range [0-255] or [0-1023]
 */
#define MRV_CSM_C_RANGE
#define MRV_CSM_C_RANGE_MASK                0x00004000U
#define MRV_CSM_C_RANGE_SHIFT               14U

/* MRV_CNR_ENABLE
 * 0: Disable
 * 1: Enable
 */
#define MRV_CNR_ENABLE
#define MRV_CNR_ENABLE_MASK                 0x00020000U
#define MRV_CNR_ENABLE_SHIFT                17U

/* MRV_DPF_RAW_OUT_ENABLE
 * 0: Disable
 * 1: Enable
 */
#define MRV_DPF_RAW_OUT_ENABLE
#define MRV_DPF_RAW_OUT_ENABLE_MASK         0x00040000U
#define MRV_DPF_RAW_OUT_ENABLE_SHIFT        18U

/* MRV_ISP_GAIN_ENABLE
 * 0: Disable
 * 1: Enable
 */
#define MRV_ISP_GAIN_ENABLE
#define MRV_ISP_GAIN_ENABLE_MASK            0x00080000U
#define MRV_ISP_GAIN_ENABLE_SHIFT           19U

#define MRV_ISP_3A_SEL
#define MRV_ISP_3A_SEL_MASK            0x00200000U
#define MRV_ISP_3A_SEL_SHIFT           21U

/*ISP_ACQ_PROP Bit Mask*/

/* MRV_SAMPLE_EDGE
 * 0: Falling edge sample
 * 1: Rising edge sample
 */
#define MRV_SAMPLE_EDGE
#define MRV_SAMPLE_EDGE_MASK                0x00000001U
#define MRV_SAMPLE_EDGE_SHIFT               0U

/* MRV_HSYNC_POL
 * 0: High acitve
 * 1: Low active
 */
#define MRV_HSYNC_POL
#define MRV_HSYNC_POL_MASK                  0x00000002U
#define MRV_HSYNC_POL_SHIFT                 1U

/* MRV_HSYNC_POL
 * 0: High acitve
 * 1: Low active
 */
#define MRV_VSYNC_POL
#define MRV_VSYNC_POL_MASK                  0x00000004U
#define MRV_VSYNC_POL_SHIFT                 2U

/* MRV_BAYER_PAT
 * 0: RGGB
 * 1: GRBG
 * 2: GBRG
 * 3: BGGR
 */
#define MRV_BAYER_PAT
#define MRV_BAYER_PAT_MASK                  0x00000018U
#define MRV_BAYER_PAT_SHIFT                 3U

/* MRV_CONV_422
 * 0: cosited color sub-sampling
 * 1: interleaved color sub-sampling(not recommended)
 * 2: non-cosited color sub-sampling
 */
#define MRV_CONV_422
#define MRV_CONV_422_MASK                   0x00000060U
#define MRV_CONV_422_SHIFT                  5U

/* MRV_CCIR_SEQ
 * 0: YCbYCr
 * 1: YCrYCb
 * 2: CbYCrY
 * 3: CrYCbY
 */
#define MRV_CCIR_SEQ
#define MRV_CCIR_SEQ_MASK                   0x00000180U
#define MRV_CCIR_SEQ_SHIFT                  7U

/* MRV_FILED_SELECTION
 * 0: sample all fileds
 * 1: sample only even fields
 * 2: sample only odd fields
 */
#define MRV_FILED_SELECTION
#define MRV_FILED_SELECTION_MASK            0x00000600U
#define MRV_FILED_SELECTION_SHIFT           9U

#define MRV_FILED_INV
#define MRV_FILED_INV_MASK                  0x00000800U
#define MRV_FILED_INV_SHIFT                 11U

/* MRV_INPUT_SELECTION
 * 0: 12Bit external Interface
 * 1: 10Bit Interface, append 2 zeroes as LSBs
 * 2: 10Bit Interface, append 2 MSBs as LSBs
 * 3: 8Bit Interface,  append 4 zeroes as LSBs
 * 4: 8Bit Interface,  append 4 MSBs as LSBs
 */
#define MRV_INPUT_SELECTION
#define MRV_INPUT_SELECTION_MASK            0x00007000U
#define MRV_INPUT_SELECTION_SHIFT           12U

#define MRV_RGB_DMA_SEL
#define MRV_RGB_DMA_SEL_MASK                0x00008000U
#define MRV_RGB_DMA_SEL_SHIFT               15U

#define MRV_YUV_DMA_SEL
#define MRV_YUV_DMA_SEL_MASK                0x00010000U
#define MRV_YUV_DMA_SEL_SHIFT               16U

#define MRV_PIN_MAPING
#define MRV_PIN_MAPING_MASK                 0x000E0000U
#define MRV_PIN_MAPING_SHIFT                17U

#define MRV_INPUT_BAYER_FORMAT
#define MRV_INPUT_BAYER_FORMAT_MASK         0x00F00000U
#define MRV_INPUT_BAYER_FORMAT_SHIFT        20U

#define MRV_TWO_CYCLE_MERGE_ENABLE
#define MRV_TWO_CYCLE_MERGE_ENABLE_MASK     0x01000000U
#define MRV_TWO_CYCLE_MERGE_ENABLE_SHIFT    24U

#define MRV_CHANGE_TWO_CYCLE_DATA_SEQ
#define MRV_CHANGE_TWO_CYCLE_DATA_SEQ_MASK  0x02000000U
#define MRV_CHANGE_TWO_CYCLE_DATA_SEQ_SHIFT 25U

#define MRV_TWO_CYCLE_DATA_SEL
#define MRV_TWO_CYCLE_DATA_SEL_MASK         0x0C000000U
#define MRV_TWO_CYCLE_DATA_SEL_SHIFT        26U

#define MRV_SAMPLE_EDGE_0
#define MRV_SAMPLE_EDGE_0_MASK              0x10000000U
#define MRV_SAMPLE_EDGE_0_SHIFT             28U

#define MRV_SAMPLE_EDGE_1
#define MRV_SAMPLE_EDGE_1_MASK              0x20000000U
#define MRV_SAMPLE_EDGE_1_SHIFT             29U

#define MRV_HDR_ENABLE
#define MRV_HDR_ENABLE_MASK                 0x80000000U
#define MRV_HDR_ENABLE_SHIFT                31U

/*ISP_IMSC SLICE*/

#define MRV_IMSC_ISP_OFF
#define MRV_IMSC_ISP_OFF_MASK               0x00000001U
#define MRV_IMSC_ISP_OFF_SHIFT              0U

#define MRV_IMSC_FRAME
#define MRV_IMSC_FRAME_MASK                 0x00000002U
#define MRV_IMSC_FRAME_SHIFT                1U

#define MRV_IMSC_DATALOSS
#define MRV_IMSC_DATALOSS_MASK              0x00000004U
#define MRV_IMSC_DATALOSS_SHIFT             2U

#define MRV_IMSC_SIZE_ERR
#define MRV_IMSC_SIZE_ERR_MASK              0x00000008U
#define MRV_IMSC_SIZE_ERR_SHIFT             3U

#define MRV_IMSC_AWB_DONE
#define MRV_IMSC_AWB_DONE_MASK              0x00000010U
#define MRV_IMSC_AWB_DONE_SHIFT             4U

#define MRV_IMSC_FRAME_IN
#define MRV_IMSC_FRAME_IN_MASK              0x00000020U
#define MRV_IMSC_FRAME_IN_SHIFT             5U

#define MRV_IMSC_V_START
#define MRV_IMSC_V_START_MASK               0x00000040U
#define MRV_IMSC_V_START_SHIFT              6U

#define MRV_IMSC_H_START
#define MRV_IMSC_H_START_MASK               0x00000080U
#define MRV_IMSC_H_START_SHIFT              7U

#define MRV_IMSC_FL_ON
#define MRV_IMSC_FL_ON_MASK                 0x00000100U
#define MRV_IMSC_FL_ON_SHIFT                8U

#define MRV_IMSC_FL_OFF
#define MRV_IMSC_FL_OFF_MASK                0x00000200U
#define MRV_IMSC_FL_OFF_SHIFT               9U

#define MRV_IMSC_SH_ON
#define MRV_IMSC_SH_ON_MASK                 0x00000400U
#define MRV_IMSC_SH_ON_SHIFT                10U

#define MRV_IMSC_SH_OFF
#define MRV_IMSC_SH_OFF_MASK                0x00000800U
#define MRV_IMSC_SH_OFF_SHIFT               11U

#define MRV_IMSC_AFM_SUM_OF
#define MRV_IMSC_AFM_SUM_OF_MASK            0x00001000U
#define MRV_IMSC_AFM_SUM_OF_SHIFT           12U

#define MRV_IMSC_AFM_LUM_OF
#define MRV_IMSC_AFM_LUM_OF_MASK            0x00002000U
#define MRV_IMSC_AFM_LUM_OF_SHIFT           13U

#define MRV_IMSC_AFM_FIN
#define MRV_IMSC_AFM_FIN_MASK               0x00004000U
#define MRV_IMSC_AFM_FIN_SHIFT              14U

#define MRV_IMSC_HIST_END
#define MRV_IMSC_HIST_END_MASK              0x00008000U
#define MRV_IMSC_HIST_END_SHIFT             15U

#define MRV_IMSC_FL_CAP
#define MRV_IMSC_FL_CAP_MASK                0x00020000U
#define MRV_IMSC_FL_CAP_SHIFT               17U

#define MRV_IMSC_EXP_END
#define MRV_IMSC_EXP_END_MASK               0x00040000U
#define MRV_IMSC_EXP_END_SHIFT              18U

#define MRV_IMSC_VSM_DONE
#define MRV_IMSC_VSM_DONE_MASK              0x00080000U
#define MRV_IMSC_VSM_DONE_SHIFT             19U


ISP_CORE_DEV_S ispCoreDevs[1];

#define LOGTAG "VSI_ISP_CORE"

ISP_CORE_DEV_S *VSI_ISP_CoreGetDevice(ISP_DEV IspDev)
{
    return &ispCoreDevs[IspDev];
}

ISP_CORE_PORT_S *VSI_ISP_CoreGetPort(ISP_PORT IspPort)
{
    ISP_CORE_DEV_S *ispDev;

    ispDev = VSI_ISP_CoreGetDevice(IspPort.devId);

    return &ispDev->ispPorts[IspPort.portId];
}

ISP_CORE_CHN_S *VSI_ISP_CoreGetChn(ISP_CHN IspChn)
{
    ISP_PORT IspPort;
    ISP_CORE_PORT_S *ispPort;

    IspPort.devId = IspChn.devId;
    IspPort.portId = IspChn.portId;

    ispPort = VSI_ISP_CoreGetPort(IspPort);

    return &ispPort->ispChns[IspChn.chnId];
}

int VSI_ISP_CoreDevInit(ISP_DEV IspDev)
{
    ISP_CORE_DEV_S *ispDev;
    ISP_CORE_PORT_S *ispPort;
    ISP_CORE_CHN_S *ispChn;
    vsi_u32_t portId;
    vsi_u32_t chnId;
    int ret;

    ispDev = VSI_ISP_CoreGetDevice(IspDev);
    vsios_memset(ispDev, 0, sizeof(*ispDev));

    ispDev->devId = IspDev;
    ispDev->ispWorkMode = WORK_MODE_NORMAL;
    for (portId = 0; portId < ISP_PORT_CNT; portId++) {
        ispPort = &ispDev->ispPorts[portId];
        ispPort->portId = portId;
        //vsios_mutex_create(&ispPort->mLock);
        ispPort->ispInputType = INPUT_TYPE_SENSOR;

        ispPort->syncCfg.sampleEdge     = SAMPLE_EDGE_RISING;
        ispPort->syncCfg.hsyncPol       = SYNC_POL_HIGH;
        ispPort->syncCfg.vsyncPol       = SYNC_POL_HIGH;
        ispPort->syncCfg.con422         = CONV422_NOCOSITED;
        ispPort->syncCfg.ccirSeq        = CCIR_YCBYCR;
        ispPort->syncCfg.filedSelection = FIELDSEL_BOTH;
        ispPort->syncCfg.lantencyFifo   = LATENCY_FIFO_INPUT_FMT;

        ispPort->isoAttr.iso = 100;
        ispPort->isoAttr.autoRouteUpd = 0;
        vsi_u32_t autoRoute[ISP_AUTO_STRENGTH_NUN] = {
            100, 200, 400, 800, 1600, 3200, 6400, 12800,
            25600, 51200, 102400, 204800, 409600, 819200, 1638400,3276800};
        vsios_memcpy(&ispPort->isoAttr.autoRoute, &autoRoute, sizeof(autoRoute));

        for (chnId = 0; chnId < ISP_CHN_CNT; chnId++) {
            ispChn = &ispPort->ispChns[chnId];
            ispChn->chnId = chnId;
            //vsios_mutex_create(&ispChn->mLock);
            VSI_VB_QueueInit(&ispChn->vbQueue);
        }
    }

    //ret = vsios_isp_sys_init(IspDev);
    //if (ret) {
    //    VSI_ALOGE("Failed to init isp device");
    //    return ret;
    //}

    // The following reset in the HAL was pointing to TPG reset.
    // This does not make any major issue in our case.
    //vsios_isp_reset(IspDev, 1);
    //vsios_usleep(100);
    //vsios_isp_reset(IspDev, 0);

    /*VI_CCL bit-2 0:enable main clk 1:disable main clk*/
    vsios_isp_write_reg(IspDev, VI_CCL, 0x00000000);
    /*Enable all clocks for sub-modules*/
    vsios_isp_write_reg(IspDev, VI_ICCL, 0xFFFFFFFF);
    /*All sub-modules reset 0:Process stataus 1: reset status*/
    vsios_isp_write_reg(IspDev, VI_IRCL, 0xFFFFFFFF);
    vsios_usleep(5);
    vsios_isp_write_reg(IspDev, VI_IRCL, 0x00000000);

    VSI_ISP_CreatePipeLine(IspDev);
    for (portId = 0; portId < 1; portId++) {
        ISP_PORT IspPort;
        IspPort.devId = IspDev;
        IspPort.portId = portId;
        VSI_ISP_PipelineInit(IspPort);
    }

    return VSI_SUCCESS;
}

int VSI_ISP_CoreDevExit(ISP_DEV IspDev)
{
    int ret;
    vsi_u32_t portId;
    vsi_u32_t chnId;

    ISP_CORE_DEV_S *ispDev;
    ISP_CORE_PORT_S *ispPort;
    ISP_CORE_CHN_S *ispChn;

    ispDev = VSI_ISP_CoreGetDevice(IspDev);

    VSI_ISP_DestroyPipeline(IspDev);

    for (portId = 0; portId < ISP_PORT_CNT; portId++) {
        ispPort = &ispDev->ispPorts[portId];
        //vsios_mutex_destroy(&ispPort->mLock);
        for (chnId = 0; chnId < ISP_CHN_CNT; chnId++) {
            ispChn = &ispPort->ispChns[chnId];
            VSI_VB_QueueDestroy(&ispChn->vbQueue);
            //vsios_mutex_destroy(&ispChn->mLock);
        }
    }

    //ret = vsios_isp_sys_exit(IspDev);
    //if (ret) {
    //    VSI_ALOGE("Failed to exit isp device");
    //    return ret;
    //}

    return VSI_SUCCESS;
}

int VSI_ISP_CoreSetDevAttr(ISP_DEV IspDev, ISP_DEV_ATTR_S *pDevAttr)
{
    ISP_CORE_DEV_S *ispDev;
    ispDev = VSI_ISP_CoreGetDevice(IspDev);

    ispDev->ispWorkMode = pDevAttr->ispWorkMode;

    return VSI_SUCCESS;
}

int VSI_ISP_CoreGetDevAttr(ISP_DEV IspDev, ISP_DEV_ATTR_S *pDevAttr)
{
    ISP_CORE_DEV_S *ispDev;
    ispDev = VSI_ISP_CoreGetDevice(IspDev);

    pDevAttr->ispWorkMode = ispDev->ispWorkMode;

    return VSI_SUCCESS;
}

int VSI_ISP_CoreEnableDev(ISP_DEV IspDev)
{
    ISP_CORE_DEV_S *ispDev;

    ispDev = VSI_ISP_CoreGetDevice(IspDev);

    //vsios_isp_subscribe_event(IspDev);

    ispDev->state = ISP_STATE_STREAMON;

    //vsios_thread_attr_init(&ispDev->threadIrqAttr);

    //vsios_thread_create(&ispDev->threadIrq, &ispDev->threadIrqAttr, VSI_ISP_IrqProcess, ispDev);

    return VSI_SUCCESS;
}

int VSI_ISP_CoreDisableDev(ISP_DEV IspDev)
{
    ISP_CORE_DEV_S *ispDev;
    ispDev = VSI_ISP_CoreGetDevice(IspDev);


    //vsios_isp_unsubscribe_event(IspDev);

    ispDev->state = ISP_STATE_STREAMOFF;

    //vsios_thread_destroy(&ispDev->threadIrq);
    //vsios_thread_attr_destroy(&ispDev->threadIrqAttr);

    return VSI_SUCCESS;
}


static int VSI_ISP_CoreSetPortInput(ISP_PORT IspPort, ISP_PORT_ATTR_S *pPortAttr)
{
    int ret = VSI_SUCCESS;
    vsi_reg_t ispCtrl = 0x00;
    vsi_reg_t ispAcqProp = 0x00;
    vsi_u8_t ispMode;
    ISP_BAYER_PAT_E bayerPattern;
    vsi_u8_t bitWidth = 12;

    VSI_ISP_CoreReadReg(IspPort, ISP_CTRL, &ispCtrl);
    VSI_ISP_CoreReadReg(IspPort, ISP_ACQ_PROP, &ispAcqProp);

    switch (pPortAttr->pixelFormat) {
        case PIXEL_FORMAT_BGGR8:
            ispMode = ISP_MODE_RAW;
            bayerPattern = BAYER_PAT_BGGR;
            bitWidth = 8;
            break;
        case PIXEL_FORMAT_GBRG8:
            ispMode = ISP_MODE_RAW;
            bayerPattern = BAYER_PAT_GBRG;
            bitWidth = 8;
            break;
        case PIXEL_FORMAT_GRBG8:
            ispMode = ISP_MODE_RAW;
            bayerPattern = BAYER_PAT_GRBG;
            bitWidth = 8;
            break;
        case PIXEL_FORMAT_RGGB8:
            ispMode = ISP_MODE_RAW;
            bayerPattern = BAYER_PAT_RGGB;
            bitWidth = 8;
            break;
        case PIXEL_FORMAT_BGGR10:
            ispMode = ISP_MODE_RAW;
            bayerPattern = BAYER_PAT_BGGR;
            bitWidth = 10;
            break;
        case PIXEL_FORMAT_GBRG10:
            ispMode = ISP_MODE_RAW;
            bayerPattern = BAYER_PAT_GBRG;
            bitWidth = 10;
            break;
        case PIXEL_FORMAT_GRBG10:
            ispMode = ISP_MODE_RAW;
            bayerPattern = BAYER_PAT_GRBG;
            bitWidth = 10;
            break;
        case PIXEL_FORMAT_RGGB10:
            ispMode = ISP_MODE_RAW;
            bayerPattern = BAYER_PAT_RGGB;
            bitWidth = 10;
            break;
        case PIXEL_FORMAT_BGGR12:
            ispMode = ISP_MODE_RAW;
            bayerPattern = BAYER_PAT_BGGR;
            bitWidth = 12;
            break;
        case PIXEL_FORMAT_GBRG12:
            ispMode = ISP_MODE_RAW;
            bayerPattern = BAYER_PAT_GBRG;
            bitWidth = 12;
            break;
        case PIXEL_FORMAT_GRBG12:
            ispMode = ISP_MODE_RAW;
            bayerPattern = BAYER_PAT_GRBG;
            break;
        case PIXEL_FORMAT_RGGB12:
            ispMode = ISP_MODE_RAW;
            bayerPattern = BAYER_PAT_RGGB;
            bitWidth = 12;
            break;
        default:
            ret = VSI_ERR_ILLEGAL_PARAM;
            break;
    }

    if (ret) {
        VSI_ALOGE("Invalid input pixel format");
        return ret;
    }

    REG_SET_SLICE(ispCtrl, MRV_ISP_ENABLE, 0);
    REG_SET_SLICE(ispCtrl, MRV_ISP_MODE, ispMode);
    REG_SET_SLICE(ispCtrl, MRV_INFORM_ENABLE, 1);
    REG_SET_SLICE(ispCtrl, MRV_GEN_CFG_UPD_FIX, 1);
    REG_SET_SLICE(ispCtrl, MRV_ISP_3A_SEL, 1);
    VSI_ISP_CoreWriteReg(IspPort, ISP_CTRL, ispCtrl);

    REG_SET_SLICE(ispAcqProp, MRV_SAMPLE_EDGE, pPortAttr->syncCfg.sampleEdge);
    REG_SET_SLICE(ispAcqProp, MRV_HSYNC_POL, pPortAttr->syncCfg.hsyncPol);
    REG_SET_SLICE(ispAcqProp, MRV_VSYNC_POL, pPortAttr->syncCfg.vsyncPol);
    REG_SET_SLICE(ispAcqProp, MRV_BAYER_PAT, bayerPattern);
    REG_SET_SLICE(ispAcqProp, MRV_CONV_422, pPortAttr->syncCfg.con422);
    REG_SET_SLICE(ispAcqProp, MRV_CCIR_SEQ, pPortAttr->syncCfg.ccirSeq);
    REG_SET_SLICE(ispAcqProp, MRV_FILED_SELECTION, pPortAttr->syncCfg.filedSelection);
    REG_SET_SLICE(ispAcqProp, MRV_PIN_MAPING, 2);

    switch (bitWidth) {
        case 8:
            REG_SET_SLICE(ispAcqProp, MRV_INPUT_SELECTION, INPUT_SEL_8BIT_APPEND_ZERO);
            break;
        case 10:
            REG_SET_SLICE(ispAcqProp, MRV_INPUT_SELECTION, INPUT_SEL_10BIT_APPEND_ZERO);
            break;
        case 12:
            REG_SET_SLICE(ispAcqProp, MRV_INPUT_SELECTION, INPUT_SEL_12BIT);
            break;
        default:
            REG_SET_SLICE(ispAcqProp, MRV_INPUT_SELECTION, INPUT_SEL_12BIT);
            break;
    }

    REG_SET_SLICE(ispAcqProp, MRV_RGB_DMA_SEL, pPortAttr->syncCfg.lantencyFifo);

    VSI_ISP_CoreWriteReg(IspPort, ISP_ACQ_PROP, ispAcqProp);

    if (pPortAttr->ispInputType == INPUT_TYPE_TPG) {
        ret = VSI_ISP_CoreSetTpg(IspPort, pPortAttr, bayerPattern, bitWidth);
        if (ret) {
            VSI_ALOGE("VSI_ISP_CoreSetTpg failed");
            return ret;
        }
    } else if (pPortAttr->ispInputType == INPUT_TYPE_SENSOR) {
//        ret = VSI_ISP_SnsSetMode(IspPort, pPortAttr);
//        if (ret) {
//            VSI_ALOGE("VSI_ISP_SnsSetMode failed");
//            return ret;
//        }
    }

    return VSI_SUCCESS;
}

static int VSI_ISP_CoreSetInformRect(ISP_PORT IspPort, RECT_S *pInFormRect)
{
    vsi_reg_t ispAcqHOff = 0x00;
    vsi_reg_t ispAcqVOff = 0x00;
    vsi_reg_t ispAcqHSize = 0x00;
    vsi_reg_t ispAcqVSize = 0x00;

    ispAcqHOff = pInFormRect->top;
    ispAcqVOff = pInFormRect->left;
    ispAcqHSize = pInFormRect->width;
    ispAcqVSize = pInFormRect->height;

    VSI_ISP_CoreWriteReg(IspPort, ISP_ACQ_H_OFFS, ispAcqHOff);
    VSI_ISP_CoreWriteReg(IspPort, ISP_ACQ_V_OFFS, ispAcqVOff);
    VSI_ISP_CoreWriteReg(IspPort, ISP_ACQ_H_SIZE, ispAcqHSize);
    VSI_ISP_CoreWriteReg(IspPort, ISP_ACQ_V_SIZE, ispAcqVSize);

#ifdef ISP_ACQ_NR_NUMBERS
    VSI_ISP_CoreWriteReg(IspPort, ISP_ACQ_NR_FRAMES, 0);
#endif

    return 0;
}

static int VSI_ISP_CoreSetOutformRect(ISP_PORT IspPort, RECT_S *pOutFormRect)
{
    vsi_reg_t ispOutHOff = 0x00;
    vsi_reg_t ispOutVOff = 0x00;
    vsi_reg_t ispOutHSize = 0x00;
    vsi_reg_t ispOutVSize = 0x00;

    ispOutHOff = pOutFormRect->top;
    ispOutVOff = pOutFormRect->left;
    ispOutHSize = pOutFormRect->width;
    ispOutVSize = pOutFormRect->height;

    VSI_ISP_CoreWriteReg(IspPort, ISP_OUT_H_OFFS, ispOutHOff);
    VSI_ISP_CoreWriteReg(IspPort, ISP_OUT_V_OFFS, ispOutVOff);
    VSI_ISP_CoreWriteReg(IspPort, ISP_OUT_H_SIZE, ispOutHSize);
    VSI_ISP_CoreWriteReg(IspPort, ISP_OUT_V_SIZE, ispOutVSize);

    return 0;
}

#ifdef ISP_IS_CROP_V10
static int VSI_ISP_CoreSetIsRect(ISP_PORT IspPort, RECT_S *pIsRect)
{
    vsi_reg_t ispIsHOff = 0x00;
    vsi_reg_t ispIsVOff = 0x00;
    vsi_reg_t ispIsHSize = 0x00;
    vsi_reg_t ispIsVSize = 0x00;

    ispIsHOff = pIsRect->top;
    ispIsVOff = pIsRect->left;
    ispIsHSize = pIsRect->width;
    ispIsVSize = pIsRect->height;

    VSI_ISP_CoreWriteReg(IspPort, ISP_IS_CTRL, 0);
    VSI_ISP_CoreWriteReg(IspPort, ISP_IS_RECENTER, 0);

    VSI_ISP_CoreWriteReg(IspPort, ISP_IS_H_OFFS, ispIsHOff);
    VSI_ISP_CoreWriteReg(IspPort, ISP_IS_V_OFFS, ispIsVOff);
    VSI_ISP_CoreWriteReg(IspPort, ISP_IS_H_SIZE, ispIsHSize);
    VSI_ISP_CoreWriteReg(IspPort, ISP_IS_V_SIZE, ispIsVSize);

    return 0;
}
#endif

#ifdef ISP_AE_V10
extern int VSI_ISP_ExpV10InitAlgo(ISP_PORT IspPort);
extern int VSI_ISP_ExpV10DeInitAlgo(ISP_PORT IspPort);
#endif

#ifdef ISP_WB_V10
extern int VSI_ISP_WbV10InitAlgo(ISP_PORT IspPort);
extern int VSI_ISP_WbV10DeInitAlgo(ISP_PORT IspPort);
#endif

#ifdef ISP_WB_V10_1
extern int VSI_ISP_WbV101InitAlgo(ISP_PORT IspPort);
extern int VSI_ISP_WbV101DeInitAlgo(ISP_PORT IspPort);
#endif

int VSI_ISP_CoreSetPortAttr(ISP_PORT IspPort, ISP_PORT_ATTR_S *pPortAttr)
{
    ISP_CORE_PORT_S *ispPort;
    int ret = VSI_SUCCESS;

    ispPort = VSI_ISP_CoreGetPort(IspPort);

    ret = VSI_ISP_CoreSetPortInput(IspPort, pPortAttr);
    if (ret) {
        VSI_ALOGE("VSI_ISP_CoreSetPortInput failed");
        return ret;
    }

    ret = VSI_ISP_CoreSetInformRect(IspPort, &pPortAttr->inFormRect);
    if (ret) {
        VSI_ALOGE("VSI_ISP_CoreSetInformRect failed");
        return ret;
    }

    ret = VSI_ISP_CoreSetOutformRect(IspPort, &pPortAttr->outFormRect);
    if (ret) {
        VSI_ALOGE("VSI_ISP_CoreSetOutformRect failed");
        return ret;
    }
    ispPort->ispCoreSize.width  = pPortAttr->outFormRect.width;
    ispPort->ispCoreSize.height = pPortAttr->outFormRect.height;

#ifdef ISP_BINNING_V10
    ret = VSI_ISP_BinV10SetNum(IspPort);
    if (ret) {
        VSI_ALOGE("VSI_ISP_BinV10SetNum failed");
        return ret;
    }
#endif

#ifdef ISP_IS_CROP_V10
    ret = VSI_ISP_CoreSetIsRect(IspPort, &pPortAttr->iSRect);
    if (ret) {
        VSI_ALOGE("VSI_ISP_CoreSetIsRect failed");
        return ret;
    }
    ispPort->iSRect       = pPortAttr->iSRect;
#endif

    ispPort->ispInputType = pPortAttr->ispInputType;
    ispPort->ispMode      = pPortAttr->ispMode;
    ispPort->hdrMode      = pPortAttr->hdrMode;
    ispPort->stichMode    = pPortAttr->stichMode;
    ispPort->pixelFormat  = pPortAttr->pixelFormat;
    ispPort->snsRect      = pPortAttr->snsRect;
    ispPort->snsFps      = pPortAttr->snsFps;
    ispPort->syncCfg      = pPortAttr->syncCfg;
    ispPort->inFormRect   = pPortAttr->inFormRect;
    ispPort->outFormRect  = pPortAttr->outFormRect;


#ifdef ISP_AE_V10
    if (pPortAttr->ispInputType == INPUT_TYPE_SENSOR) {
        VSI_ISP_ExpV10InitAlgo(IspPort);
        VSI_ISP_SnsRegUpdate(IspPort);
    }
#endif

#ifdef ISP_WB_V10
    VSI_ISP_WbV10InitAlgo(IspPort);
#endif

#ifdef ISP_WB_V10_1
    VSI_ISP_WbV101InitAlgo(IspPort);
#endif

    VSI_ISP_PipelineInitBySize(IspPort, ispPort->ispCoreSize);

    return VSI_SUCCESS;
}

int VSI_ISP_CoreGetPortAttr(ISP_PORT IspPort, ISP_PORT_ATTR_S *pPortAttr)
{
    ISP_CORE_PORT_S *ispPort;

    ispPort = VSI_ISP_CoreGetPort(IspPort);

    pPortAttr->ispInputType = ispPort->ispInputType;
    pPortAttr->ispMode      = ispPort->ispMode;
    pPortAttr->hdrMode      = ispPort->hdrMode;
    pPortAttr->stichMode    = ispPort->stichMode;
    pPortAttr->pixelFormat  = ispPort->pixelFormat;
    pPortAttr->snsRect      = ispPort->snsRect;
    pPortAttr->snsFps      = ispPort->snsFps;
    pPortAttr->syncCfg      = ispPort->syncCfg;
    pPortAttr->inFormRect   = ispPort->inFormRect;
    pPortAttr->outFormRect  = ispPort->outFormRect;
    pPortAttr->iSRect       = ispPort->iSRect;

    return VSI_SUCCESS;
}

int VSI_ISP_CoreEnablePort(ISP_PORT IspPort)
{
    int ret;
    vsi_reg_t ispCtrl = 0x00;
    vsi_reg_t ispImsc = 0x00;
    ISP_CORE_PORT_S *ispPort;

    ispPort = VSI_ISP_CoreGetPort(IspPort);

    /*isp mis mask*/
    REG_SET_SLICE(ispImsc, MRV_IMSC_ISP_OFF, 1);
    REG_SET_SLICE(ispImsc, MRV_IMSC_FRAME, 1);
    REG_SET_SLICE(ispImsc, MRV_IMSC_DATALOSS, 1);
    REG_SET_SLICE(ispImsc, MRV_IMSC_SIZE_ERR, 1);
    REG_SET_SLICE(ispImsc, MRV_IMSC_AWB_DONE, 1);
    REG_SET_SLICE(ispImsc, MRV_IMSC_FRAME_IN, 1);
    REG_SET_SLICE(ispImsc, MRV_IMSC_V_START, 1);
    REG_SET_SLICE(ispImsc, MRV_IMSC_AFM_SUM_OF, 1);
    REG_SET_SLICE(ispImsc, MRV_IMSC_AFM_LUM_OF, 1);
    REG_SET_SLICE(ispImsc, MRV_IMSC_AFM_FIN, 1);
    REG_SET_SLICE(ispImsc, MRV_IMSC_HIST_END, 1);
    REG_SET_SLICE(ispImsc, MRV_IMSC_EXP_END, 1);
    REG_SET_SLICE(ispImsc, MRV_IMSC_VSM_DONE, 1);
    VSI_ISP_CoreWriteReg(IspPort, ISP_IMSC, ispImsc);

    VSI_ISP_CoreReadReg(IspPort, ISP_CTRL, &ispCtrl);
    REG_SET_SLICE(ispCtrl, MRV_CFG_UPD, 1);
    REG_SET_SLICE(ispCtrl, MRV_ISP_ENABLE, 1);
    VSI_ISP_CoreWriteReg(IspPort, ISP_CTRL, ispCtrl);

    if (ispPort->ispInputType == INPUT_TYPE_SENSOR) {
        //ret = VSI_ISP_SnsStreamOn(IspPort);
    } else if (ispPort->ispInputType == INPUT_TYPE_TPG) {
        ret = VSI_ISP_CoreSetTpgCtrl(IspPort, true);
    }
    if (ret != VSI_SUCCESS) {
        return ret;
    }

    ispPort->state = ISP_STATE_STREAMON;

    return VSI_SUCCESS;
}

int VSI_ISP_CoreDisablePort(ISP_PORT IspPort)
{
    int ret;
    vsi_reg_t ispCtrl = 0x00;
    ISP_CORE_PORT_S *ispPort;
    ispPort = VSI_ISP_CoreGetPort(IspPort);

    if (ispPort->ispInputType == INPUT_TYPE_SENSOR) {
        //ret = VSI_ISP_SnsStreamOff(IspPort);
    } else if (ispPort->ispInputType == INPUT_TYPE_TPG) {
        ret = VSI_ISP_CoreSetTpgCtrl(IspPort, false);
    }
    if (ret != VSI_SUCCESS) {
        return ret;
    }

    VSI_ISP_CoreReadReg(IspPort, ISP_CTRL, &ispCtrl);
    REG_SET_SLICE(ispCtrl, MRV_ISP_ENABLE, 0);
    VSI_ISP_CoreWriteReg(IspPort, ISP_CTRL, ispCtrl);

    VSI_ISP_CoreWriteReg(IspPort, ISP_IMSC, 0);

    ispPort->state = ISP_STATE_STREAMOFF;

    return VSI_SUCCESS;
}

ISP_STATUS_E VSI_ISP_CoreGetPortStatus(ISP_PORT IspPort)
{
    ISP_CORE_PORT_S *ispPort;
    ispPort = VSI_ISP_CoreGetPort(IspPort);

    return ispPort->state;
}

int VSI_ISP_CoreSetIsoAttr(ISP_PORT IspPort, ISP_ISO_ATTR_S *pIsoAttr)
{
    ISP_CORE_PORT_S *ispPort;
    ispPort = VSI_ISP_CoreGetPort(IspPort);

    if (vsios_memcmp(&ispPort->isoAttr.autoRoute, &pIsoAttr->autoRoute, sizeof(pIsoAttr->autoRoute)) != 0) {
        vsios_memcpy(&ispPort->isoAttr.autoRoute, &pIsoAttr->autoRoute, sizeof(pIsoAttr->autoRoute));
        ispPort->isoAttr.autoRouteUpd = 1;
    }

    ispPort->isoAttr.iso = vsios_clamp(vsi_u32_t, pIsoAttr->iso,
                         ispPort->isoAttr.autoRoute[0],
                         ispPort->isoAttr.autoRoute[ISP_AUTO_STRENGTH_NUN - 1]);

    return VSI_SUCCESS;
}

int VSI_ISP_CoreGetIsoAttr(ISP_PORT IspPort, ISP_ISO_ATTR_S *pIsoAttr)
{
    ISP_CORE_PORT_S *ispPort;
    ispPort = VSI_ISP_CoreGetPort(IspPort);

    *pIsoAttr = ispPort->isoAttr;

    return VSI_SUCCESS;
}

static int VSI_ISP_CoreMcmWriteReg(ISP_PORT IspPort, vsi_u32_t reg, vsi_u32_t val)
{
    return VSI_SUCCESS;
}

static int VSI_ISP_CoreMcmReadReg(ISP_PORT IspPort, vsi_u32_t reg, vsi_u32_t *pVal)
{
    return VSI_SUCCESS;
}

int VSI_ISP_CoreWriteReg(ISP_PORT IspPort, vsi_u32_t reg, vsi_u32_t val)
{
    ISP_CORE_DEV_S *ispDev;
    int ret;

    ispDev = VSI_ISP_CoreGetDevice(IspPort.devId);
    if (ispDev->ispWorkMode == WORK_MODE_MCM) {
        ret = VSI_ISP_CoreMcmWriteReg(IspPort, reg, val);
    } else {
        ret = vsios_isp_write_reg(IspPort.devId, reg, val);
    }

    return ret;
}

int VSI_ISP_CoreReadReg(ISP_PORT IspPort, vsi_u32_t reg, vsi_u32_t *pVal)
{
    ISP_CORE_DEV_S *ispDev;
    int ret;

    ispDev = VSI_ISP_CoreGetDevice(IspPort.devId);

    if (ispDev->ispWorkMode == WORK_MODE_MCM) {
        ret = VSI_ISP_CoreMcmReadReg(IspPort, reg, pVal);
    } else {
        ret = vsios_isp_read_reg(IspPort.devId, reg, pVal);
    }

    return ret;
}

int VSI_ISP_CoreSetChnAttr(ISP_CHN IspChn, ISP_CHN_ATTR_S *pChnAttr)
{
    int ret;
    ISP_CORE_CHN_S *ispChn;

    ispChn = VSI_ISP_CoreGetChn(IspChn);

    ret = VSI_MPI_ISP_SetMiChnAttr(IspChn, pChnAttr);
    if (ret) {
        VSI_ALOGE("VSI_MPI_ISP_SetMiChnAttr failed");
        return ret;
    }

    ispChn->transBus  = pChnAttr->transBus;
    ispChn->chnFormat = pChnAttr->chnFormat;

    return VSI_SUCCESS;
}

int VSI_ISP_CoreGetChnAttr(ISP_CHN IspChn, ISP_CHN_ATTR_S *pChnAttr)
{
    ISP_CORE_CHN_S *ispChn;

    ispChn = VSI_ISP_CoreGetChn(IspChn);

    pChnAttr->transBus  = ispChn->transBus;
    pChnAttr->chnFormat = ispChn->chnFormat;

    return VSI_SUCCESS;
}

int VSI_ISP_CoreEnableChnAttr(ISP_CHN IspChn)
{
    int ret;
    ISP_CORE_CHN_S *ispChn;
    ispChn = VSI_ISP_CoreGetChn(IspChn);

    ret = VSI_MPI_ISP_SetMiChnStream(IspChn, 1);
    if (ret) {
        VSI_ALOGE("VSI_MPI_ISP_SetMiChnStream on failed");
        return ret;
    }
    ispChn->state = ISP_STATE_STREAMON;

    VSI_VB_StreamOn(&ispChn->vbQueue);

    return VSI_SUCCESS;
}

int VSI_ISP_CoreDisableChnAttr(ISP_CHN IspChn)
{
    int ret;
    ISP_CORE_CHN_S *ispChn;
    ispChn = VSI_ISP_CoreGetChn(IspChn);

    ret = VSI_MPI_ISP_SetMiChnStream(IspChn, 0);
    if (ret) {
        VSI_ALOGE("VSI_MPI_ISP_SetMiChnStream off failed");
        return ret;
    }
    ispChn->state = ISP_STATE_STREAMOFF;

    VSI_VB_StreamOff(&ispChn->vbQueue);

    return VSI_SUCCESS;
}

int VSI_ISP_CoreChnQBUF(ISP_CHN IspChn, VIDEO_BUF_S *pBuf)
{
    ISP_CORE_CHN_S *ispChn;
    int ret;

    ispChn = VSI_ISP_CoreGetChn(IspChn);

    ret = VSI_VB_QueueBuf(&ispChn->vbQueue, pBuf);

    return ret;
}

int VSI_ISP_CoreChnDQBUF(ISP_CHN IspChn, VIDEO_BUF_S *pBuf, vsi_u32_t timeMs)
{
    ISP_CORE_CHN_S *ispChn;
    int ret ;

    ispChn = VSI_ISP_CoreGetChn(IspChn);

    ret = VSI_VB_DeQueueBuf(&ispChn->vbQueue, pBuf, timeMs);

    return ret;
}
