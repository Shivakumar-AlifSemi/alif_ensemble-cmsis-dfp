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
#include "vsios_log.h"
#include "vsi_isp_reg_def.h"
#include "vsi_comm_video.h"
#include "vsi_comm_isp.h"
#include "vsi_isp_core.h"
#include "vsi_isp_miv10.h"
#include "vsi_vb_queue.h"
#include "vsi_isp_pipeline.h"

#include "isp_conf.h"

/* VI_DPCL SLICE */
/* MRV_MP_MUX
 * 00 : reserved
 * 01 : Data from main resize to mi
 */
#define MRV_MP_MUX
#define MRV_MP_MUX_MASK                     0x00000003U
#define MRV_MP_MUX_SHIFT                    0U

/* MRV_VI_CHAN_MODE
 * 00 : Disable
 * 01 : Main path and raw data mode
 */
#define MRV_VI_CHAN_MODE
#define MRV_VI_CHAN_MODE_MASK               0x0000000CU
#define MRV_VI_CHAN_MODE_SHIFT              2U

/* MRV_IF_SELECT0
 * 0 : Parallel interface
 * 1 : SMIA interface
 * 2 : unused
 * 3 : HDR stiching output
 */
#define MRV_IF_SELECT0
#define MRV_IF_SELECT0_MASK                 0x00000300U
#define MRV_IF_SELECT0_SHIFT                8U

/* MI_CTRL SLICE */
/* MRV_MI_PATH_ENABLE
 * 0000: Disabled, no data is transferred
 * 0001: MP YUV data output, mainpath only (mp_enable only)
 * 0010: SP1 YUV data output, self path only (sp1_enable only)
 * 0011: mp and sp YUV path enable
 * 0100: jpeg enbale ,but in project,jpeg is not used ,so reserved
 * 0110: sp YUV and jpeg enable ,jpeg is not used
 * 1000: raw enable
 * 1001: mp YUV and dp enable,dp use sp path
 * 1010: sp Yuv and dp enable ,dp use mp path
 * 1100: jpeg and dp enable,dp use sp path ,jpeg is not used
 * 1101: dp enable ,dp use mp path
 * 1110: dp enable ,dp use sp path
 * 1111: raw and dp enable ,dp use sp path
 */
#define MRV_MI_PATH_ENABLE
#define MRV_MI_PATH_ENABLE_MASK             0x0000000FU
#define MRV_MI_PATH_ENABLE_SHIFT            0U

/* MRV_MP_PINGPONG_ENABLE */
#define MRV_MP_PINGPONG_ENABLE
#define MRV_MP_PINGPONG_ENABLE_MASK         0x00000800U
#define MRV_MP_PINGPONG_ENABLE_SHIFT        11U

/* MRV_SP_PINGPONG_ENABLE */
#define MRV_SP_PINGPONG_ENABLE
#define MRV_SP_PINGPONG_ENABLE_MASK         0x00001000U
#define MRV_SP_PINGPONG_ENABLE_SHIFT        12U

/*MRV_MP_AUTO_UPDATE*/
#define MRV_MP_AUTO_UPDATE
#define MRV_MP_AUTO_UPDATE_MASK             0x00002000U
#define MRV_MP_AUTO_UPDATE_SHIFT            13U

/* MRV_SP_AUTO_UPDATE */
#define MRV_SP_AUTO_UPDATE
#define MRV_SP_AUTO_UPDATE_MASK             0x00004000U
#define MRV_SP_AUTO_UPDATE_SHIFT            14U

/* MRV_BURST_LEN_LUM
 * 0: 4-bit burst
 * 1: 8-bit burst
 * 2: 16-bit burst
 */
#define MRV_BURST_LEN_LUM
#define MRV_BURST_LEN_LUM_MASK              0x00030000U
#define MRV_BURST_LEN_LUM_SHIFT             16U

/* MRV_BURST_LEN_CHROM
 * 0: 4-bit burst
 * 1: 8-bit burst
 * 2: 16-bit burst
 */
#define MRV_BURST_LEN_CHROM
#define MRV_BURST_LEN_CHROM_MASK            0x000C0000U
#define MRV_BURST_LEN_CHROM_SHIFT           18U

/* MRV_INIT_BASE_ENABLE */
#define MRV_INIT_BASE_ENABLE
#define MRV_INIT_BASE_ENABLE_MASK           0x00100000U
#define MRV_INIT_BASE_ENABLE_SHIFT          20U

/* MRV_INIT_OFFSET_ENABLE */
#define MRV_INIT_OFFSET_ENABLE
#define MRV_INIT_OFFSET_ENABLE_MASK         0x00200000U
#define MRV_INIT_OFFSET_ENABLE_SHIFT        21U

/* MRV_MP_WRITE_FORMAT
 * In YCbCr mode the following meaning is applicable:
 * 00: Planar
 * 01: Semi planar, for YCbCr 4:2:x
 * 10: Interleaved (combined), for YCbCr 4:2:2 only
 * In RAW data mode the following meaning is applicable:
 * 00: RAW 8 bit
 * 01: RAW 10 bit
 * 10: RAW 12 bit
 */
#define MRV_MP_WRITE_FORMAT
#define MRV_MP_WRITE_FORMAT_MASK            0x00C00000U
#define MRV_MP_WRITE_FORMAT_SHIFT           22U

/* MRV_SP_WRITE_FORMAT */
#define MRV_SP_WRITE_FORMAT
#define MRV_SP_WRITE_FORMAT_MASK            0x03000000U
#define MRV_SP_WRITE_FORMAT_SHIFT           24U

/* MRV_SP_INPUT_FORMAT */
#define MRV_SP_INPUT_FORMAT
#define MRV_SP_INPUT_FORMAT_MASK            0x0C000000U
#define MRV_SP_INPUT_FORMAT_SHIFT           26U

/* MRV_SP_OUTPUT_FORMAT
 * 000: YCbCr 4:0:0  not supported
 * 001: YCbCr 4:2:0
 * 010: YCbCr 4:2:2
 * 011: YCbCr 4:4:4
 * 100: RGB565
 * 101: RGB666
 * 110: RGB888
 */
#define MRV_SP_OUTPUT_FORMAT
#define MRV_SP_OUTPUT_FORMAT_MASK           0x70000000U
#define MRV_SP_OUTPUT_FORMAT_SHIFT          28U

/* MI_INIT SLICE */

/* MRV_MI_SKIP */
#define MRV_MI_SKIP
#define MRV_MI_SKIP_MASK                    0x00000004U
#define MRV_MI_SKIP_SHIFT                  2U

/* MRV_MI_CFG_UPD */
#define MRV_MI_CFG_UPD
#define MRV_MI_CFG_UPD_MASK                 0x00000010U
#define MRV_MI_CFG_UPD_SHIFT                4U

/* MRV_MP_OUTPUT_FORMAT
 * 000: YCbCr 4:0:0  not supported
 * 001: YCbCr 4:2:0
 * 010: YCbCr 4:2:2
 * 011: YCbCr 4:4:4
 * 101: RAW8
 */
#define MRV_MP_OUTPUT_FORMAT
#define MRV_MP_OUTPUT_FORMAT_MASK           0x000001E0U
#define MRV_MP_OUTPUT_FORMAT_SHIFT          5U

/* MI_OUTPUT_ALIGN_FORMAT SLICE */

/* MRV_MP_LSB_ALIGN
 * 0: MSB ALIGNED
 * 1: LSB ALIGNED
 */
#define MRV_MP_LSB_ALIGN
#define MRV_MP_LSB_ALIGN_MASK               0x00000001U
#define MRV_MP_LSB_ALIGN_SHIFT              0U

/* MRV_MP_BYTE_SWAP
 * 001: ABCDEFGH => BADCFEHG
 * 000: ABCDEFGH => ABCDEFGH
 * 010: ABCDEFGH => CDABGHEF
 * 011: ABCDEFGH => DCBAHGFE
 * 100: ABCDEFGH => EFGHABCD
 * 101: ABCDEFGH => FEHGBADC
 * 110: ABCDEFGH => GHEFCDAB
 * 111: ABCDEFGH => HGFEDCBA
 */
#define MRV_MP_BYTE_SWAP
#define MRV_MP_BYTE_SWAP_MASK               0x0000000EU
#define MRV_MP_BYTE_SWAP_SHIFT              1U

/* MRV_SP_BYTE_SWAP
 * 001: ABCDEFGH => BADCFEHG
 * 000: ABCDEFGH => ABCDEFGH
 * 010: ABCDEFGH => CDABGHEF
 * 011: ABCDEFGH => DCBAHGFE
 * 100: ABCDEFGH => EFGHABCD
 * 101: ABCDEFGH => FEHGBADC
 * 110: ABCDEFGH => GHEFCDAB
 * 111: ABCDEFGH => HGFEDCBA
 */
#define MRV_SP_BYTE_SWAP
#define MRV_SP_BYTE_SWAP_MASK                0x00000070U
#define MRV_SP_BYTE_SWAP_SHIFT               4U

/* MRV_DMA_BYTE_SWAP
 * 001: ABCDEFGH => BADCFEHG
 * 000: ABCDEFGH => ABCDEFGH
 * 010: ABCDEFGH => CDABGHEF
 * 011: ABCDEFGH => DCBAHGFE
 * 100: ABCDEFGH => EFGHABCD
 * 101: ABCDEFGH => FEHGBADC
 * 110: ABCDEFGH => GHEFCDAB
 * 111: ABCDEFGH => HGFEDCBA
 */
#define MRV_DMA_BYTE_SWAP
#define MRV_DMA_BYTE_SWAP_MASK              0x00000380U
#define MRV_DMA_BYTE_SWAP_SHIFT             7U


/* MI_IMSC SLICE */

#define MRV_MP_FRAME_END
#define MRV_MP_FRAME_END_MASK               0x00000001U
#define MRV_MP_FRAME_END_SHIFT              0U

#define MRV_SP_FRAME_END
#define MRV_SP_FRAME_END_MASK               0x00000002U
#define MRV_SP_FRAME_END_SHIFT              1U

#define MRV_MBLK_LINE
#define MRV_MBLK_LINE_MASK                  0x00000004U
#define MRV_MBLK_LINE_SHIFT                 2U

#define MRV_FILL_MP_Y
#define MRV_FILL_MP_Y_MASK                  0x00000008U
#define MRV_FILL_MP_Y_SHIFT                 3U

#define MRV_WRAP_MP_Y
#define MRV_WRAP_MP_Y_MASK                  0x00000010U
#define MRV_WRAP_MP_Y_SHIFT                 4U

#define MRV_WRAP_MP_CB
#define MRV_WRAP_MP_CB_MASK                 0x00000020U
#define MRV_WRAP_MP_CB_SHIFT                5U

#define MRV_WRAP_MP_CR
#define MRV_WRAP_MP_CR_MASK                 0x00000040U
#define MRV_WRAP_MP_CR_SHIFT                6U

#define MRV_WRAP_SP_Y
#define MRV_WRAP_SP_Y_MASK                  0x00000080U
#define MRV_WRAP_SP_Y_SHIFT                 7U

#define MRV_WRAP_SP_CB
#define MRV_WRAP_SP_CB_MASK                 0x00000100U
#define MRV_WRAP_SP_CB_SHIFT                8U

#define MRV_WRAP_SP_CR
#define MRV_WRAP_SP_CR_MASK                 0x00000200U
#define MRV_WRAP_SP_CR_SHIFT                9U

#define MRV_DMA_READY
#define MRV_DMA_READY_MASK                  0x00000800U
#define MRV_DMA_READY_SHIFT                 11U

#define MRV_DMA_HANDSINK_INT
#define MRV_DMA_HANDSINK_INT_MASK           0x00001000U
#define MRV_DMA_HANDSINK_INT_SHIFT          12U

#define MRV_BP_FRAME_END
#define MRV_BP_FRAME_END_MASK               0x00004000U
#define MRV_BP_FRAME_END_SHIFT              14U

#define MRV_FILL_BP_R
#define MRV_FILL_BP_R_MASK                  0x00008000U
#define MRV_FILL_BP_R_SHIFT                 15U

#define MRV_WARP_BP_R
#define MRV_WARP_BP_R_MASK                  0x00010000U
#define MRV_WARP_BP_R_SHIFT                 16U

#define MRV_WARP_BP_GR
#define MRV_WARP_BP_GR_MASK                 0x00020000U
#define MRV_WARP_BP_GR_SHIFT                17U

#define MRV_WARP_BP_GB
#define MRV_WARP_BP_GB_MASK                 0x00040000U
#define MRV_WARP_BP_GB_SHIFT                18U

#define MRV_WARP_BP_B
#define MRV_WARP_BP_B_MASK                  0x00080000U
#define MRV_WARP_BP_B_SHIFT                 19U

#define MRV_CR_OUTSYNC_INT
#define MRV_CR_OUTSYNC_INT_MASK            0x00100000U
#define MRV_CR_OUTSYNC_INT_SHIFT           20U

#define MRV_CR_TIMEOUT_INT
#define MRV_CR_TIMEOUT_INT_MASK            0x00200000U
#define MRV_CR_TIMEOUT_INT_SHIFT           21U

#define MRV_CB_OUTSYNC_INT
#define MRV_CB_OUTSYNC_INT_MASK            0x00400000U
#define MRV_CB_OUTSYNC_INT_SHIFT           22U

#define MRV_CB_TIMEOUT_INT
#define MRV_CB_TIMEOUT_INT_MASK            0x00800000U
#define MRV_CB_TIMEOUT_INT_SHIFT           23U

#define MRV_Y_OUTSYNC_INT
#define MRV_Y_OUTSYNC_INT_MASK             0x01000000U
#define MRV_Y_OUTSYNC_INT_SHIFT            24U

#define MRV_Y_TIMEOUT_INT
#define MRV_Y_TIMEOUT_INT_MASK            0x02000000U
#define MRV_Y_TIMEOUT_INT_SHIFT           25U

#define MRV_RSZ_OUTPUT_FORMAT
#define MRV_RSZ_OUTPUT_FORMAT_MASK        0x0000001CU
#define MRV_RSZ_OUTPUT_FORMAT_SHIFT       2U

#define MRV_RSZ_INPUT_FORMAT
#define MRV_RSZ_INPUT_FORMAT_MASK         0x00000003U
#define MRV_RSZ_INPUT_FORMAT_SHIFT        0U

#define MRV_ISP_MODE
#define MRV_ISP_MODE_MASK                   0x0000000EU
#define MRV_ISP_MODE_SHIFT                  1U

#define LOGTAG "VSI_ISP_MI"

typedef enum vsiMIV1_YUV_WRITE_FORMAT_E {
    MIV1_YUV_PLANER      = 0,
    MIV1_YUV_SEMIPLANER  = 1,
    MIV1_YUV_INTERLEAVED = 2,
    MIV1_YUV_YONLY       = 3,
} MIV1_YUV_WRITE_FORMAT_E;

typedef enum vsiMIV1_RAW_WRITE_FORMAT_E {
    MIV1_RAW8      = 0,
    MIV1_RAW10     = 1,
    MIV1_RAW12     = 2,
} MIV1_RAW_WRITE_FORMAT_E;

typedef enum vsiMIV1_OUTPUT_FORMAT_E {
    MIV1_FMT_YUV400 = 0,
    MIV1_FMT_YUV420 = 1,
    MIV1_FMT_YUV422 = 2,
    MIV1_FMT_YUV444 = 3,
    MIV1_FMT_RGB565 = 4,
    MIV1_FMT_RGB666 = 5,
    MIV1_FMT_RGB888 = 6,
} MIV1_OUTPUT_FORMAT_E;


typedef enum vsiMIV1_RAW_OUTPUT_FORMAT_E {
    MIV1_FMT_RAW12  = 4,
    MIV1_FMT_RAW8   = 5,
    MIV1_FMT_RAW10  = 7,
} MIV1_RAW_OUTPUT_FORMAT_E;


static bool VSI_ISP_MiV10FormatIsRaw(PIXEL_FORMAT_E pixelFmt)
{
    bool bIsRaw = false;
    switch (pixelFmt) {
        case PIXEL_FORMAT_RAW8:
        case PIXEL_FORMAT_RAW10:
        case PIXEL_FORMAT_RAW12:
            bIsRaw = true;
            break;
        default:
            bIsRaw = false;
            break;
    }

    return bIsRaw;
}

static int VSI_ISP_SetMiV10ChnFmt(ISP_CHN IspChn, ISP_CHN_ATTR_S *pChnAttr)
{
    vsi_u8_t writeFmt = 0;
    vsi_u8_t outFmt = 0;
    vsi_reg_t miCtrl = 0x00;
    vsi_reg_t miInit = 0x00;
    vsi_u32_t rszFmCtrl = 0x00;
    vsi_reg_t alignFmt = 0x00;
    vsi_reg_t ispCtrl = 0x00;
    int ret = VSI_SUCCESS;
    ISP_PORT IspPort;
    FORMAT_S *pFormat = &pChnAttr->chnFormat;
    vsi_u32_t ySize = 0;
    vsi_u32_t cbSize = 0;

#ifdef ISP_ARY_CROP_V10
    vsi_reg_t arryFormatConv = 0x00;
#endif

    if (pChnAttr->transBus == TRANS_BUS_FLEXA) {
        ySize  = pFormat->planeFmt[0].size;
        cbSize = pFormat->planeFmt[1].size;
    }

    IspPort.devId = IspChn.devId;
    IspPort.portId = IspChn.portId;

    vsios_memset(pFormat->planeFmt, 0, sizeof(pFormat->planeFmt));

    switch(pFormat->pixelFormat) {
        case PIXEL_FORMAT_RAW8:
            writeFmt = MIV1_RAW8;
            outFmt   = MIV1_FMT_RAW8;
            pFormat->numPlanes = 1;
            pFormat->planeFmt[0].bytesPerLine = pFormat->width;
            pFormat->planeFmt[0].size =
                pFormat->planeFmt[0].bytesPerLine *pFormat->height;
            pFormat->imageSize =  pFormat->planeFmt[0].size;
            break;
        case PIXEL_FORMAT_RAW10:
            writeFmt = MIV1_RAW10;
            outFmt   = MIV1_FMT_RAW10;
            pFormat->numPlanes = 1;
            pFormat->planeFmt[0].bytesPerLine = pFormat->width;
            pFormat->planeFmt[0].size =
                pFormat->planeFmt[0].bytesPerLine * pFormat->height * 2;
            pFormat->imageSize =  pFormat->planeFmt[0].size;
            break;
        case PIXEL_FORMAT_RAW12:
            writeFmt = MIV1_RAW12;
            outFmt   = MIV1_FMT_RAW12;
            pFormat->numPlanes = 1;
            pFormat->planeFmt[0].bytesPerLine = pFormat->width;
            pFormat->planeFmt[0].size =
                pFormat->planeFmt[0].bytesPerLine * pFormat->height  * 2;
            pFormat->imageSize =  pFormat->planeFmt[0].size;
            break;
        case PIXEL_FORMAT_NV16:
            writeFmt = MIV1_YUV_SEMIPLANER;
            outFmt   = MIV1_FMT_YUV422;
            pFormat->numPlanes = 2;
            pFormat->planeFmt[0].bytesPerLine = pFormat->width;
            pFormat->planeFmt[0].size =
                pFormat->planeFmt[0].bytesPerLine * pFormat->height;

            pFormat->planeFmt[1].bytesPerLine = pFormat->width;
            pFormat->planeFmt[1].size =
                pFormat->planeFmt[1].bytesPerLine * pFormat->height;

            pFormat->imageSize = pFormat->planeFmt[0].size +
                pFormat->planeFmt[1].size;
            break;
        case PIXEL_FORMAT_NV12:
            writeFmt = MIV1_YUV_SEMIPLANER;
            outFmt   = MIV1_FMT_YUV420;
            pFormat->numPlanes = 2;
            pFormat->planeFmt[0].bytesPerLine = pFormat->width;
            pFormat->planeFmt[1].bytesPerLine = pFormat->width;

            if (pChnAttr->transBus == TRANS_BUS_FLEXA) {
                pFormat->planeFmt[0].size = ySize;
                pFormat->planeFmt[1].size = cbSize;
            } else {
                pFormat->planeFmt[0].size =
                    pFormat->planeFmt[0].bytesPerLine * pFormat->height;

                pFormat->planeFmt[1].size =
                    pFormat->planeFmt[1].bytesPerLine * pFormat->height / 2;
            }

            pFormat->imageSize = pFormat->planeFmt[0].size +
                pFormat->planeFmt[1].size;
            break;
        case PIXEL_FORMAT_YUV422P:
            writeFmt = MIV1_YUV_PLANER;
            outFmt   = MIV1_FMT_YUV422;
            pFormat->numPlanes = 3;
            pFormat->planeFmt[0].bytesPerLine = pFormat->width;
            pFormat->planeFmt[0].size =
                pFormat->planeFmt[0].bytesPerLine * pFormat->height;

            pFormat->planeFmt[1].bytesPerLine = pFormat->width / 2;
            pFormat->planeFmt[1].size =
                pFormat->planeFmt[1].bytesPerLine * pFormat->height;

            pFormat->planeFmt[2].bytesPerLine = pFormat->width / 2;
            pFormat->planeFmt[2].size =
                pFormat->planeFmt[2].bytesPerLine * pFormat->height;

            pFormat->imageSize = pFormat->planeFmt[0].size +
                pFormat->planeFmt[1].size + pFormat->planeFmt[2].size;
            break;
        case PIXEL_FORMAT_YUV420P:
            writeFmt = MIV1_YUV_PLANER;
            outFmt   = MIV1_FMT_YUV420;
            pFormat->numPlanes = 3;
            pFormat->planeFmt[0].bytesPerLine = pFormat->width;
            pFormat->planeFmt[0].size =
                pFormat->planeFmt[0].bytesPerLine * pFormat->height;

            pFormat->planeFmt[1].bytesPerLine = pFormat->width / 2;
            pFormat->planeFmt[1].size =
                pFormat->planeFmt[1].bytesPerLine * pFormat->height / 2;

            pFormat->planeFmt[2].bytesPerLine = pFormat->width / 2;
            pFormat->planeFmt[2].size =
                pFormat->planeFmt[2].bytesPerLine * pFormat->height / 2;

            pFormat->imageSize = pFormat->planeFmt[0].size +
                pFormat->planeFmt[1].size + pFormat->planeFmt[2].size;
            break;
        case PIXEL_FORMAT_YUYV:
            writeFmt = MIV1_YUV_INTERLEAVED;
            outFmt   = MIV1_FMT_YUV422;
            pFormat->numPlanes = 1;
            pFormat->planeFmt[0].bytesPerLine = pFormat->width;
            pFormat->planeFmt[0].size =
                pFormat->planeFmt[0].bytesPerLine * pFormat->height * 2;

            pFormat->imageSize = pFormat->planeFmt[0].size;
            break;
        case PIXEL_FORMAT_RAW420SP:
            writeFmt = MIV1_YUV_SEMIPLANER;
            outFmt   = MIV1_FMT_YUV420;
            pFormat->numPlanes = 2;
            pFormat->height /= 2;
            pFormat->planeFmt[0].bytesPerLine = pFormat->width;
            pFormat->planeFmt[0].size =
                pFormat->planeFmt[0].bytesPerLine * pFormat->height;

            pFormat->planeFmt[1].bytesPerLine = pFormat->width;
            pFormat->planeFmt[1].size =
                pFormat->planeFmt[1].bytesPerLine * pFormat->height / 2;

            pFormat->imageSize = pFormat->planeFmt[0].size +
                pFormat->planeFmt[1].size;
            break;
        case PIXEL_FORMAT_RAW422SP:
            writeFmt = MIV1_YUV_SEMIPLANER;
            outFmt   = MIV1_FMT_YUV422;
            pFormat->numPlanes = 2;
            pFormat->height /= 2;
            pFormat->planeFmt[0].bytesPerLine = pFormat->width;
            pFormat->planeFmt[0].size =
                pFormat->planeFmt[0].bytesPerLine * pFormat->height;

            pFormat->planeFmt[1].bytesPerLine = pFormat->width;
            pFormat->planeFmt[1].size =
                pFormat->planeFmt[1].bytesPerLine * pFormat->height;

            pFormat->imageSize = pFormat->planeFmt[0].size +
                pFormat->planeFmt[1].size;
            break;
        case PIXEL_FORMAT_RGB888:
            writeFmt = MIV1_YUV_INTERLEAVED;
            outFmt   = MIV1_FMT_RGB888;
            pFormat->numPlanes = 1;
            pFormat->planeFmt[0].bytesPerLine = pFormat->width;
            pFormat->planeFmt[0].size =
                pFormat->planeFmt[0].bytesPerLine * pFormat->height * 4;

            pFormat->imageSize = pFormat->planeFmt[0].size;
            break;
        case PIXEL_FORMAT_RGB888P:
            writeFmt = MIV1_YUV_PLANER;
            outFmt   = MIV1_FMT_YUV444;
            pFormat->numPlanes = 3;
            pFormat->planeFmt[0].bytesPerLine = pFormat->width;
            pFormat->planeFmt[0].size =
                pFormat->planeFmt[0].bytesPerLine * pFormat->height;

            pFormat->planeFmt[1].bytesPerLine = pFormat->width;
            pFormat->planeFmt[1].size =
                pFormat->planeFmt[1].bytesPerLine * pFormat->height;

            pFormat->planeFmt[2].bytesPerLine = pFormat->width;
            pFormat->planeFmt[2].size =
                pFormat->planeFmt[2].bytesPerLine * pFormat->height;

            pFormat->imageSize = pFormat->planeFmt[0].size +
                pFormat->planeFmt[1].size + pFormat->planeFmt[2].size;
            break;
         case PIXEL_FORMAT_YUV400:
#ifdef ISPPICO_V2402
        //Due to the lack of frame end judgment for yuv400 in hw, raw is used to supplement it.
            writeFmt = MIV1_RAW8;
#else
            writeFmt = MIV1_YUV_YONLY;
#endif
            outFmt   = MIV1_FMT_YUV400;
            pFormat->numPlanes = 1;
            pFormat->planeFmt[0].bytesPerLine = pFormat->width;
            pFormat->planeFmt[0].size =
                pFormat->planeFmt[0].bytesPerLine * pFormat->height;

            pFormat->imageSize = pFormat->planeFmt[0].size;
            break;
        default:
            VSI_ALOGE("Invalid pixel format");
            ret = VSI_ERR_ILLEGAL_PARAM;
            return ret;
    }
#ifdef ISP_ARY_CROP_V10
    /* 0: raw 420
     * 1: raw 422
     * 2: yuv420
     * 3: yuv422
     */
    if (pFormat->pixelFormat == PIXEL_FORMAT_NV12) {
        arryFormatConv = 0x02;
    } else if (pFormat->pixelFormat == PIXEL_FORMAT_NV16) {
        arryFormatConv = 0x03;
    } else if (pFormat->pixelFormat == PIXEL_FORMAT_RAW420SP) {
        arryFormatConv = 0x0;
    } else if (pFormat->pixelFormat == PIXEL_FORMAT_RAW422SP) {
        arryFormatConv = 0x01;
    }
    VSI_ISP_CoreWriteReg(IspPort, ISP_ARRAY_FORMAT_CONV_CTRL, arryFormatConv);
#endif

    VSI_ISP_CoreReadReg(IspPort, MI_CTRL, &miCtrl);
    VSI_ISP_CoreReadReg(IspPort, MI_INIT, &miInit);

    REG_SET_SLICE(miCtrl, MRV_BURST_LEN_LUM, 2);
    REG_SET_SLICE(miCtrl, MRV_BURST_LEN_CHROM, 0);
    REG_SET_SLICE(miCtrl, MRV_INIT_BASE_ENABLE, 1);
    REG_SET_SLICE(miCtrl, MRV_INIT_OFFSET_ENABLE, 1);

    REG_SET_SLICE(miInit, MRV_MI_CFG_UPD, 0);
    REG_SET_SLICE(miInit, MRV_MI_SKIP, 0);

    switch (IspChn.chnId) {
        case CHN_ID_MP:
            REG_SET_SLICE(miCtrl, MRV_MP_PINGPONG_ENABLE, 0);
            REG_SET_SLICE(miCtrl, MRV_MP_AUTO_UPDATE, 1);
            REG_SET_SLICE(miCtrl, MRV_MP_WRITE_FORMAT, writeFmt);
            REG_SET_SLICE(miInit, MRV_MP_OUTPUT_FORMAT, outFmt);

            VSI_ISP_CoreWriteReg(IspPort, MI_CTRL, miCtrl);
            VSI_ISP_CoreWriteReg(IspPort, MI_INIT, miInit);

            VSI_ISP_CoreWriteReg(IspPort, MI_MP_Y_SIZE_INIT, pFormat->planeFmt[0].size);
            VSI_ISP_CoreWriteReg(IspPort, MI_MP_Y_OFFS_CNT_INIT, 0);
            VSI_ISP_CoreWriteReg(IspPort, MI_MP_CB_SIZE_INIT, pFormat->planeFmt[1].size);
            VSI_ISP_CoreWriteReg(IspPort, MI_MP_CB_OFFS_CNT_INIT, 0);
            VSI_ISP_CoreWriteReg(IspPort, MI_MP_CR_SIZE_INIT, pFormat->planeFmt[2].size);
            VSI_ISP_CoreWriteReg(IspPort, MI_MP_CR_OFFS_CNT_INIT, 0);

            VSI_ISP_CoreWriteReg(IspPort, MI_MP_Y_LLENGTH, pFormat->planeFmt[0].bytesPerLine);
            VSI_ISP_CoreWriteReg(IspPort, MI_MP_Y_PIC_WIDTH, pFormat->width);
            VSI_ISP_CoreWriteReg(IspPort, MI_MP_Y_PIC_HEIGHT, pFormat->height);
            VSI_ISP_CoreWriteReg(IspPort, MI_MP_Y_PIC_SIZE, pFormat->planeFmt[0].size);
            REG_SET_SLICE(alignFmt, MRV_MP_BYTE_SWAP, 0);

#ifdef ISP_MP_SCALE
            VSI_ISP_CoreReadReg(IspPort, MRSZ_FORMAT_CONV_CTRL, &rszFmCtrl);
            switch(pFormat->pixelFormat) {
                case PIXEL_FORMAT_RGB888:
                case PIXEL_FORMAT_RGB888P:
                    REG_SET_SLICE(rszFmCtrl, MRV_RSZ_OUTPUT_FORMAT, MIV1_FMT_RGB888);
                    REG_SET_SLICE(rszFmCtrl, MRV_RSZ_INPUT_FORMAT, MIV1_FMT_YUV422);
                    VSI_ISP_CoreWriteReg(IspPort, MI_MP_Y_PIC_SIZE, pFormat->width * pFormat->height);
                    break;
                default:
                    break;
            }
            VSI_ISP_CoreWriteReg(IspPort, MRSZ_FORMAT_CONV_CTRL, rszFmCtrl);
#endif
            break;
#ifdef ISP_SP1
        case CHN_ID_SP1:
            REG_SET_SLICE(miCtrl, MRV_SP_PINGPONG_ENABLE, 0);
            REG_SET_SLICE(miCtrl, MRV_SP_AUTO_UPDATE, 1);
            REG_SET_SLICE(miCtrl, MRV_SP_WRITE_FORMAT, writeFmt);
            REG_SET_SLICE(miCtrl, MRV_SP_OUTPUT_FORMAT, outFmt);

            VSI_ISP_CoreWriteReg(IspPort, MI_CTRL, miCtrl);
            VSI_ISP_CoreWriteReg(IspPort, MI_INIT, miInit);

            VSI_ISP_CoreWriteReg(IspPort, MI_SP_Y_SIZE_INIT, pFormat->planeFmt[0].size);
            VSI_ISP_CoreWriteReg(IspPort, MI_SP_Y_OFFS_CNT_INIT, 0);
            VSI_ISP_CoreWriteReg(IspPort, MI_SP_CB_SIZE_INIT, pFormat->planeFmt[1].size);
            VSI_ISP_CoreWriteReg(IspPort, MI_SP_CB_OFFS_CNT_INIT, 0);
            VSI_ISP_CoreWriteReg(IspPort, MI_SP_CR_SIZE_INIT, pFormat->planeFmt[2].size);
            VSI_ISP_CoreWriteReg(IspPort, MI_SP_CR_OFFS_CNT_INIT, 0);

            VSI_ISP_CoreWriteReg(IspPort, MI_SP_Y_LLENGTH, pFormat->planeFmt[0].bytesPerLine);
            VSI_ISP_CoreWriteReg(IspPort, MI_SP_Y_PIC_WIDTH, pFormat->width);
            VSI_ISP_CoreWriteReg(IspPort, MI_SP_Y_PIC_HEIGHT, pFormat->height);
            VSI_ISP_CoreWriteReg(IspPort, MI_SP_Y_PIC_SIZE, pFormat->planeFmt[0].size);
            REG_SET_SLICE(alignFmt, MRV_SP_BYTE_SWAP, 0);
#ifdef ISP_SP1_SCALE
            VSI_ISP_CoreReadReg(IspPort, SRSZ_FORMAT_CONV_CTRL, &rszFmCtrl);
            switch(pFormat->pixelFormat) {
                case PIXEL_FORMAT_RGB888:
                case PIXEL_FORMAT_RGB888P:
                    REG_SET_SLICE(rszFmCtrl, MRV_RSZ_OUTPUT_FORMAT, MIV1_FMT_RGB888);
                    REG_SET_SLICE(rszFmCtrl, MRV_RSZ_INPUT_FORMAT, MIV1_FMT_YUV422);
                    VSI_ISP_CoreWriteReg(IspPort, MI_SP_Y_PIC_SIZE, pFormat->width * pFormat->height);
                    break;
                default:
                    break;
            }
            VSI_ISP_CoreWriteReg(IspPort, SRSZ_FORMAT_CONV_CTRL, rszFmCtrl);
#endif
            break;
#endif
        default:
            VSI_ALOGE("Invalid chn id");
            ret = VSI_ERR_ILLEGAL_PARAM;
            return ret;
    }
    VSI_ISP_CoreReadReg(IspPort, ISP_CTRL, &ispCtrl);
    if (VSI_ISP_MiV10FormatIsRaw(pFormat->pixelFormat)) {
        REG_SET_SLICE(ispCtrl, MRV_ISP_MODE, 0);
        REG_SET_SLICE(alignFmt, MRV_MP_LSB_ALIGN, 1);
    } else {
        REG_SET_SLICE(ispCtrl, MRV_ISP_MODE, ISP_MODE_RAW);  //bayer raw
        REG_SET_SLICE(alignFmt, MRV_MP_LSB_ALIGN, 0);
    }
    VSI_ISP_CoreWriteReg(IspPort, ISP_CTRL, ispCtrl);
    VSI_ISP_CoreWriteReg(IspPort, MI_OUTPUT_ALIGN_FORMAT, alignFmt);

    return VSI_SUCCESS;
}

int VSI_ISP_SetMiV10ChnAttr(ISP_CHN IspChn, ISP_CHN_ATTR_S *pChnAttr)
{
    int ret;

#ifdef ISP_DATA_PATH_CTL
    ISP_CORE_PORT_S *ispPort;
    vsi_reg_t viDpcl = 0x00;
    ISP_PORT IspPort;

    IspPort.devId = IspChn.devId;
    IspPort.portId = IspChn.portId;

    VSI_ISP_CoreReadReg(IspPort, VI_DPCL, &viDpcl);

    ispPort = VSI_ISP_CoreGetPort(IspPort);
    if (ispPort->hdrMode == HDR_MODE_ISP_STICH) {
        REG_SET_SLICE(viDpcl, MRV_IF_SELECT0, 3);
    } else {
        REG_SET_SLICE(viDpcl, MRV_IF_SELECT0, 0);
    }
    REG_SET_SLICE(viDpcl, MRV_MP_MUX, 1);
    if (IspChn.chnId == CHN_ID_MP) {
        REG_SET_SLICE(viDpcl, MRV_VI_CHAN_MODE, 1);
    } else if(IspChn.chnId == CHN_ID_SP1) {
        REG_SET_SLICE(viDpcl, MRV_VI_CHAN_MODE, 2);
    }
    VSI_ISP_CoreWriteReg(IspPort, VI_DPCL, viDpcl);
#endif

    ret = VSI_ISP_SetMiV10ChnFmt(IspChn, pChnAttr);
    if (ret) {
        VSI_ALOGE("Failed to VSI_ISP_SetMiV10ChnFmt");
        return ret;
    }

    return VSI_SUCCESS;
}

int VSI_ISP_SetMiV10ChnBuf(ISP_CHN IspChn, VIDEO_BUF_S *pBuf)
{
    ISP_PORT IspPort;
    IspPort.devId = IspChn.devId;
    IspPort.portId = IspChn.portId;

    switch (IspChn.chnId){
        case CHN_ID_MP:
            VSI_ISP_CoreWriteReg(IspPort, MI_MP_Y_BASE_AD_INIT, pBuf->planes[0].dmaPhyAddr);
            VSI_ISP_CoreWriteReg(IspPort, MI_MP_CB_BASE_AD_INIT, pBuf->planes[1].dmaPhyAddr);
            VSI_ISP_CoreWriteReg(IspPort, MI_MP_CR_BASE_AD_INIT, pBuf->planes[2].dmaPhyAddr);
            break;
#ifdef ISP_SP1
        case CHN_ID_SP1:
            VSI_ISP_CoreWriteReg(IspPort, MI_SP_Y_BASE_AD_INIT, pBuf->planes[0].dmaPhyAddr);
            VSI_ISP_CoreWriteReg(IspPort, MI_SP_CB_BASE_AD_INIT, pBuf->planes[1].dmaPhyAddr);
            VSI_ISP_CoreWriteReg(IspPort, MI_SP_CR_BASE_AD_INIT, pBuf->planes[2].dmaPhyAddr);
            break;
#endif
        default:
            break;
    }

    return VSI_SUCCESS;
}

int VSI_ISP_SetMiV10ChnStream(ISP_CHN IspChn, vsi_u8_t stream)
{
    vsi_reg_t miCtrl = 0x00;
    vsi_reg_t miInit = 0x00;
    vsi_reg_t miImsc = 0x00;
    ISP_CORE_CHN_S *ispChn;
    ISP_PORT IspPort;
    VIDEO_BUF_S *pBuf;
    int ret;

    IspPort.devId = IspChn.devId;
    IspPort.portId = IspChn.portId;

    ispChn = VSI_ISP_CoreGetChn(IspChn);

    ret = VSI_ISP_CoreReadReg(IspPort, MI_CTRL, &miCtrl);

    VSI_ISP_CoreReadReg(IspPort, MI_IMSC, &miImsc);

    switch (IspChn.chnId) {
        case CHN_ID_MP:
            if (stream) {
                if (VSI_ISP_MiV10FormatIsRaw(ispChn->chnFormat.pixelFormat)) {
                    REG_SET_SLICE(miCtrl, MRV_MI_PATH_ENABLE, 0x8);
                } else {
        #ifdef ISPPICO_V2402
                    REG_SET_SLICE(miCtrl, MRV_MI_PATH_ENABLE, 0x8);
        #else
                    REG_SET_SLICE(miCtrl, MRV_MI_PATH_ENABLE, 0x1);
        #endif
                }

                REG_SET_SLICE(miImsc, MRV_MP_FRAME_END, 1);
                REG_SET_SLICE(miImsc, MRV_FILL_MP_Y, 1);
                REG_SET_SLICE(miImsc, MRV_WRAP_MP_Y, 1);
                REG_SET_SLICE(miImsc, MRV_WRAP_MP_CB, 1);
                REG_SET_SLICE(miImsc, MRV_WRAP_MP_CR, 1);
                if (ispChn->transBus == TRANS_BUS_FLEXA) {
                    REG_SET_SLICE(miImsc, MRV_Y_TIMEOUT_INT, 1);
                    REG_SET_SLICE(miImsc, MRV_Y_OUTSYNC_INT, 1);
                    REG_SET_SLICE(miImsc, MRV_CB_TIMEOUT_INT, 1);
                    REG_SET_SLICE(miImsc, MRV_CB_OUTSYNC_INT, 1);
                    REG_SET_SLICE(miImsc, MRV_CR_TIMEOUT_INT, 1);
                    REG_SET_SLICE(miImsc, MRV_CR_OUTSYNC_INT, 1);
                }
            } else {
                if (VSI_ISP_MiV10FormatIsRaw(ispChn->chnFormat.pixelFormat)) {
                    REG_SET_SLICE(miCtrl, MRV_MI_PATH_ENABLE, 0x0);
                } else {
                    miCtrl &= ~(0x01);
                }

                REG_SET_SLICE(miImsc, MRV_MP_FRAME_END, 0);
                REG_SET_SLICE(miImsc, MRV_FILL_MP_Y, 0);
                REG_SET_SLICE(miImsc, MRV_WRAP_MP_Y, 0);
                REG_SET_SLICE(miImsc, MRV_WRAP_MP_CB, 0);
                REG_SET_SLICE(miImsc, MRV_WRAP_MP_CR, 0);
                if (ispChn->transBus == TRANS_BUS_FLEXA) {
                    REG_SET_SLICE(miImsc, MRV_Y_TIMEOUT_INT, 0);
                    REG_SET_SLICE(miImsc, MRV_Y_OUTSYNC_INT, 0);
                    REG_SET_SLICE(miImsc, MRV_CB_TIMEOUT_INT, 0);
                    REG_SET_SLICE(miImsc, MRV_CB_OUTSYNC_INT, 0);
                    REG_SET_SLICE(miImsc, MRV_CR_TIMEOUT_INT, 0);
                    REG_SET_SLICE(miImsc, MRV_CR_OUTSYNC_INT, 0);
                }
            }
            break;
#ifdef ISP_SP1
        case CHN_ID_SP1:
            if (stream) {
                miCtrl |= 0x02;
                REG_SET_SLICE(miImsc, MRV_SP_FRAME_END, 1);
                REG_SET_SLICE(miImsc, MRV_WRAP_SP_Y, 1);
                REG_SET_SLICE(miImsc, MRV_WRAP_SP_CB, 1);
                REG_SET_SLICE(miImsc, MRV_WRAP_SP_CR, 1);
            } else {
                miCtrl &= ~(0x02);
                REG_SET_SLICE(miImsc, MRV_SP_FRAME_END, 0);
                REG_SET_SLICE(miImsc, MRV_WRAP_SP_Y, 0);
                REG_SET_SLICE(miImsc, MRV_WRAP_SP_CB, 0);
                REG_SET_SLICE(miImsc, MRV_WRAP_SP_CR, 0);
            }
            break;
#endif
        default:
            break;
    }

    VSI_ISP_CoreWriteReg(IspPort, MI_CTRL, miCtrl);
    VSI_ISP_CoreWriteReg(IspPort, MI_IMSC, miImsc);

    if (stream) {
        ret = VSI_VB_GetEmptyBuf(&ispChn->vbQueue, &pBuf);
        if (ret) {
            VSI_ALOGE("Failed to VSI_VB_GetEmptyBuf");
            return ret;
        }
        VSI_ISP_SetMiV10ChnBuf(IspChn, pBuf);
        ispChn->pBuf = pBuf;

        VSI_ISP_CoreReadReg(IspPort, MI_INIT, &miInit);
        REG_SET_SLICE(miInit, MRV_MI_CFG_UPD, 1);
        VSI_ISP_CoreWriteReg(IspPort, MI_INIT, miInit);

        if ((ispChn->transBus == TRANS_BUS_FLEXA) || (VB_MAX_FRAME == 1)) {
            ispChn->pShdBuf = ispChn->pBuf;
            return VSI_SUCCESS;
        }

        ret = VSI_VB_GetEmptyBuf(&ispChn->vbQueue, &pBuf);
        if (ret) {
            VSI_ALOGE("Failed to VSI_VB_GetEmptyBuf");
            return ret;
        }

        VSI_ISP_SetMiV10ChnBuf(IspChn, pBuf);
        ispChn->pShdBuf = ispChn->pBuf;
        ispChn->pBuf = pBuf;

    } else {
        //vsios_mutex_lock(&ispChn->mLock);
        ispChn->pShdBuf = NULL;
        ispChn->pBuf = NULL;
        //vsios_mutex_unlock(&ispChn->mLock);
    }

    return VSI_SUCCESS;
}

int VSI_ISP_MiV10MiMisProcess_Alif(ISP_DEV IspDev, vsi_u32_t miMis)
{
    ISP_CHN IspChn;
    ISP_CORE_CHN_S *ispChn;
    VIDEO_BUF_S *pBuf;
    int ret;

    if (miMis & MRV_MP_FRAME_END_MASK) {
    	IspChn.devId = IspDev;
    	IspChn.portId = 0;
    	IspChn.chnId = CHN_ID_MP;

    	ispChn = VSI_ISP_CoreGetChn(IspChn);

    	if (ispChn->pShdBuf) {
    		VSI_VB_BufDone(&ispChn->vbQueue, ispChn->pShdBuf);
    		ispChn->pShdBuf = NULL;
    		if (VB_MAX_FRAME == 1) {
    			ispChn->pBuf = NULL;
    			return VSI_SUCCESS;
    		}
    	}

    	ret = VSI_VB_GetEmptyBuf(&ispChn->vbQueue, &pBuf);
		if (ret == VSI_SUCCESS) {
			VSI_ISP_SetMiV10ChnBuf(IspChn, pBuf);
			ispChn->pShdBuf = ispChn->pBuf;
			ispChn->pBuf = pBuf;
		}
    }

    return VSI_SUCCESS;
}

int VSI_ISP_MiV10MiMisProcess(ISP_DEV IspDev, vsi_u32_t miMis)
{
    ISP_CHN IspChn;
    ISP_CORE_CHN_S *ispChn;
    VIDEO_BUF_S *pBuf;
    int ret;

    if (miMis & MRV_MP_FRAME_END_MASK) {
        IspChn.devId = IspDev;
        IspChn.portId = 0;
        IspChn.chnId = CHN_ID_MP;

        ispChn = VSI_ISP_CoreGetChn(IspChn);

        //vsios_mutex_lock(&ispChn->mLock);

        if (ispChn->pShdBuf) {
            VSI_VB_BufDone(&ispChn->vbQueue, ispChn->pShdBuf);
            ispChn->pShdBuf = NULL;
            if (VB_MAX_FRAME == 1) {
            	ispChn->pBuf = NULL;
            	return VSI_SUCCESS;
            }
        }

        ret = VSI_VB_GetEmptyBuf(&ispChn->vbQueue, &pBuf);
        if (ret == VSI_SUCCESS) {
            VSI_ISP_SetMiV10ChnBuf(IspChn, pBuf);
            ispChn->pShdBuf = ispChn->pBuf;
            ispChn->pBuf = pBuf;
        }

        //vsios_mutex_unlock(&ispChn->mLock);

    }

#ifdef ISP_SP1
    if (miMis & MRV_SP_FRAME_END_MASK) {
        IspChn.devId = IspDev;
        IspChn.portId = 0;
        IspChn.chnId = CHN_ID_SP1;

        ispChn = VSI_ISP_CoreGetChn(IspChn);

        vsios_mutex_lock(&ispChn->mLock);

        if (ispChn->pShdBuf) {
            VSI_VB_BufDone(&ispChn->vbQueue, ispChn->pShdBuf);
            ispChn->pShdBuf = NULL;
        }

        ret = VSI_VB_GetEmptyBuf(&ispChn->vbQueue, &pBuf);
        if (ret == VSI_SUCCESS) {
            VSI_ISP_SetMiV10ChnBuf(IspChn, pBuf);
            ispChn->pShdBuf = ispChn->pBuf;
            ispChn->pBuf = pBuf;
        }

        vsios_mutex_unlock(&ispChn->mLock);
    }
#endif

    return VSI_SUCCESS;
}
