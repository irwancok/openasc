/*! \file comm_interface_rx_queue.c
 *  \brief The comm interface RX QUEUE
 *  \ingroup comm_interface_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-12-09
 *  \code #include "comm_interface_rx_queue.c" \endcode
 */
//    Copyright (C) 2011  Mikael Larsmark, SM2WMV
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

/* FIFO queue */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "comm_interface_rx_queue.h"

volatile static char data_changed = 0;

//! The RX queue
static comm_interface_rx_queue_struct comm_interface_rx_queue;

/*! \brief Initialize the comm interface rx queue */
void comm_interface_rx_queue_init(void) {
	comm_interface_rx_queue.first = 0;
	comm_interface_rx_queue.last = 0;
  
  comm_interface_rx_queue.count = 0;
}

/*! \brief Insert a message into the TX queue (FIFO)
 * \param message - The message that should be inserted to the queue
 * \return 0 If it went well, 1 if the queue is full
 */
unsigned char comm_interface_rx_queue_add(struct_comm_interface_msg message) {
	if (comm_interface_rx_queue.count < COMM_INTERFACE_RX_QUEUE_SIZE) {
		data_changed = 1;
		
		comm_interface_rx_queue.message[comm_interface_rx_queue.last++] = message;
		
		if (comm_interface_rx_queue.last >= COMM_INTERFACE_RX_QUEUE_SIZE)
			comm_interface_rx_queue.last = 0;
		
		if (comm_interface_rx_queue.last == comm_interface_rx_queue.first)
			comm_interface_rx_queue.first++;
		
		if (comm_interface_rx_queue.first >= COMM_INTERFACE_RX_QUEUE_SIZE)
			comm_interface_rx_queue.first = 0;	
		
		comm_interface_rx_queue.count++;
		
		return(0);
	}
	else
		return(1);
}

/*!\brief Retrieve the first message from the FIFO TX queue.
 * \return The first message in the queue
 */
struct_comm_interface_msg comm_interface_rx_queue_get(void) {
  data_changed = 0;
  
  //Return the message (content of the first node)
  struct_comm_interface_msg mess = comm_interface_rx_queue.message[comm_interface_rx_queue.first];
  
  if (data_changed) {
    disable_comm_interface_interrupt();
    mess = comm_interface_rx_queue.message[comm_interface_rx_queue.first];
    enable_comm_interface_interrupt();
  }

  return(mess);
}

/*! Drops the first message in the queue Frees up the memory space aswell.
 */
void comm_interface_rx_queue_drop(void) {
  comm_interface_rx_queue.first++;
	
	if (comm_interface_rx_queue.first >= COMM_INTERFACE_RX_QUEUE_SIZE)
		comm_interface_rx_queue.first = 0;
  
  if (comm_interface_rx_queue.count > 0)
    comm_interface_rx_queue.count--;
}

/*! \brief Erase all content in the TX queue
 * \return The number of items that were cleared
 */
void comm_interface_rx_queue_dropall(void) {
  comm_interface_rx_queue.first = 0;
	comm_interface_rx_queue.last = 0;
  
  comm_interface_rx_queue.count = 0;
}

/*! \brief Check if the queue is empty
 *	\return 1 if the queue is empty and 0 otherwise
 */
unsigned char comm_interface_rx_queue_is_empty(void) {
  unsigned char state = 0;
  
  if (comm_interface_rx_queue.first == comm_interface_rx_queue.last)
		state = 1;
  
  return(state);
}

/*! \brief Check the number of message in the queue 
 *  \return The number of messages in the queue */
unsigned char comm_interface_rx_queue_count(void) {
  unsigned char count = comm_interface_rx_queue.count;
  
  return(count);
}