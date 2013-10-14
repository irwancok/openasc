/*! \file ext_events.h
 *  \brief External events
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-12-11
 *  \code #include "ext_events.h" \endcode */
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

#ifndef _EXT_EVENTS_H_
#define _EXT_EVENTS_H_

/* EXTERNAL CTRL FUNCTIONS */
//! Ext ctrl - No function assigned
#define EXT_CTRL_SEL_NONE             0
//! Ext ctrl - Set RX antenna #1
#define EXT_CTRL_SEL_RX_ANT1          1
//! Ext ctrl - Set RX antenna #2
#define EXT_CTRL_SEL_RX_ANT2          2
//! Ext ctrl - Set RX antenna #3
#define EXT_CTRL_SEL_RX_ANT3          3
//! Ext ctrl - Set RX antenna #4
#define EXT_CTRL_SEL_RX_ANT4          4
//! Ext ctrl - Set RX antenna #5
#define EXT_CTRL_SEL_RX_ANT5          5
//! Ext ctrl - Set RX antenna #6
#define EXT_CTRL_SEL_RX_ANT6          6
//! Ext ctrl - Set RX antenna #7
#define EXT_CTRL_SEL_RX_ANT7          7
//! Ext ctrl - Set RX antenna #8
#define EXT_CTRL_SEL_RX_ANT8          8
//! Ext ctrl - Set RX antenna #9
#define EXT_CTRL_SEL_RX_ANT9          9
//! Ext ctrl - Set RX antenna #10
#define EXT_CTRL_SEL_RX_ANT10         10
//! Ext ctrl - Set RX antenna #11
#define EXT_CTRL_SEL_RX_ANT11         11
//! Ext ctrl - Set RX antenna #12
#define EXT_CTRL_SEL_RX_ANT12         12

//! Ext ctrl - Toggle TX antenna combination #1
#define EXT_CTRL_TOGGLE_TX_ANT1     13
//! Ext ctrl - Toggle TX antenna combination #2
#define EXT_CTRL_TOGGLE_TX_ANT2     14
//! Ext ctrl - Toggle TX antenna combination #3
#define EXT_CTRL_TOGGLE_TX_ANT3     15
//! Ext ctrl - Toggle TX antenna combination #4
#define EXT_CTRL_TOGGLE_TX_ANT4     16

//! Ext ctrl - Toggle RX antenna combination #1
#define EXT_CTRL_TOGGLE_RX_ANT1     17
//! Ext ctrl - Toggle RX antenna combination #2
#define EXT_CTRL_TOGGLE_RX_ANT2     18
//! Ext ctrl - Toggle RX antenna combination #3
#define EXT_CTRL_TOGGLE_RX_ANT3     19
//! Ext ctrl - Toggle RX antenna combination #4
#define EXT_CTRL_TOGGLE_RX_ANT4     20

//! Ext ctrl - Toggle RX antenna enabled
#define EXT_CTRL_TOGGLE_RX_ANT_MODE 21
//! Ext ctrl - Toggle TX/RX mode on/off
#define EXT_CTRL_TOGGLE_TXRX_MODE   22

//! Ext ctrl - Select array direction #1
#define EXT_CTRL_SET_ARRAY_DIR1     23
//! Ext ctrl - Select array direction #2
#define EXT_CTRL_SET_ARRAY_DIR2     24
//! Ext ctrl - Select array direction #3
#define EXT_CTRL_SET_ARRAY_DIR3     25
//! Ext ctrl - Select array direction #4
#define EXT_CTRL_SET_ARRAY_DIR4     26
//! Ext ctrl - Select array direction #5
#define EXT_CTRL_SET_ARRAY_DIR5     27
//! Ext ctrl - Select array direction #6
#define EXT_CTRL_SET_ARRAY_DIR6     28
//! Ext ctrl - Select array direction #7
#define EXT_CTRL_SET_ARRAY_DIR7     29
//! Ext ctrl - Select array direction #8
#define EXT_CTRL_SET_ARRAY_DIR8     30

//! Ext ctrl - Select stack combo #1
#define EXT_CTRL_SET_STACK_COMB1      31
//! Ext ctrl - Select stack combo #2
#define EXT_CTRL_SET_STACK_COMB2      32
//! Ext ctrl - Select stack combo #3
#define EXT_CTRL_SET_STACK_COMB3      33
//! Ext ctrl - Select stack combo #4
#define EXT_CTRL_SET_STACK_COMB4      34
//! Ext ctrl - Select stack combo #5
#define EXT_CTRL_SET_STACK_COMB5      35
//! Ext ctrl - Select stack combo #6
#define EXT_CTRL_SET_STACK_COMB6      36

//! Ext ctrl - Toggle the amplifier on/off
#define EXT_CTRL_AMPLIFIER_TOGGLE_ON_OFF  37
//! Ext ctrl - Toggle the amplifier standby
#define EXT_CTRL_AMPLIFIER_TOGGLE_STANDBY 38
//! Ext ctrl - Tune the amplifier to the correct band
#define EXT_CTRL_AMPLIFIER_TUNE 39
//! Ext ctrl - Reset the amplifier
#define EXT_CTRL_AMPLIFIER_RESET  40

//! Ext ctrl - Rotate the last antenna clock wise
#define EXT_CTRL_ROTATE_CW  41
//! Ext ctrl - Rotate the last antenna counter clock wise
#define EXT_CTRL_ROTATE_CCW 42
//! Ext ctrl - Rotate stop
#define EXT_CTRL_ROTATE_STOP 43

//! Ext ctrl - Select band portion
#define EXT_CTRL_SEL_BAND_PORTION 44
//! Ext ctrl - Change band up
#define EXT_CTRL_CHANGE_BAND_UP   45
//! Ext ctrl - Change band down
#define EXT_CTRL_CHANGE_BAND_DOWN 46
//! Ext ctrl - Set no RX ant
#define EXT_CTRL_SEL_RX_NONE 47
//! Ext ctrl - Set amplifier band segment 1
#define EXT_CTRL_SEL_AMPLIFIER_SEGMENT_1  48
//! Ext ctrl - Set amplifier band segment 2
#define EXT_CTRL_SEL_AMPLIFIER_SEGMENT_2  49
//! Ext ctrl - Set amplifier band segment 3
#define EXT_CTRL_SEL_AMPLIFIER_SEGMENT_3  50
//! Ext ctrl - Set amplifier band segment 4
#define EXT_CTRL_SEL_AMPLIFIER_SEGMENT_4  51
//! Ext ctrl - Set amplifier band segment 5
#define EXT_CTRL_SEL_AMPLIFIER_SEGMENT_5  52
//! Ext ctrl - Set amplifier band segment 6
#define EXT_CTRL_SEL_AMPLIFIER_SEGMENT_6  53
//! Ext ctrl - Tune the amplifier to current band and segment 1
#define EXT_CTRL_TUNE_AMPLIFIER_SEGMENT_1  54
//! Ext ctrl - Tune the amplifier to current band and segment 2
#define EXT_CTRL_TUNE_AMPLIFIER_SEGMENT_2  55
//! Ext ctrl - Tune the amplifier to current band and segment 3
#define EXT_CTRL_TUNE_AMPLIFIER_SEGMENT_3  56
//! Ext ctrl - Tune the amplifier to current band and segment 4
#define EXT_CTRL_TUNE_AMPLIFIER_SEGMENT_4  57
//! Ext ctrl - Tune the amplifier to current band and segment 5
#define EXT_CTRL_TUNE_AMPLIFIER_SEGMENT_5  58
//! Ext ctrl - Tune the amplifier to current band and segment 6
#define EXT_CTRL_TUNE_AMPLIFIER_SEGMENT_6  59

//! Ext ctrl - Rotate button pressed
#define EXT_CTRL_TOGGLE_ROTATE_BUTTON   60
//! Ext ctrl - Sub menu button pressed
#define EXT_CTRL_TOGGLE_SUBMENU_BUTTON  61
//! Ext ctrl - Menu button pressed
#define EXT_CTRL_TOGGLE_MENU_BUTTON     62
//! Ext ctrl - Knob rotate CW
#define EXT_CTRL_TOGGLE_KNOB_ROTATE_CW  63
//! Ext ctrl - Knob rotate CCW
#define EXT_CTRL_TOGGLE_KNOB_ROTATE_CCW 64
//! Ext ctrl - Knob pressed
#define EXT_CTRL_TOGGLE_KNOB_BUTTON     65
#endif