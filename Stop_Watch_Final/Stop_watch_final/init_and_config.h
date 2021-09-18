/*
 * init_and_config.h
 *
 *  Created on: Sep 13, 2021
 *      Author: yasser ahmed
 */

#ifndef INIT_AND_CONFIG_H_
#define INIT_AND_CONFIG_H_

#define SET_BIT(reg,pin) reg|=(1<<pin)
#define CLEAR_BIT(reg,pin) reg&=~(1<<pin)
#define TOGGLE_BIT(reg,pin) reg^=(1<<pin)

void init_pin();
void interrupt_config();
void timer_config();

#endif /* INIT_AND_CONFIG_H_ */
