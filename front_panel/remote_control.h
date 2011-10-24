/*! \file front_panel/remote_control.h
 *  \brief Remote control of the openASC box
 *  \ingroup front_panel_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "front_panel/remote_control.h" \endcode
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

#ifndef _REMOTE_CONTROL_H_
#define _REMOTE_CONTROL_H_

/*! Remote control description 
 *  REMOTE_CONTROL_RX_ANT_TEXT -> ant1 text REMOTE_CONTROL_TEXT_SEPERATOR ant2 text REMOTE_CONTROL_TEXT_SEPERATOR ant3 text
 *  REMOTE_CONTROL_ANT_TEXT -> ant1 text REMOTE_CONTROL_ANT_TEXT ant2 text REMOTE_CONTROL_ANT_TEXT ant3 text REMOTE_CONTROL_ANT_TEXT ant4 text

*/

void remote_control_activate_remote_mode(void);
void remote_control_deactivate_remote_mode(void);
unsigned char remote_control_get_remote_mode(void);
void remote_control_parse_command(unsigned char command, unsigned char length, char *data);

void remote_control_parse_button(unsigned char button);

void remote_control_parse_ascii_cmd(UC_MESSAGE *uc_message);

//! Command to activate the remote control mode
#define REMOTE_CONTROL_ACTIVATE_MODE		0x01
//! Command to deactivate the remote control mode
#define REMOTE_CONTROL_DEACTIVATE_MODE	0x02

//! A button should be pressed
#define REMOTE_CONTROL_BUTTON_PRESSED		0x10
//! Command for sending rx antenna button texts
#define REMOTE_CONTROL_RX_ANT_TEXT			0x11

#define REMOTE_CONTROL_BUTTON_PRESSED		0x10
#define REMOTE_CONTROL_RX_ANT_TEXT			0x11
#define REMOTE_CONTROL_ANT_TEXT					0x12
#define REMOTE_CONTROL_GET_STATUS				0x13
#define REMOTE_CONTROL_CHANGE_BAND			0x14

#define REMOTE_CONTROL_TEXT_SEPERATOR	0xFC

#endif