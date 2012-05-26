/*! \file wmv_bus/bus_rx_queue.h 
 *  \ingroup bus_group
 *  \brief FIFO queue for the RXed messages.
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "wmv_bus/bus_rx_queue.h" \endcode
 */
//    Copyright (C) 2008  Mikael Larsmark, SM2WMV
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef _BUS_RX_QUEUE_H_
#define _BUS_RX_QUEUE_H_

#include "bus.h"

void rx_queue_add(BUS_MESSAGE message);
BUS_MESSAGE rx_queue_get(void);
void rx_queue_drop(void);
void rx_queue_dropall(void);
void rx_queue_init(void);
unsigned char rx_queue_is_empty(void);
unsigned char rx_queue_size(void);

#endif