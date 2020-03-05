/*
 * Copyright (C) 2018  NetDEF, Inc.
 *                     Sebastien Merle
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; see the file COPYING; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef _PATH_PCEP_LIB_H_
#define _PATH_PCEP_LIB_H_

#include <stdbool.h>
#include <pcep_pcc_api.h>
#include "pathd/path_pcep.h"

int pcep_lib_initialize(void);
void pcep_lib_finalize(void);
pcep_session *pcep_lib_connect(struct pcc_opts *pcc_opts,
			       struct pce_opts *pce_opts);
void pcep_lib_disconnect(pcep_session *sess);
struct pcep_message *pcep_lib_format_report(struct path *path);
struct pcep_message *pcep_lib_format_request(uint32_t reqid, struct ipaddr *src,
					     struct ipaddr *dst);
struct path *pcep_lib_parse_path(struct pcep_message *msg);
void pcep_lib_parse_capabilities(struct pcep_message *msg,
				 struct pcep_caps *caps);

#endif // _PATH_PCEP_LIB_H_