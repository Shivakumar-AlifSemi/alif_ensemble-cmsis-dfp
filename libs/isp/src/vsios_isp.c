/*
 * vsios_isp.c
 *
 *  Created on: 05-Sep-2025
 *      Author: yogender
 */

#include <vsi_isp_reg_def.h>
#include "vsios_isp.h"
#include "vsios_log.h"
#include "isp_conf.h"

#include "soc.h"
#include <stdint.h>

#define LOGTAG "VSIOS_ISP"

int vsios_isp_write_reg(vsi_u32_t dev_id, vsi_reg_t reg, vsi_u32_t val)
{
    if (dev_id >= ISP_DEV_CNT) {
    	VSI_ALOGE("Invaild isp device id %d", dev_id);
    	return VSI_ERR_INVALID_DEVID;
    }

    *(volatile uint32_t *) (reg + ISP_BASE) = val;

    return 0;
}

int vsios_isp_read_reg(vsi_u32_t dev_id, vsi_reg_t reg, vsi_u32_t *val)
{
	if (dev_id >= ISP_DEV_CNT) {
	    VSI_ALOGE("Invaild isp device id %d", dev_id);
	    return VSI_ERR_INVALID_DEVID;
    }

	*val = *(volatile uint32_t *) (reg + ISP_BASE);

	return 0;
}
