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

#include <I2C.h>
#include <MCP23017.h>


#define MCP23017_ADDRESS  (uint8_t)0x20
#define MCP23017_ADDRMASK (uint8_t)0x07

// registers
#define MCP23017_IODIRA   (uint8_t)0x00
#define MCP23017_IPOLA    (uint8_t)0x02
#define MCP23017_GPINTENA (uint8_t)0x04
#define MCP23017_DEFVALA  (uint8_t)0x06
#define MCP23017_INTCONA  (uint8_t)0x08
#define MCP23017_IOCONA   (uint8_t)0x0A
#define MCP23017_GPPUA    (uint8_t)0x0C
#define MCP23017_INTFA    (uint8_t)0x0E
#define MCP23017_INTCAPA  (uint8_t)0x10
#define MCP23017_GPIOA    (uint8_t)0x12
#define MCP23017_OLATA    (uint8_t)0x14

#define MCP23017_IODIRB   (uint8_t)0x01
#define MCP23017_IPOLB    (uint8_t)0x03
#define MCP23017_GPINTENB (uint8_t)0x05
#define MCP23017_DEFVALB  (uint8_t)0x07
#define MCP23017_INTCONB  (uint8_t)0x09
#define MCP23017_IOCONB   (uint8_t)0x0B
#define MCP23017_GPPUB    (uint8_t)0x0D
#define MCP23017_INTFB    (uint8_t)0x0F
#define MCP23017_INTCAPB  (uint8_t)0x11
#define MCP23017_GPIOB    (uint8_t)0x13
#define MCP23017_OLATB    (uint8_t)0x15


MCP23017::MCP23017(uint8_t _i2c_addr)
{
  m_i2c_addr = MCP23017_ADDRESS | (_i2c_addr & MCP23017_ADDRMASK);
}

bool MCP23017::initialize()
{
  if (I2c.write(m_i2c_addr, MCP23017_IODIRA, (uint8_t)0xFF)) // all inputs on port A
    return false;

  if (I2c.write(m_i2c_addr, MCP23017_IODIRB, (uint8_t)0xFF)) // all inputs on port B
    return false;

  return true;
}

void MCP23017::set_pin_mode(uint8_t _pin, uint8_t _mode) 
{
  // Only 16 bits available.
  if (_pin > 15) return;

  // Select register by pin number.
  uint8_t reg_id = MCP23017_IODIRA;
  if (_pin >= 8)
  {
    reg_id = MCP23017_IODIRB;
    _pin -= 8;
  }

  // Read the current IODIR.
  I2c.read(m_i2c_addr, reg_id, (uint8_t)1);
  uint8_t iodir = I2c.receive();

  // Set the pin and direction.
  if (INPUT == _mode) 
    iodir |=  (1 << _pin); 
  else
    iodir &= ~(1 << _pin);

  // Write the new IODIR
  I2c.write(m_i2c_addr, reg_id, iodir);
}

void MCP23017::pull_up_pin(uint8_t _pin, bool _pull_up) 
{
  // Only 16 bits available.
  if (_pin > 15) return;

  // Select register by pin number.
  uint8_t reg_id = MCP23017_GPPUA;
  if (_pin >= 8)
  {
    reg_id = MCP23017_GPPUB;
    _pin -= 8;
  }

  // Read the current pullup resistor set.
  I2c.read(m_i2c_addr, reg_id, (uint8_t)1);
  uint8_t gppu = I2c.receive();

  // set the pin and direction
  if (_pull_up) 
    gppu |=  (1 << _pin); 
  else
    gppu &= ~(1 << _pin);

  // Write the new GPIO.
  I2c.write(m_i2c_addr, reg_id, gppu);
}

uint8_t MCP23017::read_pin(uint8_t _pin) const 
{
  // Only 16 bits available.
  if (_pin > 15) return 0;

  // Select register by pin number.
  uint8_t reg_id = MCP23017_GPIOA;
  if (_pin >= 8)
  {
    reg_id = MCP23017_GPIOB;
    _pin -= 8;
  }

  // Read the current GPIO
  I2c.read(m_i2c_addr, reg_id, (uint8_t)1);
  return (I2c.receive() >> _pin) & 0x1;
}

void MCP23017::write_pin(uint8_t _pin, uint8_t _val) 
{
  // Only 16 bits available.
  if (_pin > 15) return;

  // Select register by pin number.
  uint8_t reg_olat = MCP23017_OLATA;
  uint8_t reg_gpio = MCP23017_GPIOA;
  if (_pin >= 8) 
  {
    reg_olat = MCP23017_OLATB;
    reg_gpio = MCP23017_GPIOB;
    _pin -= 8;
  }

  // Read the current GPIO output latches
  I2c.read(m_i2c_addr, reg_olat, (uint8_t)1);
  uint8_t gpio = I2c.receive();

  // Set the pin and direction
  if (_val) 
    gpio |=  (1 << _pin);
  else
    gpio &= ~(1 << _pin);

  // Write the new GPIO
  I2c.write(m_i2c_addr, reg_gpio, gpio);
}

uint16_t MCP23017::read_gpio_ba() const 
{
  uint8_t Data[2] = { 0, 0 };

  I2c.read(m_i2c_addr, MCP23017_GPIOA, sizeof(Data), Data);

  return (Data[1] << 8) | Data[0];
}

void MCP23017::write_gpio_ba(uint16_t _ba) 
{
  uint8_t Data[2] =
  {
    (_ba & 0xff), (_ba >> 8) & 0xff
  };

  I2c.write(m_i2c_addr, MCP23017_GPIOA, Data, sizeof(Data));
}
