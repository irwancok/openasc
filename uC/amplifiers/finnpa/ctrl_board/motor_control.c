/*! \file 4cx1500b/ctrl_board/motor_control.c
 *  \brief Motor control code
 *  \ingroup ctrl_board_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-02-05
 *  \code #include "4cx1500b/ctrl_board/motor_control.c" \endcode
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

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#include "motor_control.h"

//! Variable which contains the current information of the stepper motor, positions etc
static struct_stepper_motor stepper_motor[3];

//! milli-second counter
static unsigned int counter_ms = 0;

static unsigned int motor_limit_up[3] = {MOTOR1_LIMIT_UP,MOTOR2_LIMIT_UP,MOTOR3_LIMIT_UP};
static unsigned int motor_limit_down[3] = {MOTOR1_LIMIT_DOWN,MOTOR2_LIMIT_DOWN,MOTOR3_LIMIT_DOWN};

/*! \brief Motor control init */
void motor_control_init(void) {
  for (unsigned char i=0;i<3;i++) {
    stepper_motor[i].current_pos = 0;
    stepper_motor[i].target_pos = 0;
    stepper_motor[i].current_tick = 0;
    stepper_motor[i].current_phase = 0;
    stepper_motor[i].current_dir = MOTOR_DIR_NONE;
  }
}

/*! \brief This function will make the stepper motor to go to a certain position 
 *  \param motor_index Which motor we wish to control
 *  \param pos Which position we wish to head to */
void motor_control_goto(unsigned char motor_index, unsigned int pos) {
  if (motor_index < 3) {
    if (stepper_motor[motor_index].current_pos != pos) {
      stepper_motor[motor_index].current_tick = 0;
      stepper_motor[motor_index].target_pos = pos;
      
      if (stepper_motor[motor_index].target_pos < stepper_motor[motor_index].current_pos) {
        stepper_motor[motor_index].current_dir = MOTOR_DIR_UP;
        stepper_motor[motor_index].current_phase = 0;
      }
      else if (stepper_motor[motor_index].target_pos > stepper_motor[motor_index].current_pos) {
        stepper_motor[motor_index].current_dir = MOTOR_DIR_DOWN;
        stepper_motor[motor_index].current_phase = 3;
      }
      
      stepper_motor[motor_index].next_tick = stepper_motor[motor_index].current_tick + MOTOR_CONTROL_STEP_DELAY;
    }
  }
}

/*! \brief This function will return the current position of a stepper motor
 *  \param motor_index Which motor we wish to get the position from
 *  \return The current position of the specified motor */
unsigned int motor_control_get_pos(unsigned char motor_index) {
  if (motor_index < 3)
    return(stepper_motor[motor_index].current_pos);

  return(0);
}

void motor_control_step_motor1(void) {
/*  if (stepper_motor[0].current_phase == 0) {
		PORTL |= (1<<2);
		PORTL &= ~(1<<3);
		PORTL &= ~(1<<4);
		PORTL &= ~(1<<5);
  }
  else if (stepper_motor[0].current_phase == 1) {
		PORTL |= (1<<2);
		PORTL |= (1<<3);
		PORTL &= ~(1<<4);
		PORTL &= ~(1<<5);
  }
  else if (stepper_motor[0].current_phase == 2) {
		PORTL &= ~(1<<2);
		PORTL |= (1<<3);
		PORTL &= ~(1<<4);
		PORTL &= ~(1<<5);
  }
  else if (stepper_motor[0].current_phase == 3) {
		PORTL &= ~(1<<2);
		PORTL |= (1<<3);
		PORTL |= (1<<4);
		PORTL &= ~(1<<5);
  } 
	else if (stepper_motor[0].current_phase == 4) {
		PORTL &= ~(1<<2);
		PORTL &= ~(1<<3);
		PORTL |= (1<<4);
		PORTL &= ~(1<<5);
  }
  else if (stepper_motor[0].current_phase == 5) {
		PORTL &= ~(1<<2);
		PORTL &= ~(1<<3);
		PORTL |= (1<<4);
		PORTL |= (1<<5);
  }
  else if (stepper_motor[0].current_phase == 6) {
		PORTL &= ~(1<<2);
		PORTL &= ~(1<<3);
		PORTL &= ~(1<<4);
		PORTL |= (1<<5);
  }
  else if (stepper_motor[0].current_phase == 7) {
		PORTL |= (1<<2);
		PORTL &= ~(1<<3);
		PORTL &= ~(1<<4);
		PORTL |= (1<<5);
  } */
  if (stepper_motor[0].current_phase == 0) {
		PORTL |= (1<<2);
		PORTL &= ~(1<<3);
		PORTL &= ~(1<<4);
		PORTL |= ~(1<<5);
  }
  else if (stepper_motor[0].current_phase == 1) {
		PORTL |= (1<<2);
		PORTL |= (1<<3);
		PORTL &= ~(1<<4);
		PORTL &= ~(1<<5);
  }
  else if (stepper_motor[0].current_phase == 2) {
		PORTL &= ~(1<<2);
		PORTL |= (1<<3);
		PORTL |= ~(1<<4);
		PORTL &= ~(1<<5);
  }
  else if (stepper_motor[0].current_phase == 3) {
		PORTL &= ~(1<<2);
		PORTL &= ~(1<<3);
		PORTL |= (1<<4);
		PORTL |= ~(1<<5);
  } 
}

void motor_control_step_motor2(void) {
  
}

void motor_control_step_motor3(void) {
  
}

/*! \brief This function is called from the main loop, and is called as often as possible */
void motor_control_process(void) {
  for (unsigned char i=0;i<3;i++) {
    if (stepper_motor[i].current_tick >= stepper_motor[i].next_tick) {
      //TODO: POLL the A/D value and compare it to the target position
      
      //Set when the next step should occur
      stepper_motor[i].next_tick += MOTOR_CONTROL_STEP_DELAY;
      
      if (stepper_motor[i].current_dir == MOTOR_DIR_DOWN) {
				if (get_ad_curr_val(i) > motor_limit_down[i]) {
					if (stepper_motor[i].current_phase > 0)
						stepper_motor[i].current_phase--;
					else
						stepper_motor[i].current_phase = 3;
				}
				else {
					stepper_motor[i].current_dir = MOTOR_DIR_NONE;
				}
      }
      else if (stepper_motor[i].current_dir == MOTOR_DIR_UP) {
				if (get_ad_curr_val(i) <= motor_limit_up[i]) {
					if (stepper_motor[i].current_phase < 3)
						stepper_motor[i].current_phase++;
					else
						stepper_motor[i].current_phase = 0;
				}
				else {
					stepper_motor[i].current_dir = MOTOR_DIR_NONE;
				}					
      }
      
			if (stepper_motor[i].current_dir != MOTOR_DIR_NONE) {
				if (i == 0)
					motor_control_step_motor1();
				else if (i == 1)
					motor_control_step_motor2();
				else if (i == 2)
					motor_control_step_motor3();
			}
    }
  }
}

/*! \brief This function should be called each ms */
void motor_control_tick(void) {
  counter_ms++;
  
  stepper_motor[0].current_tick++;
  stepper_motor[1].current_tick++;
  stepper_motor[2].current_tick++;
}

void motor_control_set_phase(unsigned char motor_index, unsigned char phase) {
	stepper_motor[motor_index].current_phase = phase;
}