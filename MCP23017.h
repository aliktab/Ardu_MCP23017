/*
  Copyright (C) 2014 Alik <aliktab@gmail.com> 

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __MCP23017__
#define __MCP23017__

#include <inttypes.h>
#include <limits.h>

class MCP23017 
{
public:

  // Create MCP23017 object and link it with specified I2C address.
  MCP23017(uint8_t _i2c_addr);

  // Initialize MCP23017 and check it presence.
  bool initialize();  

  // Set pin mode (INPUT or OUTPUT)
  void set_pin_mode(uint8_t _pin, uint8_t _mode);

  // Switch on or off pull up resistor
  void pull_up_pin(uint8_t _pin, bool _pull_up);

  // Read one pin.
  uint8_t read_pin(uint8_t _pin) const;

  // Write one pin.
  void write_pin(uint8_t _pin, uint8_t _val);

  // Read all 16 pins, B in high byte and A in low byte.
  uint16_t read_gpio_ba() const;

  // Write all 16 pins, B in high byte and A in low byte.
  void write_gpio_ba(uint16_t _ba);

 private:

  uint8_t m_i2c_addr;
};

#endif // __MCP23017__
