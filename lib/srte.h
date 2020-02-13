/*
 * SR-TE definitions
 * Copyright 2020 NetDef Inc.
 *                Sascha Kattelmann
 *
 * This file is part of GNU Zebra.
 *
 * GNU Zebra is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2, or (at your
 * option) any later version.
 *
 * GNU Zebra is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; see the file COPYING; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifdef __cplusplus
extern "C" {
#endif

#define SRTE_POLICY_NAME_MAX_LENGTH	100

enum zebra_sr_policy_status {
    ZEBRA_SR_POLICY_UNKNOWN = 0,
    ZEBRA_SR_POLICY_UP = 1,
    ZEBRA_SR_POLICY_DOWN = 2,
};

static inline int sr_policy_compare(const struct ipaddr *a_endpoint,
				    const struct ipaddr *b_endpoint,
				    uint32_t a_color, uint32_t b_color)
{
	if (a_color < b_color)
		return -1;

	if (a_color > b_color)
		return 1;

	return ipaddr_cmp(a_endpoint, b_endpoint);
}

#ifdef __cplusplus
}
#endif
