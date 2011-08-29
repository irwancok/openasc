/*! \file wmv_bus/bus.c
 *  \brief The communication bus protocol used in the openASC project.
 *  \ingroup bus_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "wmv_bus/bus.c" \endcode
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

/* bus protocol format
 * ----------------------------------------------------------------------------
 * |0xFE|0xFE|fm-adr|to-adr|checksum|flags|cmd|length|data|data|data|data|0xFD|
 * ----------------------------------------------------------------------------
 *
 * The length of a message is total length - 7 bytes (premble, to-adr, fm-adr,checksum,cmd,length,postamble)
 * Checksum is calculated by adding together to-addr, fm-addr, cmd, length, data[0..n]
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/wdt.h>

#ifndef DEVICE_TYPE_COMPUTER
	#include <avr/io.h>
	#include <avr/interrupt.h>
#endif


#ifdef DEVICE_TYPE_MAIN_FRONT_UNIT
	#include "../front_panel/led_control.h"
	#include "../front_panel/event_handler.h"
	#include "../front_panel/errors.h"
#endif

#include "bus.h"
#include "bus_tx_queue.h"
#include "bus_rx_queue.h"
#include "bus_commands.h"
#include "bus_ping.h"
#include "global.h"
#include "bus_usart.h"

//! \brief The bus status structure
bus_status_struct bus_status;

//! \brief Variable used to calculate the checksum when receiving a message
unsigned char calc_checksum = 0;

//! The new message
BUS_MESSAGE bus_new_message;

/*! \brief Counter that keeps track of how long time ago it was when we received a
    new character and if it's over the limit we erase all the RX buffer */
unsigned char timer_bus_timeout = 0;

/*! \brief Counter that keeps track of how long time ago it was when we received a
    new SYNC message on the BUS */
unsigned int counter_sync_timeout = 0;

//! Counter which keeps track of each time the 130us timer counts up
unsigned int counter_130us = 0;

/*! \brief Init the communication bus */
void bus_init(void) {
	rx_queue_init();
	tx_queue_init();
	
	#ifdef DEVICE_TYPE_DRIVER_UNIT
		//57.600kpbs
		bus_usart_init(15);
	#endif
		
	#ifdef DEVICE_TYPE_POWERMETER_PICKUP
		//57.600kpbs
		bus_usart_init(15);
	#endif
	
			
	#ifdef DEVICE_TYPE_DRIVER_UNIT_V2
		//57.600kpbs
		bus_usart_init(15);
	#endif
	
	#ifdef DEVICE_TYPE_GENERAL_IO
		//57.600kpbs
		bus_usart_init(15);
	#endif
	
	#ifdef DEVICE_TYPE_ROTATOR_UNIT
			//57.600kpbs
		bus_usart_init(7);
	#endif
			
	#ifdef DEVICE_TYPE_MAIN_FRONT_UNIT
		//57.600kpbs
		bus_usart_init(15);
	#endif
    
  #ifdef DEVICE_TYPE_AMP_CTRL_BOX
    //57.600kpbs
    bus_usart_init(15);
  #endif
    

	bus_status.flags = 0;

	/* Calculate the lower limit of the bus time frame */
	bus_status.lower_limit = (bus_status.ext_addr * BUS_TIME_FRAME_LIMIT + BUS_SLOT_DEAD_TIME) / BUS_TIME_INTERRUPT_INTERVAL;
	/* Calculate the upper limit of the bus time frame */
	bus_status.upper_limit = (bus_status.ext_addr * BUS_TIME_FRAME_LIMIT + BUS_TIME_FRAME_LIMIT - BUS_SLOT_DEAD_TIME) / BUS_TIME_INTERRUPT_INTERVAL;

	bus_status.device_count = 0;

	bus_status.frame_counter = 0;

	bus_reset_tx_status();
	bus_reset_rx_status();
	
	bus_ping_init();
	
	bus_status.flags |= (1<<BUS_STATUS_RECEIVE_ON);
}

/*! \brief Set the address of this device on the bus.
 * \param addr The address of this device */
void bus_set_address(unsigned char addr) {
	bus_status.ext_addr = addr;
}

/*! \brief Returns if you are allowed to transmit data to the bus or not
 *  \return 1 if it's allowed to transmit and 0 if not */
unsigned char bus_allowed_to_send(void) {
	if (bus_status.flags & (1<<BUS_STATUS_MASTER_SENT_SYNC_BIT))// && (bus_status.flags & (1<<BUS_STATUS_ALLOWED_TO_SEND_BIT)))
		return(1);
	else
		return(0);
}

/*! \brief Returns the address of this device
	\return The address of this device */
unsigned char bus_get_address(void) {
	return(bus_status.ext_addr);
}

/*! \brief Sends the first message in the FIFO TX queue to the communication bus */
void bus_send_message(void) {
	/* Indicates that a message is currently being transmitted */
	bus_status.flags |= (1<<BUS_STATUS_SEND_ACTIVE);

	/* Turn off the receive enable, this is turned on again in the USART
	 * Transmit interrupt. Used to know when we are sending and somebody else */
	bus_status.flags &= ~(1<<BUS_STATUS_RECEIVE_ON);

	BUS_MESSAGE bus_message = tx_queue_get();


	#ifndef DEVICE_TYPE_COMPUTER
		bus_usart_transmit(BUS_MSG_PREAMBLE);
		bus_usart_transmit(BUS_MSG_PREAMBLE);
		bus_usart_transmit(bus_message.from_addr);
		bus_usart_transmit(bus_message.to_addr);
		bus_usart_transmit(bus_message.checksum);
		bus_usart_transmit(bus_message.flags);
		bus_usart_transmit(bus_message.cmd);
		bus_usart_transmit(bus_message.length);

		for (unsigned char i=0;i<bus_message.length;i++)
			bus_usart_transmit(bus_message.data[i]);

		bus_usart_transmit(BUS_MSG_POSTAMBLE);
	#endif

	// If the message is sent as an broadcast then we don't want any replies from the other units
	// and no resends will be made which means that we should drop the message from the TX queue.
	// This is also the case when it's just an ACK of a package or a NACK since we don't get any
	// confirmation of these anyway.
	if ((bus_message.to_addr == BUS_BROADCAST_ADDR) || (bus_message.cmd == BUS_CMD_ACK) || (bus_message.cmd == BUS_CMD_NACK)) {
		bus_status.flags &= ~(1<<BUS_STATUS_MESSAGE_ACK_TIMEOUT);
		tx_queue_drop();
		bus_reset_tx_status();
	}
	else {
		//Reset the timeout counter for resends
		bus_status.flags |= (1<<BUS_STATUS_MESSAGE_ACK_TIMEOUT);
		bus_status.wraparounds = 0;
	}

	/* Indicates that a message is currently NOT being transmitted */
	bus_status.flags &= ~(1<<BUS_STATUS_SEND_ACTIVE);
}

/*! \brief Function that resets the bus status variables */
void __inline__ bus_reset_tx_status(void) {
	bus_status.flags |= (1<<BUS_STATUS_SEND_MESSAGE);

	bus_status.wraparounds = 0;
	bus_status.send_count = 0;
}

/*! \brief Function that resets the bus status variables */
void __inline__ bus_reset_rx_status(void) {
	bus_status.flags &= ~(1<<BUS_STATUS_PREAMBLE_FOUND_BIT);
	
	bus_status.flags |= (1<<BUS_STATUS_ALLOWED_TO_SEND_BIT);
	
	bus_status.char_count = 0;
	calc_checksum = 0;
	bus_status.prev_char = 0;
  
  //Clear bus_new_message content
  bus_new_message.checksum = 0;
  bus_new_message.cmd = 0;
  bus_new_message.flags = 0;
  bus_new_message.from_addr = 0;
  bus_new_message.to_addr = 0;
  bus_new_message.length = 0;
  
  for (unsigned char i=0;i<BUS_MESSAGE_DATA_SIZE;i++)
    bus_new_message.data[i] = 0;
}


/*! \brief Returns if the bus is set to be master
	\return 1 if it is configured to be master, 0 otherwise */
unsigned char bus_is_master(void) {
	if (bus_status.flags & (1<< BUS_STATUS_DEVICE_IS_MASTER_BIT))
		return(1);
	else
		return(0);
}

/*! \brief Set the status if the device should be master or not
	\param state 1 if you wish the device to be master, 0 if you wish that it should be slave
	\param count The nr of devices */
void bus_set_is_master(unsigned char state, unsigned char count) {
	if (state) {
		bus_status.flags |= (1<<BUS_STATUS_DEVICE_IS_MASTER_BIT);

		bus_status.device_count = count;
		bus_status.device_count_mult = bus_status.device_count * BUS_TIME_MULTIPLIER;

		if (!(bus_status.flags & (1<<BUS_STATUS_ALLOWED_TO_SEND_BIT)))
			bus_status.flags |= (1<<BUS_STATUS_FORCE_SYNC);
	}
	else
		bus_status.flags &= ~(1<<BUS_STATUS_DEVICE_IS_MASTER_BIT);
}

/*! \brief Send an NOT acknowledge 
 *  \param to_addr Which address we wish to send the nack to
 *  \param error_type Why was the message nacked, see bus.h for more information about BUS errors */
void bus_send_nack(unsigned char to_addr, unsigned char error_type) {
	if (to_addr != BUS_BROADCAST_ADDR)
		bus_add_tx_message(bus_status.ext_addr,to_addr, 0, BUS_CMD_NACK, 1, &error_type);
}

/*! \brief Send an acknowledge */
void bus_send_ack(unsigned char to_addr) {
	bus_add_tx_message(bus_status.ext_addr,to_addr, 0, BUS_CMD_ACK, 0, NULL);
}

/*! \brief Receive the device count on the bus
 * \return The number of devices on the bus */
unsigned char bus_get_device_count(void) {
	return (bus_status.device_count);
}

/*! \brief Set the number of devices that are on the bus
 * \param device_count The number of devices on the bus, ie the number of time slots */
void bus_set_device_count(unsigned char device_count) {
	bus_status.device_count = device_count;

	bus_status.device_count_mult = bus_status.device_count*BUS_TIME_MULTIPLIER;
}

/*! \brief Resend the last message */
void  bus_resend_message(void) {
	if (bus_status.send_count < BUS_MAX_RESENDS) {
		bus_status.flags |= (1<<BUS_STATUS_SEND_MESSAGE);
	}
	else {
		#ifdef DEVICE_TYPE_MAIN_FRONT_UNIT
			//Set the error flag
			error_handler_set(ERROR_TYPE_BUS_RESEND,1,0);
		#endif
			
		tx_queue_drop();
		bus_reset_tx_status();
	}
}

/*! \brief Checks if there is anything that should be sent in the TX queue */
void bus_check_tx_status(void) {
	if ((bus_status.flags & (1<<BUS_STATUS_TIME_SLOT_ACTIVE)) && (bus_status.flags & (1<<BUS_STATUS_ALLOWED_TO_SEND_BIT))) {
		if (bus_status.flags & (1<<BUS_STATUS_SEND_MESSAGE)) {
			bus_status.flags &= ~(1<<BUS_STATUS_SEND_MESSAGE);

			bus_status.send_count++;
			bus_send_message();
		}
	}
	else if (bus_status.flags & (1<<BUS_STATUS_FORCE_SYNC)) {
		bus_status.flags &= ~(1<<BUS_STATUS_SEND_MESSAGE);
		bus_status.flags &= ~(1<<BUS_STATUS_FORCE_SYNC);

		bus_status.send_count++;
		bus_send_message();
	}
}

/*!\brief Adds a message to the TX queue which will be sent as soon as possible
* \param from_addr The address of the sender
* \param to_addr The address to the receiever
* \param flags Different flags, see defines
* \param cmd The command wanted to be performed
* \param length The length of the data wanting to be sent
* \param data The data wanted to be transmitted to the receiever */
void bus_add_tx_message(unsigned char from_addr, unsigned char to_addr, unsigned char flags, unsigned char cmd, unsigned char length, unsigned char data[]) {
	if ((cmd == BUS_CMD_SYNC) || (bus_allowed_to_send() != 0)) {
		unsigned char checksum = 0;

		BUS_MESSAGE bus_message;

		bus_message.from_addr = from_addr;
		checksum += from_addr;
		bus_message.to_addr = to_addr;
		checksum += to_addr;
		bus_message.flags = flags;
		checksum += flags;
		bus_message.cmd = cmd;
		checksum += cmd;
		bus_message.length = length;
		checksum += length;	

		for (unsigned char i=0;i<length;i++) {
			checksum += data[i];
			bus_message.data[i] = data[i];
		}
		
		bus_message.checksum = checksum;

		tx_queue_add(bus_message);
    
    #ifdef DEVICE_TYPE_MAIN_FRONT_UNIT
      main_update_critical_list();
    #endif
	}
}

/*!\brief Adds a message to the RX queue which will be sent as soon as possible
* \param from_addr The address of the sender
* \param to_addr The address to the receiever
* \param flags Different flags, see defines
* \param cmd The command performed
* \param length The length of the data received
* \param data The data receieved */
void bus_add_rx_message(unsigned char from_addr, unsigned char to_addr, unsigned char flags, unsigned char cmd, unsigned char length, unsigned char data[]) {
	BUS_MESSAGE bus_message;

	bus_message.from_addr = from_addr;
	bus_message.to_addr = to_addr;
	bus_message.flags = flags;
	bus_message.cmd = cmd;	
	bus_message.length = length;

	for (unsigned char i=0;i<length;i++) {
		bus_message.data[i] = data[i];
	}

	rx_queue_add(bus_message);
}

/*! \brief Goes through the TX queue and checks if a certain command exists 
 *  \param cmd The command we wish to check if it exists in the queue *
 *  \return 1 if it exist in the queue, 0 if not */
unsigned char bus_check_cmd_in_tx_queue(unsigned char cmd) {
  unsigned char ret_val = 0;
  
  for (unsigned char i=0;i<tx_queue_size();i++)
    if (tx_queue_get_pos(i).cmd == cmd) {
      ret_val = 1;
      break;
    }
    
  return(ret_val);
}

/*! \brief Adds the message bus_new_message into the RX queue */
void bus_add_new_message(void) {
	rx_queue_add(bus_new_message);
}

/*! \brief The message last sent was NACKED from the receiver
 *  \param addr The address of the device that sent the NACK
 *  \param error_type Contains information why the message was NACKED */
void bus_message_nacked(unsigned char addr, unsigned char error_type) {
	bus_status.flags &= ~(1<<BUS_STATUS_MESSAGE_ACK_TIMEOUT);

	BUS_MESSAGE bus_message;
	
	bus_message = tx_queue_get();
	
	//We need to make sure that this NACK was actually from the address we expected
	//Otherwise there is a risk that another device which has gotten corrupted information
	//will send a NACK to this device, even though the message wasn't supposed to be from that unit
	if (bus_message.to_addr == addr)
		bus_resend_message();
}

/*! \brief The message last sent was acknowledged from the receiver */
void bus_message_acked(unsigned char addr) {
	BUS_MESSAGE bus_message;
	
	bus_message = tx_queue_get();
	
	//We need to make sure that this ACK was actually from the address we expected
	//Otherwise there is a risk that another device which has gotten corrupted information
	//will send a ACK to this device, even though the message wasn't supposed to be from that unit
	if (bus_message.to_addr == addr) {
		bus_status.flags &= ~(1<<BUS_STATUS_MESSAGE_ACK_TIMEOUT);

		tx_queue_drop();
		bus_reset_tx_status();
	}
}

#ifndef DEVICE_TYPE_COMPUTER
/*!USART data interrupt*/
ISR(ISR_BUS_USART_DATA) {
}


/*!USART data receive interrupt */
ISR(ISR_BUS_USART_RECV) {
	//Reset the bus timeout since we have received a character
	timer_bus_timeout = 0;

	#ifdef DEVICE_TYPE_MAIN_FRONT_UNIT
		unsigned char data = UDR2;
	#endif

  #ifdef DEVICE_TYPE_STN_CTRL_BOARD
    unsigned char data = UDR2;
  #endif

  #ifdef DEVICE_TYPE_AMP_CTRL_BOARD
    unsigned char data = UDR2;
  #endif
	
  #ifdef DEVICE_TYPE_DRIVER_UNIT_V2
		unsigned char data = UDR1;
	#endif

	#ifdef DEVICE_TYPE_GENERAL_IO
		unsigned char data = UDR1;
	#endif

#ifdef DEVICE_TYPE_ROTATOR_UNIT
		unsigned char data = UDR1;
#endif
		
	#ifdef DEVICE_TYPE_DRIVER_UNIT
		unsigned char data = UDR;
	#endif

	#ifdef DEVICE_TYPE_POWERMETER_PICKUP
			unsigned char data = UDR;
	#endif		
      
  #ifdef DEVICE_TYPE_AMP_CTRL_BOX
      unsigned char data = UDR2;
  #endif
      
	
	bus_status.char_count++;

	if (bus_status.flags & (1<< BUS_STATUS_PREAMBLE_FOUND_BIT)) {
		//Check if this is a postamble
		if ((data == 0xFD) && (bus_new_message.length == (bus_status.char_count-7))) {		

			if ((bus_new_message.to_addr == bus_status.ext_addr) || (bus_new_message.to_addr == BUS_BROADCAST_ADDR)) {
				//Is the receiver ON? If not we don't parse the message since it's sent by ourself. This is needed because of the SYNC message if this
				//device is used as master.
				if (bus_status.flags & (1<<BUS_STATUS_RECEIVE_ON)) {
					//Is the checksum OK? In that case we add the new message to the RX queue, if not we send a NACK (not sent if it's a broadcast)
					if (calc_checksum == bus_new_message.checksum) {
						//Is the message a PING message? If so we should send it to the PING list
						if (bus_new_message.cmd == BUS_CMD_PING) {
							if (bus_new_message.length > 1)
								bus_ping_new_stamp(bus_new_message.from_addr, bus_new_message.data[0], bus_new_message.length-1, (unsigned char *)(bus_new_message.data+1));
							else
								bus_ping_new_stamp(bus_new_message.from_addr, bus_new_message.data[0], 0, 0);
							
							//If the device is a driver unit or a general I/O card, we also add the message to the message queue
							#ifdef DEVICE_TYPE_DRIVER_UNIT_V2
								if (bus_new_message.data[0] == DEVICE_ID_MAINBOX)
									bus_add_new_message();
							#endif
								
							#ifdef DEVICE_TYPE_GENERAL_IO
								if (bus_new_message.data[0] == DEVICE_ID_MAINBOX)
									bus_add_new_message();
							#endif
						}
						else {
              if (bus_new_message.cmd == BUS_CMD_SHUTTING_DOWN)
                bus_ping_clear_device(bus_new_message.from_addr);
              
							bus_add_new_message();
					
							if (bus_new_message.flags & (1<<BUS_MESSAGE_FLAGS_NEED_ACK))
								bus_send_ack(bus_new_message.from_addr);
						}
					}
					else if (bus_new_message.to_addr != BUS_BROADCAST_ADDR) //SEND NACK
						bus_send_nack(bus_new_message.from_addr, BUS_CHECKSUM_ERROR);
				}
			}
			
			bus_reset_rx_status();
			bus_status.prev_char = data;
			return;
		}
		else {
			switch(bus_status.char_count) {
				case 1 :	bus_new_message.from_addr = data;
									calc_checksum += data;
									break;
				case 2 :	bus_new_message.to_addr = data;
									calc_checksum += data;
									break;
				case 3 :	bus_new_message.checksum = data;
									break;
				case 4 :	bus_new_message.flags = data;
									calc_checksum += data;
									break;
				case 5 :	bus_new_message.cmd = data;
									calc_checksum += data;
									break;
				case 6 :	bus_new_message.length = data;
									calc_checksum += data;
									break;
				default:	if ((bus_status.char_count-7) < BUS_MESSAGE_DATA_SIZE) { //Check so that we don't write larger than the buffer size
                    if ((bus_status.char_count-7) < bus_new_message.length) {
                      bus_new_message.data[bus_status.char_count - 7] = data;

                      //The command is a SYNC message, reset the timer
                      if (bus_new_message.cmd == BUS_CMD_SYNC) {
                        TCNT2 = 0;	//Clear the counter so that all units have about the same baseline
                        
                        bus_status.device_count = data;
                        bus_status.device_count_mult = (data * BUS_TIME_MULTIPLIER);

                        bus_status.frame_counter = 0;

                        //Indicate that atleast one SYNC has been received
                        bus_status.flags |= (1<<BUS_STATUS_ALLOWED_TO_SEND_BIT);
                        bus_status.flags |= (1<<BUS_STATUS_MASTER_SENT_SYNC_BIT);

                        //Reset the counter keeping track of how long ago we last receieved a SYNC
                        //message from the master.
                        counter_sync_timeout = 0;
                      }

                      calc_checksum += data;
                    }
                    else {
                      bus_reset_rx_status();
                      return;
                    }
                  }
                  else { //If we have written larger than the buffer size, reset the RX and let it start over
                    bus_reset_rx_status();
                    return;
                  }

                  break;
			}
		}
	}
	else if (data == 0xFE) {
		//Is this a preamble or just a variable sent?
		if (bus_status.char_count >= 2)
			if (bus_status.prev_char == 0xFE) {
				bus_status.flags &= ~(1<<BUS_STATUS_ALLOWED_TO_SEND_BIT);
				bus_status.char_count = 0;
				calc_checksum = 0;
				bus_status.flags |= (1 << BUS_STATUS_PREAMBLE_FOUND_BIT);
			}
	}

	bus_status.prev_char = data;
}

/*!USART data transmit interrupt */
ISR(ISR_BUS_USART_TRANS) {
	if (!(bus_status.flags & (1<<BUS_STATUS_SEND_ACTIVE)))
		bus_status.flags |= (1<<BUS_STATUS_RECEIVE_ON);
}

/*! Timer interrupt with ~130us intervals */
ISR(ISR_BUS_TIMER_COMPARE) {
	/* If the bus hasn't received a message in a certain amount of time we clear it's status
	   allowing other devices to transmit data again */
	if (timer_bus_timeout >= BUS_TIMEOUT_LIMIT) {
		bus_reset_rx_status();

		timer_bus_timeout = 0;
	}

	//If we have not received a sync within a certain time we stop all communication
	//until we receive a new sync again
	if (!bus_is_master() && (counter_sync_timeout >= BUS_SYNC_TIMEOUT_LIMIT)) {
		bus_status.flags &= ~(1<<BUS_STATUS_MASTER_SENT_SYNC_BIT);
		
		//Drops all current messages in the TX queue
		tx_queue_dropall();

		counter_sync_timeout = 0;
		
		#ifdef DEVICE_TYPE_MAIN_FRONT_UNIT
			error_handler_set(ERROR_TYPE_BUS_SYNC,1,0);
		#endif
	}

	if (bus_status.wraparounds >= BUS_ACK_WRAPAROUND_LIMIT) {
		if (bus_status.flags & (1<<BUS_STATUS_MESSAGE_ACK_TIMEOUT)) {
			bus_resend_message();
			
			bus_status.wraparounds = 0;
		}
	}

	if (bus_status.device_count != 0) {
		if (((bus_status.frame_counter) >= bus_status.lower_limit) && ((bus_status.frame_counter) < bus_status.upper_limit)) {
			bus_status.flags |= (1<<BUS_STATUS_TIME_SLOT_ACTIVE);
		}
		else {
			bus_status.flags &= ~(1<<BUS_STATUS_TIME_SLOT_ACTIVE);
		}

		if (bus_status.frame_counter >= bus_status.device_count_mult) {
			bus_status.frame_counter = 0;
			bus_status.wraparounds++;
		}
		else
			bus_status.frame_counter++;
	}

	//Will update with 1.04 ms intervals
	if ((counter_130us % 8) == 0) {
		counter_sync_timeout++;
	}
	
	counter_130us++;
	timer_bus_timeout++;
}

#endif
