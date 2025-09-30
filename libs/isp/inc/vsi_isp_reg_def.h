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

#ifndef __VSI_ISP_REG_DEF_H__
#define __VSI_ISP_REG_DEF_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif


#define VI_CCL                           0x00000000    /*!< Clock control register (rw) */
#define VI_ID_CUSTOMER_ID                0x00000004    /*!< Customer identification register (ro) */
#define VI_ID                            0x00000008    /*!< Revision identification register (r) */
#define VI_ID_CHIP_ID                    0x0000000C    /*!< Chip identification register (ro) */
#define VI_ICCL                          0x00000010    /*!< Internal clock  control register (rw) */
#define VI_IRCL                          0x00000014    /*!< Internal reset control register (rw) */
#define VI_DPCL                          0x00000018

#define ISP_CTRL                         0x00000400    /*!< global control register (rw) */
#define ISP_ACQ_PROP                     0x00000404    /*!< ISP acquisition properties (rw) */
#define ISP_ACQ_H_OFFS                   0x00000408    /*!< horizontal input offset (rw) */
#define ISP_ACQ_V_OFFS                   0x0000040C    /*!< vertical input offset (rw) */
#define ISP_ACQ_H_SIZE                   0x00000410    /*!< horizontal input size (rw) */
#define ISP_ACQ_V_SIZE                   0x00000414    /*!< vertical input size (rw) */

#define ISP_TPG_CTRL                     0x00000500    /*!< Test pattern generator module (rw) */
#define ISP_TPG_TOTAL_IN                 0x00000504    /*!< Total clock (rw) */
#define ISP_TPG_ACT_IN                   0x00000508    /*!< Available clock (rw) */
#define ISP_TPG_FP_IN                    0x0000050C    /*!< First valid (rw) */
#define ISP_TPG_BP_IN                    0x00000510    /*!< The distabce between positive edge of vs with positive edge of hde (rw) */
#define ISP_TPG_W_IN                     0x00000514    /*!< Available clk (rw) */
#define ISP_TPG_GAP_IN                   0x00000518    /*!< The gap of sub_picture (rw) */
#define ISP_TPG_GAP_STD_IN               0x0000051C    /*!< The gad stdio of sub_picture (rw) */
#define ISP_TPG_RANDOM_SEED              0x00000520    /*!< Random seed (rw) */
#define ISP_TPG_FRAME_NUM                0x00000524    /*!< Test pattern generator frame number (rw) */

#define ISP_FRAME_RATE                   0x00000600    /*!< Frame rate control register (rw) */

#define ISP_OUT_H_OFFS                   0x00000604    /*!< Horizontal offset of output window (rw) */
#define ISP_OUT_V_OFFS                   0x00000608    /*!< Vertical offset of output window (rw) */
#define ISP_OUT_H_SIZE                   0x0000060C    /*!< Output horizontal picture size (rw) */
#define ISP_OUT_V_SIZE                   0x00000610    /*!< Output vertical picture size (rw) */
#define ISP_OUT_H_OFFS_SHD               0x00000614    /*!< current horizontal offset of output window (shadow register) (ro) */
#define ISP_OUT_V_OFFS_SHD               0x00000618    /*!< current vertical offset of output window (shadow register) (ro) */
#define ISP_OUT_H_SIZE_SHD               0x0000061C    /*!< current output horizontal picture size (shadow register) (ro) */
#define ISP_OUT_V_SIZE_SHD               0x00000620    /*!< current output vertical picture size (shadow register) (ro) */

#define BINNING_SIZE                     0x00000624    /*!< Binning step size (rw) */
#define BINNING_NUM                      0x00000628    /*!< Binning counter num (rw) */
#define BINNING_SIZE_SHD                 0x0000062C    /*!< Binning step size (shadow register) (ro) */
#define BINNING_NUM_SHD                  0x00000630    /*!< Binning counter num (shadow register) (ro) */

#define ISP_BLS_CTRL                     0x00000700    /*!< BLS global control register (rw) */
#define ISP_BLS_A_FIXED                  0x00000704    /*!< fixed black level A (rw) */
#define ISP_BLS_B_FIXED                  0x00000708    /*!< fixed black level B (rw) */
#define ISP_BLS_C_FIXED                  0x0000070C    /*!< fixed black level C (rw) */
#define ISP_BLS_D_FIXED                  0x00000710    /*!< fixed black level D (rw) */

#define ISP_EXP_CONF                     0x00000720    /*!< Exposure control (rw) */
#define ISP_EXP_H_OFFSET                 0x00000724    /*!< Horizontal offset for first block (rw) */
#define ISP_EXP_V_OFFSET                 0x00000728    /*!< Vertical offset for first block (rw) */
#define ISP_EXP_H_SIZE                   0x0000072C    /*!< Horizontal size of one block (rw) */
#define ISP_EXP_V_SIZE                   0x00000730    /*!< Vertical size of one block (rw) */
#define ISP_EXP_MEAN_00                  0x00000734    /*!< Mean luminance value of block 00 (ro) */
#define ISP_EXP_MEAN_10                  0x00000738    /*!< Mean luminance value of block 10 (ro) */
#define ISP_EXP_MEAN_20                  0x0000073C    /*!< Mean luminance value of block 20 (ro) */
#define ISP_EXP_MEAN_30                  0x00000740    /*!< Mean luminance value of block 30 (ro) */
#define ISP_EXP_MEAN_40                  0x00000744    /*!< Mean luminance value of block 40 (ro) */
#define ISP_EXP_MEAN_01                  0x00000748    /*!< Mean luminance value of block 01 (ro) */
#define ISP_EXP_MEAN_11                  0x0000074C    /*!< Mean luminance value of block 11 (ro) */
#define ISP_EXP_MEAN_21                  0x00000750    /*!< Mean luminance value of block 21 (ro) */
#define ISP_EXP_MEAN_31                  0x00000754    /*!< Mean luminance value of block 31 (ro) */
#define ISP_EXP_MEAN_41                  0x00000758    /*!< Mean luminance value of block 41 (ro) */
#define ISP_EXP_MEAN_02                  0x0000075C    /*!< Mean luminance value of block 02 (ro) */
#define ISP_EXP_MEAN_12                  0x00000760    /*!< Mean luminance value of block 12 (ro) */
#define ISP_EXP_MEAN_22                  0x00000764    /*!< Mean luminance value of block 22 (ro) */
#define ISP_EXP_MEAN_32                  0x00000768    /*!< Mean luminance value of block 32 (ro) */
#define ISP_EXP_MEAN_42                  0x0000076C    /*!< Mean luminance value of block 42 (ro) */
#define ISP_EXP_MEAN_03                  0x00000770    /*!< Mean luminance value of block 03 (ro) */
#define ISP_EXP_MEAN_13                  0x00000774    /*!< Mean luminance value of block 13 (ro) */
#define ISP_EXP_MEAN_23                  0x00000778    /*!< Mean luminance value of block 23 (ro) */
#define ISP_EXP_MEAN_33                  0x0000077C    /*!< Mean luminance value of block 33 (ro) */
#define ISP_EXP_MEAN_43                  0x00000780    /*!< Mean luminance value of block 43 (ro) */
#define ISP_EXP_MEAN_04                  0x00000784    /*!< Mean luminance value of block 04 (ro) */
#define ISP_EXP_MEAN_14                  0x00000788    /*!< Mean luminance value of block 14 (ro) */
#define ISP_EXP_MEAN_24                  0x0000078C    /*!< Mean luminance value of block 24 (ro) */
#define ISP_EXP_MEAN_34                  0x00000790    /*!< Mean luminance value of block 34 (ro) */
#define ISP_EXP_MEAN_44                  0x00000794    /*!< Mean luminance value of block 44 (ro) */

#define ISP_DGAIN_RB                     0x00000800    /*!< Digital gain of red blue (rw) */
#define ISP_DGAIN_G                      0x00000804    /*!< Digital gain of green (rw) */
#define ISP_DGAIN_RB_SHD                 0x00000808    /*!< Digital gain of red (shadow register) blue (ro) */
#define ISP_DGAIN_G_SHD                  0x0000080C    /*!< Digital gain of green (shadow register) (ro) */

#define ISP_DEMOSAIC                     0x00000810    /*!< Demosaic parameters (rw) */
#define ISP_FILT_MODE                    0x00000814    /*!< mode control register for the filter block (rw) */
#define ISP_FILT_THRES_BL0               0x00000818    /*!< Blurring threshold 0 (rw) */
#define ISP_FILT_THRES_BL1               0x0000081C    /*!< Blurring threshold 1 (rw) */
#define ISP_FILT_THRES_SH0               0x00000820    /*!< Sharpening threshold 0 (rw) */
#define ISP_FILT_THRES_SH1               0x00000824    /*!< Sharpening threshold 1 (rw) */
#define ISP_FILT_LUM_WEIGHT              0x00000828    /*!< Parameters for luminance weight function (rw) */
#define ISP_FILT_FAC_SH1                 0x0000082C    /*!< filter factor sharp1 (rw) */
#define ISP_FILT_FAC_SH0                 0x00000830    /*!< filter factor sharp0 (rw) */
#define ISP_FILT_FAC_MID                 0x00000834    /*!< filter factor middle (rw) */
#define ISP_FILT_FAC_BL0                 0x00000838    /*!< Parameter for blur 0 filter (rw) */
#define ISP_FILT_FAC_BL1                 0x0000083C    /*!< Parameter for blur 1 filter (rw) */
#define ISP_CAC_CTRL                     0x00000870    /*!< Control register for chromatic aberration correction (rw) */
#define ISP_CAC_COUNT_START              0x00000874    /*!< Preload values for CAC pixel and line counter (rw) */
#define ISP_CAC_COUNT_A                  0x00000878    /*!< Linear Parameters for radial shift calculation (rw) */
#define ISP_CAC_COUNT_B                  0x0000087C    /*!< Square Parameters for radial shift calculation (rw) */
#define ISP_CAC_COUNT_C                  0x00000880    /*!< Cubical Parameters for radial shift calculation (rw) */
#define ISP_CAC_X_NORM                   0x00000884    /*!< Normalization parameters for calculation of image coordinate x_d relative to optical center (rw) */
#define ISP_CAC_Y_NORM                   0x00000888    /*!< Normalization parameters for calculation of image coordinate y_d relative to optical center (rw) */

#define ISP_GAMMA_OUT_MODE               0x00000900    /*!< gamma segmentation mode register for output gamma (rw) */
#define ISP_GAMMA_OUT_Y_0                0x00000904    /*!< gamma out curve definition y (rw) */
#define ISP_GAMMA_OUT_Y_1                0x00000908    /*!< gamma out curve definition y (rw) */
#define ISP_GAMMA_OUT_Y_2                0x0000090C    /*!< gamma out curve definition y (rw) */
#define ISP_GAMMA_OUT_Y_3                0x00000910    /*!< gamma out curve definition y (rw) */
#define ISP_GAMMA_OUT_Y_4                0x00000914    /*!< gamma out curve definition y (rw) */
#define ISP_GAMMA_OUT_Y_5                0x00000918    /*!< gamma out curve definition y (rw) */
#define ISP_GAMMA_OUT_Y_6                0x0000091C    /*!< gamma out curve definition y (rw) */
#define ISP_GAMMA_OUT_Y_7                0x00000920    /*!< gamma out curve definition y (rw) */
#define ISP_GAMMA_OUT_Y_8                0x00000924    /*!< gamma out curve definition y (rw) */
#define ISP_GAMMA_OUT_Y_9                0x00000928    /*!< gamma out curve definition y (rw) */
#define ISP_GAMMA_OUT_Y_10               0x0000092C    /*!< gamma out curve definition y (rw) */
#define ISP_GAMMA_OUT_Y_11               0x00000930    /*!< gamma out curve definition y (rw) */
#define ISP_GAMMA_OUT_Y_12               0x00000934    /*!< gamma out curve definition y (rw) */
#define ISP_GAMMA_OUT_Y_13               0x00000938    /*!< gamma out curve definition y (rw) */
#define ISP_GAMMA_OUT_Y_14               0x0000093C    /*!< gamma out curve definition y (rw) */
#define ISP_GAMMA_OUT_Y_15               0x00000940    /*!< gamma out curve definition y (rw) */
#define ISP_GAMMA_OUT_Y_16               0x00000944    /*!< gamma out curve definition y (rw) */

#define ISP_AWB_PROP                     0x00000950    /*!< Auto white balance properties (rw) */
#define ISP_AWB_H_OFFS                   0x00000954    /*!< Auto white balance horizontal offset of measure window (rw) */
#define ISP_AWB_V_OFFS                   0x00000958    /*!< Auto white balance vertical offset of measure window (rw) */
#define ISP_AWB_H_SIZE                   0x0000095C    /*!< Auto white balance horizontal window size (rw) */
#define ISP_AWB_V_SIZE                   0x00000960    /*!< Auto white balance vertical window size (rw) */
#define ISP_AWB_FRAMES                   0x00000964    /*!< Auto white balance mean value over multiple frames (rw) */
#define ISP_AWB_REF                      0x00000968    /*!< Auto white balance reference Cb/Cr values (rw) */
#define ISP_AWB_THRESH                   0x0000096C    /*!< Auto white balance threshold values (rw) */
#define ISP_AWB_WHITE_CNT                0x00000980    /*!< Auto white balance white pixel count (ro) */
#define ISP_AWB_MEAN                     0x00000984    /*!< Auto white balance measured mean value (ro) */

#define ISP_CC_COEFF_0                   0x00000A00    /*!< Color conversion coefficient 0 (rw)*/
#define ISP_CC_COEFF_1                   0x00000A04    /*!< Color conversion coefficient 1 (rw)*/
#define ISP_CC_COEFF_2                   0x00000A08    /*!< Color conversion coefficient 2 (rw)*/
#define ISP_CC_COEFF_3                   0x00000A0C    /*!< Color conversion coefficient 3 (rw)*/
#define ISP_CC_COEFF_4                   0x00000A10    /*!< Color conversion coefficient 4 (rw)*/
#define ISP_CC_COEFF_5                   0x00000A14    /*!< Color conversion coefficient 5 (rw)*/
#define ISP_CC_COEFF_6                   0x00000A18    /*!< Color conversion coefficient 6 (rw)*/
#define ISP_CC_COEFF_7                   0x00000A1C    /*!< Color conversion coefficient 7 (rw)*/
#define ISP_CC_COEFF_8                   0x00000A20    /*!< Color conversion coefficient 8 (rw)*/

#define FORMAT_CONV_CTRL                 0x00000A24    /*!< Output control register (rw) */

#define ISP_CT_COEFF_0                   0x00000A30
#define ISP_CT_COEFF_1                   0x00000A34
#define ISP_CT_COEFF_2                   0x00000A38
#define ISP_CT_COEFF_3                   0x00000A3C
#define ISP_CT_COEFF_4                   0x00000A40
#define ISP_CT_COEFF_5                   0x00000A44
#define ISP_CT_COEFF_6                   0x00000A48
#define ISP_CT_COEFF_7                   0x00000A4C
#define ISP_CT_COEFF_8                   0x00000A50

#define ISP_CT_OFFSET_R                  0x00000A54    /*!< Color conversion coefficient 0 (rw)*/
#define ISP_CT_OFFSET_G                  0x00000A58    /*!< Color conversion coefficient 0 (rw)*/
#define ISP_CT_OFFSET_B                  0x00000A5C    /*!< Color conversion coefficient 0 (rw)*/

#define ISP_IMSC                         0x00000B00    /*!< Interrupt mask (rw) */
#define ISP_RIS                          0x00000B04    /*!< Raw interrupt status (r) */
#define ISP_MIS                          0x00000B08    /*!< Masked interrupt status (r) */
#define ISP_ICR                          0x00000B0C    /*!< Interrupt clear register (w) */
#define ISP_ISR                          0x00000B10    /*!< Interrupt set register (wo) */
#define ERR                              0x00000B14    /*!< ISP error register (ro) */
#define ERR_CLR                          0x00000B18    /*!< ISP error clear register (wo) */

#define MRSZ_CTRL                        0x00000C00    /*!< main resize global control register (rw) */
#define MRSZ_SCALE_HY                    0x00000C04    /*!< horizontal luminance scale factor register (rw) */
#define MRSZ_SCALE_HCB                   0x00000C08    /*!< horizontal Cb scale factor register (rw) */
#define MRSZ_SCALE_HCR                   0x00000C0C    /*!< horizontal Cr scale factor register (rw) */
#define MRSZ_SCALE_VY                    0x00000C10    /*!< vertical luminance scale factor register (rw) */
#define MRSZ_SCALE_VC                    0x00000C14    /*!< vertical chrominance scale factor register (rw) */
#define MRSZ_PHASE_HY                    0x00000C18    /*!< horizontal luminance phase register (rw) */
#define MRSZ_PHASE_HC                    0x00000C1C    /*!< horizontal chrominance phase register (rw) */
#define MRSZ_PHASE_VY                    0x00000C20    /*!< vertical luminance phase register (rw) */
#define MRSZ_PHASE_VC                    0x00000C24    /*!< vertical chrominance phase register (rw) */
#define MRSZ_SCALE_LUT_ADDR              0x00000C28    /*!< Address pointer of up-scaling look up table (rw) */
#define MRSZ_SCALE_LUT                   0x00000C2C    /*!< Entry of up-scaling look up table (rw) */
#define MRSZ_CTRL_SHD                    0x00000C30    /*!< global control shadow register (ro) */
#define MRSZ_SCALE_HY_SHD                0x00000C34    /*!< horizontal luminance scale factor shadow register (ro) */
#define MRSZ_SCALE_HCB_SHD               0x00000C38    /*!< horizontal Cb scale factor shadow register (ro) */
#define MRSZ_SCALE_HCR_SHD               0x00000C3C    /*!< horizontal Cr scale factor shadow register (ro) */
#define MRSZ_SCALE_VY_SHD                0x00000C40    /*!< vertical luminance scale factor shadow register (ro) */
#define MRSZ_SCALE_VC_SHD                0x00000C44    /*!< vertical chrominance scale factor shadow register (ro) */
#define MRSZ_PHASE_HY_SHD                0x00000C48    /*!< horizontal luminance phase shadow register (ro) */
#define MRSZ_PHASE_HC_SHD                0x00000C4C    /*!< horizontal chrominance phase shadow register (ro) */
#define MRSZ_PHASE_VY_SHD                0x00000C50    /*!< vertical luminance phase shadow register (ro) */
#define MRSZ_PHASE_VC_SHD                0x00000C54    /*!< vertical chrominance phase shadow register (ro) */

#define MRSZ_FORMAT_CONV_CTRL            0x00000C6C    /*!< Format conversion control register (rw) */

#define MI_CTRL                          0x00000E00    /*!< Global control register (rw) */
#define MI_INIT                          0x00000E04    /*!< Control register for address init and skip function (rw) */
#define MI_MP_Y_BASE_AD_INIT             0x00000E08    /*!< Base address for main picture Y component or raw data (rw)*/
#define MI_MP_Y_SIZE_INIT                0x00000E0C    /*!< Size of main picture Y component or raw data (rw) */
#define MI_MP_Y_OFFS_CNT_INIT            0x00000E10    /*!< Offset counter init value for main picture Y or raw data (rw) */
#define MI_MP_Y_OFFS_CNT_START           0x00000E14    /*!< Offset counter start value for main picture Y or raw data (ro) */
#define MI_MP_Y_IRQ_OFFS_INIT            0x00000E18    /*!< Fill level interrupt offset value for main picture Y or raw data (rw) */
#define MI_MP_CB_BASE_AD_INIT            0x00000E1C    /*!< Base address for main picture Cb component ring buffer (rw) */
#define MI_MP_CB_SIZE_INIT               0x00000E20    /*!< Size of main picture Cb component ring buffer (rw) */
#define MI_MP_CB_OFFS_CNT_INIT           0x00000E24    /*!< Offset counter init value for main picture Cb component ring buffer (rw) */
#define MI_MP_CB_OFFS_CNT_START          0x00000E28    /*!< Offset counter start value for main picture Cb component ring buffer (ro) */
#define MI_MP_CR_BASE_AD_INIT            0x00000E2C    /*!< Base address for main picture Cr component ring buffer (rw) */
#define MI_MP_CR_SIZE_INIT               0x00000E30    /*!< Size of main picture Cr component ring buffer (rw) */
#define MI_MP_CR_OFFS_CNT_INIT           0x00000E34    /*!< Offset counter init value for main picture Cr component ring buffer (rw) */
#define MI_MP_CR_OFFS_CNT_START          0x00000E38    /*!< Offset counter start value for main picture Cr component ring buffer (ro) */
#define MI_SP_Y_BASE_AD_INIT             0x00000E3C    /*!< Base address for self picture Y component ring buffer (rw) */
#define MI_SP_Y_SIZE_INIT                0x00000E40    /*!< Size of self picture Y component ring buffer (rw) */
#define MI_SP_Y_OFFS_CNT_INIT            0x00000E44    /*!< Offset counter init value for self picture Y component ring buffer (rw) */
#define MI_SP_Y_OFFS_CNT_START           0x00000E48    /*!< Offset counter start value for self picture Y component ring buffer (r) */
#define MI_SP_Y_LLENGTH                  0x00000E4C    /*!< Line length of self picture Y component (rw) */
#define MI_SP_CB_BASE_AD_INIT            0x00000E50    /*!< Base address for self picture Cb component ring buffer (rw) */
#define MI_SP_CB_SIZE_INIT               0x00000E54    /*!< Size of self picture Cb component ring buffer (rw) */
#define MI_SP_CB_OFFS_CNT_INIT           0x00000E58    /*!< Offset counter init value for self picture Cb component ring buffer (rw) */
#define MI_SP_CB_OFFS_CNT_START          0x00000E5C    /*!< Offset counter start value for self picture Cb component ring buffer (r) */
#define MI_SP_CR_BASE_AD_INIT            0x00000E60    /*!< Base address for self picture Cr component ring buffer (rw) */
#define MI_SP_CR_SIZE_INIT               0x00000E64    /*!< Size of self picture Cr component ring buffer (rw) */
#define MI_SP_CR_OFFS_CNT_INIT           0x00000E68    /*!< Offset counter init value for self picture Cr component ring buffer (rw) */
#define MI_SP_CR_OFFS_CNT_START          0x00000E6C    /*!< Offset counter start value for self picture Cr component ring buffer (r) */
#define MI_BYTE_CNT                      0x00000E70    /*!< Counter value of RAW data bytes (ro) */
#define MI_CTRL_SHD                      0x00000E74    /*!< global control internal shadow register (ro) */
#define MI_MP_Y_BASE_AD_SHD              0x00000E78    /*!< Base address shadow register for main picture Y component or raw data ring buffer (ro) */
#define MI_MP_Y_SIZE_SHD                 0x00000E7C    /*!< Size shadow register of main picture Y component or raw data (ro) */
#define MI_MP_Y_OFFS_CNT_SHD             0x00000E80    /*!< Current offset counter of main picture Y component or raw data ring buffer (ro) */
#define MI_MP_Y_IRQ_OFFS_SHD             0x00000E84    /*!< Shadow register of fill level interrupt offset value for main picture Y component or raw data (ro) */
#define MI_MP_CB_BASE_AD_SHD             0x00000E88    /*!< Base address shadow register for main picture Cb component ring buffer (ro) */
#define MI_MP_CB_SIZE_SHD                0x00000E8C    /*!< Size shadow register of main picture Cb component ring buffer (ro) */
#define MI_MP_CB_OFFS_CNT_SHD            0x00000E90    /*!< Current offset counter of main picture Cb component ring buffer (ro) */
#define MI_MP_CR_BASE_AD_SHD             0x00000E94    /*!< Base address shadow register for main picture Cr component ring buffer (ro) */
#define MI_MP_CR_SIZE_SHD                0x00000E98    /*!< Size shadow register of main picture Cr component ring buffer (ro) */
#define MI_MP_CR_OFFS_CNT_SHD            0x00000E9C    /*!< Current offset counter of main picture Cr component ring buffer (ro) */
#define MI_SP_Y_BASE_AD_SHD              0x00000EA0    /*!< Base address shadow register for self picture Y component ring buffer (r) */
#define MI_SP_Y_SIZE_SHD                 0x00000EA4    /*!< Size shadow register of self picture Y component ring buffer (r) */
#define MI_SP_Y_OFFS_CNT_SHD             0x00000EA8    /*!< Current offset counter of self picture Y component ring buffer (r) */

#define MI_SP_CB_BASE_AD_SHD             0x00000EB0    /*!< Base address shadow register for self picture Cb component ring buffer (r) */
#define MI_SP_CB_SIZE_SHD                0x00000EB4    /*!< Size shadow register of self picture Cb component ring buffer (r) */
#define MI_SP_CB_OFFS_CNT_SHD            0x00000EB8    /*!< Current offset counter of self picture Cb component ring buffer (r) */
#define MI_SP_CR_BASE_AD_SHD             0x00000EBC    /*!< Base address shadow register for self picture Cr component ring buffer (r) */
#define MI_SP_CR_SIZE_SHD                0x00000EC0    /*!< Size shadow register of self picture Cr component ring buffer (r) */
#define MI_SP_CR_OFFS_CNT_SHD            0x00000EC4    /*!< Current offset counter of self picture Cr component ring buffer (r) */

#define MI_IMSC                          0x00000EF8    /*!< Interrupt Mask (1: interrupt active, 0: interrupt masked) (rw) */
#define MI_MIS                           0x00000F00    /*!< masked interrupt status register (ro) */
#define MI_ICR                           0x00000F04    /*!< interrupt clear register (ro) */
#define MI_ISR                           0x00000F08    /*!< Interrupt Set Register (wo) */
#define MI_STATUS                        0x00000F0C    /*!< MI Status Register (ro) */
#define MI_STATUS_CLR                    0x00000F10    /*!< MI Status Clear Register (wo) */
        
#define MI_SP_Y_PIC_WIDTH                0x00000F14    /*!< Y component image width (rw) */
#define MI_SP_Y_PIC_HEIGHT               0x00000F18    /*!< Y component image height (rw) */
#define MI_SP_Y_PIC_SIZE                 0x00000F1C    /*!< Y component image size (rw) */

#define MI_MP_Y_BASE_AD_INIT2            0x00000F30    /*!< Base address 2 (ping pong) for main picture Y component or raw data (rw) */
#define MI_MP_CB_BASE_AD_INIT2           0x00000F34    /*!< Base address 2 (pingpong) for main picture Cb component (rw) */
#define MI_MP_CR_BASE_AD_INIT2           0x00000F38    /*!< Base address 2 (pingpong) for main picture Cr component ring buffer (rw) */
#define MI_SP_Y_BASE_AD_INIT2            0x00000F3C    /*!< Base address 2 (ping pong) for main picture Y component or raw data (rw) */
#define MI_SP_CB_BASE_AD_INIT2           0x00000F40    /*!< Base address 2 (pingpong) for main picture Cb component (rw) */
#define MI_SP_CR_BASE_AD_INIT2           0x00000F44    /*!< Base address 2 (pingpong) for main picture Cr component ring buffer (rw) */

#define MI_MP_Y_LLENGTH                  0x00000F50    /*!< MI mp y llength (rw) */
#define MI_OUTPUT_ALIGN_FORMAT           0x00000F5C    /*!< MI output byte swap and LSB alignment control (rw) */
#define MI_MP_OUTPUT_FIFO_SIZE           0x00000F60    /*!< MI mp output fifo control (rw) */
#define MI_MP_Y_PIC_WIDTH                0x00000F64    /*!< MI mp y width pix (rw) */
#define MI_MP_Y_PIC_HEIGHT               0x00000F68    /*!< MI mp y height pix (rw) */
#define MI_MP_Y_PIC_SIZE                 0x00000F6C    /*!< MI mp y pix size (rw) */

#define SRSZ_CTRL                        0x00001000    /*!< global control register (rw) */
#define SRSZ_SCALE_HY                    0x00001004    /*!< horizontal luminance scale factor register (rw) */
#define SRSZ_SCALE_HCB                   0x00001008    /*!< horizontal chrominance scale factor register (rw) */
#define SRSZ_SCALE_HCR                   0x0000100C    /*!< horizontal chrominance scale factor register (rw) */
#define SRSZ_SCALE_VY                    0x00001010    /*!< vertical luminance scale factor register (rw) */
#define SRSZ_SCALE_VC                    0x00001014    /*!< vertical chrominance scale factor register (rw) */
#define SRSZ_PHASE_HY                    0x00001018    /*!< horizontal luminance phase register (rw) */
#define SRSZ_PHASE_HC                    0x0000101C    /*!< horizontal chrominance phase register (rw) */
#define SRSZ_PHASE_VY                    0x00001020    /*!< vertical luminance phase register (rw) */
#define SRSZ_PHASE_VC                    0x00001024    /*!< vertical chrominance phase register (rw) */
#define SRSZ_SCALE_LUT_ADDR              0x00001028    /*!< Address pointer of up-scaling look up table (rw) */
#define SRSZ_SCALE_LUT                   0x0000102C    /*!< Entry of up-scaling look up table (rw) */
#define SRSZ_CTRL_SHD                    0x00001030    /*!< global control shadow register (r) */
#define SRSZ_SCALE_HY_SHD                0x00001034    /*!< horizontal luminance scale factor shadow register (r) */
#define SRSZ_SCALE_HCB_SHD               0x00001038    /*!< horizontal Cb scale factor shadow register (r) */
#define SRSZ_SCALE_HCR_SHD               0x0000103C    /*!< horizontal Cr scale factor shadow register (r) */
#define SRSZ_SCALE_VY_SHD                0x00001040    /*!< vertical luminance scale factor shadow register (r) */
#define SRSZ_SCALE_VC_SHD                0x00001044    /*!< vertical chrominance scale factor shadow register (r) */
#define SRSZ_PHASE_HY_SHD                0x00001048    /*!< horizontal luminance phase shadow register (r) */
#define SRSZ_PHASE_HC_SHD                0x0000104C    /*!< horizontal chrominance phase shadow register (r) */
#define SRSZ_PHASE_VY_SHD                0x00001050    /*!< vertical luminance phase shadow register (r) */
#define SRSZ_PHASE_VC_SHD                0x00001054    /*!< vertical chrominance phase shadow register (r) */

#define SRSZ_FORMAT_CONV_CTRL            0x0000106C    /*!< Format conversion control register (rw) */


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
