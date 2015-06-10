/*
 * Author: Sisinty Sasmita Patra <sisinty.s.patra@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "jhd1313m1.h"   // Lcd Display
#include "grove.h"       // Button, Temp, Light, Rotary Sensors
#include "mma7660.h"     // 3-Axis Digital Accelerometer
#include "buzzer.h"      // Buzzer
#include "mic.h"         // Sound Sensor
#include "ttp223.h"  	 // Touch Sensor
#include "ldt0028.h"     // Piezo vibration Sensor

#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

using namespace std;
using namespace upm;

int
main(int argc, char **argv)
{
    // Create the temperature sensor object using AIO pin 1 
    upm::GroveTemp* temp = new upm::GroveTemp(1);
   
    // Create the lcd sensor object using I2C pin 
    upm::Jhd1313m1 *lcd = new upm::Jhd1313m1(0, 0x3E, 0x62);

    // Create the button object using GPIO pin 2 
    upm::GroveButton* button = new upm::GroveButton(2);

    // Create the rotary Sensor object using AIO pin 0 
    upm::GroveRotary* knob = new upm::GroveRotary(0);

    // Create the light sensor object using AIO pin 2
    upm::GroveLight* light = new upm::GroveLight(2);

    // Create the TTP223 touch sensor object using GPIO pin 4
    upm::TTP223* touch = new upm::TTP223(4);

    int celsius;
    float rotary;
    int lit;
    bool tuch;

    // This for loop is required to get the updated value from respective sensors
    for (int i=0; i < 5; i++) {
    	celsius = temp->value();
    	rotary = knob->abs_deg();
    	lit = light->value();
	tuch = touch->isPressed();
    }

    lcd->setCursor(0,0);
    lcd->write("welcome to ");
    lcd->setCursor(1,0);
    lcd->write("Starter Kit!");

    sleep(3);
    lcd->clear();

    /* This variable helps to display one particular sensor at any given time, when button value is '1' */
    int x = 0;

    /* 	This while loop continously checks for button value. 
     *  if button value is '0', the LCD displays "press button".
     *	if button value is '1', the sensors values are displayed depending on x value 
    */ 
    while(1)
    {
    	if(button->value() == 0)
    	{
    	    lcd->setCursor(0,0);
    	    lcd->write("Press Button ");
    	}
    	else
    	{
    		if(x == 0)
    		{
    			celsius = temp->value();

			// Since LCD displays data in string format, we need to convert (celsius value) from integer to string
    			string tdata = static_cast<ostringstream*>( &(ostringstream() << celsius))-> str();
    			lcd->clear();
    			lcd->setCursor(0,0);
    			lcd->write("Temperature in ");
    			lcd->setCursor(1,2);
    			lcd->write("celsius: ");
    			lcd->write(tdata);

    			sleep(3);
    			lcd->clear();
    			x += 1;
    		}

    		else if(x == 1)
    		{
    			rotary = knob->abs_deg();
    			string rotdata = static_cast<ostringstream*>( &(ostringstream() << rotary))-> str();
    			lcd->clear();
    			lcd->setCursor(0,0);
    			lcd->write("Rotatory Angle ");
    			lcd->setCursor(1,2);
    			lcd->write("in degree: ");
    			lcd->write(rotdata);

    			sleep(3);
    			lcd->clear();
    			x += 1;
    		}

    		else if(x == 2)
    		{
    			lit = light->value();
    			string litdata = static_cast<ostringstream*>( &(ostringstream() << lit))-> str();
    			lcd->clear();
    			lcd->setCursor(0,0);
    			lcd->write("light value ");
    			lcd->setCursor(1,2);
    			lcd->write("in lux: ");
    			lcd->write(litdata);

    			sleep(3);
    			lcd->clear();
    			x += 1;
    		}

		else if(x == 3)
		{
			tuch = touch->isPressed();
    		    	string touchdata = static_cast<ostringstream*>( &(ostringstream() << tuch))-> str();
    		    	lcd->clear();
			lcd->setCursor(0,0);
			lcd->write("Touch Sensor ");
			lcd->setCursor(1,2);
			lcd->write("is pressed: ");
			lcd->write(touchdata);

	    		sleep(3);
			lcd->clear();
			x = 0;
		}
    	}
    }

    // Delete button sensor object
    delete button;
    // Delete temperature sensor object
    delete temp;
    // Delete rotary angle sensor object
    delete knob;
    // Delete the light sensor object
    delete light;
    // Delete the touch sensor object
    delete touch;

    return 0;
}
