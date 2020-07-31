/*
 * Copyright (C) 2018 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * Author: Droiing <jianglinxuan@kylinos.cn>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <unistd.h>

#include <biometric_common.h>
#include <biometric_storage.h>
#include <biometric_version.h>

#include "community_ops.h"

static const struct usb_id uru4000_id_table[] = {
	{ .idVendor = 0x045e, .idProduct = 0x00bb, .description = "ms kbd with fp rdr" },
	{ .idVendor = 0x045e, .idProduct = 0x00bc, .description = "ms intellimouse with fp rdr" },
	{ .idVendor = 0x045e, .idProduct = 0x00bd, .description = "ms fp rdr (standalone)" },
	{ .idVendor = 0x045e, .idProduct = 0x00ca, .description = "ms fp rdr (standalone) v2" },
	{ .idVendor = 0x05ba, .idProduct = 0x0007, .description = "dp uru4000 (standalone)" },
	{ .idVendor = 0x05ba, .idProduct = 0x0008, .description = "dp uru4000 (keyboard)" },
	{ .idVendor = 0x05ba, .idProduct = 0x000a, .description = "dp uru4000b (standalone)" },
	{ 0, 0, NULL },
};

int ops_configure(bio_dev * dev, GKeyFile * conf)
{
	dev->driver_id = bio_get_empty_driver_id();
	dev->device_name = URU4000_NAME;
	dev->full_name = "Digital Persona U.are.U 4000/4000B/4500";
	dev->bioinfo.biotype = BioT_FingerPrint;
	dev->bioinfo.stotype = StoT_OS;
	dev->bioinfo.eigtype = EigT_Eigenvalue;
	dev->bioinfo.vertype = VerT_Software;
	dev->bioinfo.idtype = IdT_Software;
	dev->bioinfo.bustype = BusT_USB;

	dev->usb_info.id_table = uru4000_id_table;
	dev->usb_info.driver_data = 0;

	dev->ops_configure = ops_configure;
	dev->ops_driver_init = community_ops_driver_init;
	dev->ops_discover = community_ops_discover;
	dev->ops_open = community_ops_open;
	dev->ops_enroll = community_ops_enroll;
	dev->ops_verify = community_ops_verify;
	dev->ops_identify = community_ops_identify;
	dev->ops_capture = NULL;
	dev->ops_search = community_ops_search;
	dev->ops_clean = community_ops_clean;
	dev->ops_get_feature_list = community_ops_get_feature_list;
	dev->ops_attach = community_ops_attach;
	dev->ops_detach = community_ops_detach;
	dev->ops_stop_by_user = community_ops_stop_by_user;
	dev->ops_feature_rename = NULL;	// Use the default function provided by the framework
	dev->ops_close = community_ops_close;
	dev->ops_free = community_ops_free;

	dev->ops_get_ops_result_mesg = community_ops_get_ops_result_mesg;
	dev->ops_get_dev_status_mesg = community_ops_get_dev_status_mesg;
	dev->ops_get_notify_mid_mesg = community_ops_get_notify_mid_mesg;

	bio_set_drv_api_version(dev);
	bio_set_dev_status(dev, DEVS_COMM_IDLE);
	bio_set_ops_result(dev, OPS_COMM_SUCCESS);
	bio_set_notify_mid(dev, NOTIFY_COMM_IDLE);

	dev->enable = bio_dev_is_enable(dev, conf);

	community_fpdev *cfpdev = malloc(sizeof(community_fpdev));
	memset(cfpdev, 0, sizeof(community_fpdev));
	cfpdev->dev = NULL;
	cfpdev->community_driver_id = URU4000_ID;
	cfpdev->ops_timeout_ms = 30 * 1000;
	dev->dev_priv = cfpdev;

	community_para_config(dev, conf);

	return 0;
}
