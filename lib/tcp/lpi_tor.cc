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

static inline bool match_tor(lpi_data_t *data, lpi_module_t *mod UNUSED) {

	/* I *think* this is TOR but have not confirmed */
	
	/* Don't believe in this rule anymore :) */
	/*
	if (match_chars_either(data, 0x3d, 0x00, 0x00, 0x00) &&
			(data->payload_len[0] == 4 ||
			data->payload_len[1] == 4))
		return true;
	*/

	/* Lots of TOR is SSL over port 443, which we can't really distinguish
	 * from HTTPS. However, we can match the stuff on port 9001 */

	if (!match_ssl(data))
		return false;

	if (data->server_port == 9001 || data->client_port == 9001)
		return true;

	return false;
}

static lpi_module_t lpi_tor = {
	LPI_PROTO_TOR,
	LPI_CATEGORY_TUNNELLING,
	"TOR",
	7, 	/* Not the strongest rule */
	match_tor
};

void register_tor(LPIModuleMap *mod_map) {
	register_protocol(&lpi_tor, mod_map);
}

