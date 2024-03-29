/*
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/



#ifndef SLOWTICKER_H
#define SLOWTICKER_H

#include "Module.h"

using namespace std;
#include <vector>

#include "libs/Hook.h"
#include "libs/Pin.h"

#include "system_LPC17xx.h" // for SystemCoreClock
#include <math.h>

class SlowTicker : public Module{
    public:
        SlowTicker();

        void on_module_loaded(void);
        void on_idle(void*);
        void on_gcode_received(void*);
        void on_gcode_execute(void*);

        void set_frequency( int frequency );
        void tick();
        // For some reason this can't go in the .cpp, see :  http://mbed.org/forum/mbed/topic/2774/?page=1#comment-14221
        template<typename T> Hook* attach( uint32_t frequency, T *optr, uint32_t ( T::*fptr )( uint32_t ) ){
            Hook* hook = new Hook();
            hook->interval = int(floor((SystemCoreClock/4)/frequency));
            hook->attach(optr, fptr);
            hook->countdown = hook->interval;
            if( frequency > this->max_frequency ){
                this->max_frequency = frequency;
                this->set_frequency(frequency);
            }
            this->hooks.push_back(hook);
            return hook;
        }

        bool flag_1s();

        vector<Hook*> hooks;
        uint32_t max_frequency;
        uint32_t interval;

        uint32_t g4_ticks;
        bool     g4_pause;

        Pin ispbtn;
protected:
    int flag_1s_count;
    volatile int flag_1s_flag;
};





#endif
