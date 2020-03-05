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

#ifndef _PATH_PCEP_H_
#define _PATH_PCEP_H_

#include <stdbool.h>
#include <debug.h>
#include <pcep_utils_logging.h>
#include <pcep_pcc_api.h>
#include "mpls.h"
#include "pathd/path_pcep_memory.h"

#define PCEP_DEFAULT_PORT 4189
#define MAX_PCC 1
#define MAX_TAG_SIZE 50
#define PCEP_DEBUG_MODE_BASIC 0x01
#define PCEP_DEBUG_MODE_PATH 0x02
#define PCEP_DEBUG_MODE_PCEP 0x04
#define PCEP_DEBUG_MODE_PCEPLIB 0x08
#define PCEP_DEBUG(fmt, ...)                                                   \
	do {                                                                   \
		if (DEBUG_FLAGS_CHECK(&pcep_g->dbg, PCEP_DEBUG_MODE_BASIC))    \
			DEBUGD(&pcep_g->dbg, "pcep: " fmt, ##__VA_ARGS__);     \
	} while (0)
#define PCEP_DEBUG_PATH(fmt, ...)                                              \
	do {                                                                   \
		if (DEBUG_FLAGS_CHECK(&pcep_g->dbg, PCEP_DEBUG_MODE_PATH))     \
			DEBUGD(&pcep_g->dbg, "pcep: " fmt, ##__VA_ARGS__);     \
	} while (0)
#define PCEP_DEBUG_PCEP(fmt, ...)                                              \
	do {                                                                   \
		if (DEBUG_FLAGS_CHECK(&pcep_g->dbg, PCEP_DEBUG_MODE_PCEP))     \
			DEBUGD(&pcep_g->dbg, "pcep: " fmt, ##__VA_ARGS__);     \
	} while (0)
#define PCEP_DEBUG_PCEPLIB(priority, fmt, ...)                                 \
	do {                                                                   \
		switch (priority) {                                            \
		case LOG_DEBUG:                                                \
			if (DEBUG_FLAGS_CHECK(&pcep_g->dbg,                    \
					      PCEP_DEBUG_MODE_PCEPLIB))        \
				DEBUGD(&pcep_g->dbg, "pcep: " fmt,             \
				       ##__VA_ARGS__);                         \
			break;                                                 \
		case LOG_INFO:                                                 \
			if (DEBUG_FLAGS_CHECK(&pcep_g->dbg,                    \
					      PCEP_DEBUG_MODE_PCEPLIB))        \
				DEBUGI(&pcep_g->dbg, "pcep: " fmt,             \
				       ##__VA_ARGS__);                         \
			break;                                                 \
		case LOG_NOTICE:                                               \
			if (DEBUG_FLAGS_CHECK(&pcep_g->dbg,                    \
					      PCEP_DEBUG_MODE_PCEPLIB))        \
				DEBUGN(&pcep_g->dbg, "pcep: " fmt,             \
				       ##__VA_ARGS__);                         \
			break;                                                 \
		case LOG_WARNING:                                              \
		case LOG_ERR:                                                  \
		default:                                                       \
			zlog(priority, "pcep: " fmt, ##__VA_ARGS__);           \
			break;                                                 \
		}                                                              \
	} while (0)

struct pce_opts {
	struct in_addr addr;
	short port;
};

struct pcc_opts {
	struct in_addr addr;
	short port;
	bool force_stateless;
};

struct lsp_nb_key {
	uint32_t color;
	struct ipaddr endpoint;
	uint32_t preference;
};

struct sid_mpls {
	mpls_label_t label;
	uint8_t traffic_class;
	bool is_bottom;
	uint8_t ttl;
};

struct pcep_caps {
	bool is_stateful;
};

union sid {
	uint32_t value;
	struct sid_mpls mpls;
};

struct nai_ipv4_node {
	struct in_addr addr;
};

union nai {
	struct nai_ipv4_node ipv4_node;
};

struct path_hop {
	/* Pointer to the next hop in the path */
	struct path_hop *next;
	/* Indicateif this ia a loose or strict hop */
	bool is_loose;
	/* Indicate if there is an SID for the hop */
	bool has_sid;
	/* Indicate if the hop as a MPLS label */
	bool is_mpls;
	/* Indicate if the MPLS label has extra attributes (TTL, class..)*/
	bool has_attribs;
	/* Hop's SID if available */
	union sid sid;
	/* Indicate if there ia a NAI for this hop */
	bool has_nai;
	/* Indicate Hop's NAI type if available */
	enum pcep_sr_subobj_nai nai_type;
	/* Hop's NAI if available */
	union nai nai;
};

struct path {
	/* The address the path is comming from (only work for the PCE for now)
	 */
	struct ipaddr sender;
	/* The northbound key identifying this path */
	struct lsp_nb_key nbkey;
	/* The generated unique PLSP identifier for this path.
	   See draft-ietf-pce-stateful-pce */
	uint32_t plsp_id;
	/* The request identifier from the PCE, when getting a path from the
	   PCE. See draft-ietf-pce-stateful-pce */
	uint32_t srp_id;
	/* The request identifier from the PCC , when getting a path from the
	   PCE after a computation request. See rfc5440, section-7.4 */
	uint32_t req_id;
	/* The name of the path */
	char *name;
	/* The operational status of the path */
	enum pcep_lsp_operational_status status;
	/* If true, the receiver (PCC) must remove the path.
	   See draft-ietf-pce-pce-initiated-lsp */
	bool do_remove;
	/* Indicate if the PCE wants the path to get active.
	   See draft-ietf-pce-stateful-pce, section-7.3, flag A */
	bool go_active;
	/* Indicate the given path was created by the PCE,
	   See draft-ietf-pce-pce-initiated-lsp, section-5.3.1, flag C */
	bool was_created;
	/* Indicate the given path was removed by the PCC.
	   See draft-ietf-pce-stateful-pce, section-7.3, flag R */
	bool was_removed;
	/* Indicate the path is part of the synchronization process.
	   See draft-ietf-pce-stateful-pce, section-7.3, flag S */
	bool is_synching;
	/* Indicate the path is delegated to the PCE.
	   See draft-ietf-pce-stateful-pce, section-7.3, flag D */
	bool is_delegated;
	/* Specify the list of hop defining the path */
	struct path_hop *first;
};

struct pcep_glob {
	struct debug dbg;
	struct thread_master *master;
	struct frr_pthread *fpt;
};

extern struct pcep_glob *pcep_g;

/* Path Helper Functions */
struct path *pcep_new_path(void);
struct path_hop *pcep_new_hop(void);
void pcep_free_path(struct path *path);

#endif // _PATH_PCEP_H_