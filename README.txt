Arduino Library for the MCP23017 chip (used Arduino I2C Master Library).

The MCP23017/MCP23S17 (MCP23X17) device family provides 16-bit, general purpose parallel I/O
expansion for I2C bus or SPI applications. The two devices differ only in the serial interface.

• MCP23017 – I2C interface
• MCP23S17 – SPI interface

The MCP23X17 consists of multiple 8-bit configuration registers for input, output and polarity 
selection. The system master can enable the I/Os as either inputs or outputs by writing the I/O 
configuration bits (IODIRA/B). The data for each input or output is kept in the corresponding 
input or output register. The polarity of the Input Port register can be inverted with the Polarity
Inversion register. All registers can be read by the system master.

MCP23017 data-sheet can be found here: http://ww1.microchip.com/downloads/en/DeviceDoc/21952b.pdf

It uses I2C library for communication with MCP9808 chip. You have to initialize I2c 
instance on your own. Somehow like that:

// Initialize I2C interface.
I2c.begin();
I2c.setSpeed(true);

You can use the same I2c instance for cummunication with more then one chip at the same time.
