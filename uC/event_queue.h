/*! \file event_queue.h
 *  \brief Event queue
 *  \defgroup event_queue_group Event QUEUE library
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "event_queue.h" \endcode
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

#ifndef _EVENT_QUEUE_H_
#define _EVENT_QUEUE_H_

//! Event message used for timing of events
typedef struct {
	//!The function we wish to run at the specified time
	void (*func)(void);
	//!The target time where we wish to event to occur
	unsigned int time_target;
	//!The event id, can be used to drop a certain type of messages
	unsigned char id;
} EVENT_MESSAGE;

//! Event list with size EVENT_LIST_SIZE
EVENT_MESSAGE event_list[EVENT_LIST_SIZE];

/*! Sequencer message types, this is used to we can keep track of different
    messages in the event queue. So if an event is aborted we can easily just 
    remove the upcoming events from the queue, that doesn't need to be executed */

//! Event that the radio should be PTT:ed from footswitch
#define SEQUENCER_EVENT_TYPE_PTT_TX_ACTIVE_ON 1
//! Event that the inhibit output should be on
#define SEQUENCER_EVENT_TYPE_PTT_INHIBIT_ON   2
//! Event that the radio should be PTT:ed from footswitch
#define SEQUENCER_EVENT_TYPE_PTT_RADIO_ON     3
//! Event that the amp should be PTT:ed from footswitch
#define SEQUENCER_EVENT_TYPE_PTT_AMP_ON       4
//! Event that the inhibit should be activated from footswitch
#define SEQUENCER_EVENT_TYPE_PTT_TX_ACTIVE_OFF  5
//! Event that the TX active output should be off
#define SEQUENCER_EVENT_TYPE_PTT_INHIBIT_OFF    6
//! Event that the radio should be deactivated from footswitch
#define SEQUENCER_EVENT_TYPE_PTT_RADIO_OFF      7
//! Event that the amp should be deactivated from footswitch
#define SEQUENCER_EVENT_TYPE_PTT_AMP_OFF        8
//! Event that the inhibit should be deactiated from footswitch
#define SEQUENCER_EVENT_TYPE_PTT_INHIBIT_OFF    9

/*! We will update the PTT status after this has run */
#define EVENT_TYPE_CRITICAL_CMD_UPDATE  10

void event_queue_init(void);
char event_queue_add(EVENT_MESSAGE event);
EVENT_MESSAGE event_queue_get(void);
void event_queue_drop(void);
unsigned char event_queue_count(void);
void event_queue_dropall(void);
unsigned char event_in_queue(void);
void event_queue_wrap(unsigned int remove_val);
int event_queue_drop_id(unsigned char id);
unsigned char event_queue_check_id(unsigned char id);

#endif
