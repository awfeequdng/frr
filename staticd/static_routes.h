/*
 * STATICd - static routes header
 * Copyright (C) 2018 Cumulus Networks, Inc.
 *               Donald Sharp
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
#ifndef __STATIC_ROUTES_H__
#define __STATIC_ROUTES_H__

#include "lib/mpls.h"

/* Static route label information */
struct static_nh_label {
	uint8_t num_labels;
	uint8_t reserved[3];
	mpls_label_t label[MPLS_MAX_LABELS];
};

enum static_blackhole_type {
	STATIC_BLACKHOLE_DROP = 0,
	STATIC_BLACKHOLE_NULL,
	STATIC_BLACKHOLE_REJECT
};

typedef enum {
	STATIC_IFNAME,
	STATIC_IPV4_GATEWAY,
	STATIC_IPV4_GATEWAY_IFNAME,
	STATIC_BLACKHOLE,
	STATIC_IPV6_GATEWAY,
	STATIC_IPV6_GATEWAY_IFNAME,
} static_types;

/* Static route information. */
struct static_route {
	/* For linked list. */
	struct static_route *prev;
	struct static_route *next;

	/* VRF identifier. */
	vrf_id_t vrf_id;
	vrf_id_t nh_vrf_id;
	char nh_vrfname[VRF_NAMSIZ + 1];

	/* Administrative distance. */
	uint8_t distance;

	/* Tag */
	route_tag_t tag;

	/* Flag for this static route's type. */
	static_types type;

	/*
	 * Nexthop value.
	 */
	enum static_blackhole_type bh_type;
	union g_addr addr;
	ifindex_t ifindex;
	bool nh_registered;
	bool nh_valid;

	char ifname[INTERFACE_NAMSIZ + 1];

	/* Label information */
	struct static_nh_label snh_label;

	uint32_t table_id;
};

extern bool mpls_enabled;

extern struct zebra_privs_t static_privs;

void static_fixup_vrf_ids(struct static_vrf *svrf);

extern int static_add_route(afi_t afi, safi_t safi, uint8_t type,
			    struct prefix *p, struct prefix_ipv6 *src_p,
			    union g_addr *gate, const char *ifname,
			    enum static_blackhole_type bh_type, route_tag_t tag,
			    uint8_t distance, struct static_vrf *svrf,
			    struct static_vrf *nh_svrf,
			    struct static_nh_label *snh_label,
			    uint32_t table_id);

extern int static_delete_route(afi_t afi, safi_t safi, uint8_t type,
			       struct prefix *p, struct prefix_ipv6 *src_p,
			       union g_addr *gate, const char *ifname,
			       route_tag_t tag, uint8_t distance,
			       struct static_vrf *svrf,
			       struct static_nh_label *snh_label,
			       uint32_t table_id);

extern void static_cleanup_vrf_ids(struct static_vrf *disable_svrf);

extern void static_ifindex_update(struct interface *ifp, bool up);
#endif
