/*
 *
 * Copyright (c) 2011-2016 The University of Waikato, Hamilton, New Zealand.
 * All rights reserved.
 *
 * This file is part of libprotoident.
 *
 * This code has been developed by the University of Waikato WAND
 * research group. For further information please see http://www.wand.net.nz/
 *
 * libprotoident is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * libprotoident is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 */

#include <string.h>

#include "libprotoident.h"
#include "proto_manager.h"
#include "proto_common.h"

static inline bool match_asd_reply(uint32_t payload) {
        
        if (MATCH(payload, 'r', '6', 'f', 'n'))
                return true;
        return false;
}

static inline bool match_asd_request(uint32_t payload) {
        
        if (MATCH(payload, '7', 'P', 'Y', 'q'))
                return true;
        return false;
}

static inline bool match_avast_secure_dns(lpi_data_t *data, 
                lpi_module_t *mod UNUSED) {

        if (data->server_port != 53 && data->client_port != 53 && 
                        data->server_port != 443 && data->client_port != 443) {
                return false;
        }

        if (match_asd_request(data->payload[0])) {
                if (match_asd_reply(data->payload[1]))
                        return true;
        }
        
        if (match_asd_request(data->payload[1])) {
                if (match_asd_reply(data->payload[0]))
                        return true;
        }

        if (data->server_port == 443 || data->client_port == 443) {
                if (match_dns(data)) {
                        return true;
                }
        }

	return false;
}

static lpi_module_t lpi_avast_secure_dns = {
	LPI_PROTO_UDP_AVAST_DNS,
	LPI_CATEGORY_SERVICES,
	"AvastSecureDNS",
	14,
	match_avast_secure_dns
};

void register_avast_secure_dns(LPIModuleMap *mod_map) {
	register_protocol(&lpi_avast_secure_dns, mod_map);
}

