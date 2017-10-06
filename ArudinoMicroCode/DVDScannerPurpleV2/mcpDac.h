/* Arduino WaveHC Library
 * Copyright (C) 2009 by William Greiman
 *  
 * This file is part of the Arduino WaveHC Library
 *  
 * This Library is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *  
 * You should have received a copy of the GNU General Public License
 * along with the Arduino WaveHC Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
/**
 *  Macros and inline functions for MCP4921 DAC
 */   
#ifndef mcpDac_h
#define mcpDac_h

#include <avr/io.h>
//#include "OTPinDefs.h"


//------------------------------------------------------------------------------
#define mcpDac1CsLow() MCP_DAC1_CS_PORT &= ~_BV(MCP_DAC1_CS_BIT)
#define mcpDac1CsHigh() MCP_DAC1_CS_PORT |= _BV(MCP_DAC1_CS_BIT)

#define mcpDac2CsLow() MCP_DAC2_CS_PORT &= ~_BV(MCP_DAC2_CS_BIT)
#define mcpDac2CsHigh() MCP_DAC2_CS_PORT |= _BV(MCP_DAC2_CS_BIT)

#define mcpDac3CsLow() MCP_DAC3_CS_PORT &= ~_BV(MCP_DAC3_CS_BIT)
#define mcpDac3CsHigh() MCP_DAC3_CS_PORT |= _BV(MCP_DAC3_CS_BIT)

#define mcpDacSckLow() MCP_DAC_SCK_PORT &= ~_BV(MCP_DAC_SCK_BIT)
#define mcpDacSckHigh() MCP_DAC_SCK_PORT |= _BV(MCP_DAC_SCK_BIT)
#define mcpDacSckPulse() {mcpDacSckHigh();mcpDacSckLow();}

#define mcpDacSdiLow() MCP_DAC_SDI_PORT &= ~_BV(MCP_DAC_SDI_BIT)
#define mcpDacSdiHigh() MCP_DAC_SDI_PORT |= _BV(MCP_DAC_SDI_BIT)
#define mcpDacSdiSet(v) if(v){mcpDacSdiHigh();}else{mcpDacSdiLow();}

// send bit b of d
#define mcpDacSendBit(d, b) {mcpDacSdiSet(d&_BV(b));mcpDacSckPulse();}

//------------------------------------------------------------------------------
// init dac I/O ports
inline void mcpDacInit(void) {
  // set all to output mode
  MCP_DAC1_CS_DDR |= _BV(MCP_DAC1_CS_BIT);
  MCP_DAC2_CS_DDR |= _BV(MCP_DAC2_CS_BIT);
  MCP_DAC3_CS_DDR |= _BV(MCP_DAC3_CS_BIT);

  MCP_DAC_SCK_DDR |= _BV(MCP_DAC_SCK_BIT);
  MCP_DAC_SDI_DDR |= _BV(MCP_DAC_SDI_BIT);
  // chip select high
  mcpDac1CsHigh();
  mcpDac2CsHigh();
  mcpDac3CsHigh();

#if USE_MCP_DAC_LDAC
  // LDAC low always - use unbuffered mode
  MCP_DAC_LDAC_DDR |= _BV(MCP_DAC_LDAC_BIT);
  MCP_DAC_LDAC_PORT &= ~_BV(MCP_DAC_LDAC_BIT);
#endif // USE_MCP_DAC_LDAC
}
//------------------------------------------------------------------------------
// send 12 bits to dac
// trusted compiler to optimize and it does 
// csLow to csHigh takes 8 - 9 usec on a 16 MHz Arduino
inline void mcpDac1ASend(uint16_t data) {
  mcpDac1CsLow();
  // send DAC config bits
  mcpDacSdiLow();
  mcpDacSckPulse();  // DAC A
  mcpDacSdiHigh();
  mcpDacSckPulse();  // buffered REF

  mcpDacSckPulse();  // 1X gain
  mcpDacSckPulse();  // no SHDN
  // send 12 data bits
  mcpDacSendBit(data, 11);
  mcpDacSendBit(data, 10);
  mcpDacSendBit(data,  9);
  mcpDacSendBit(data,  8);
  mcpDacSendBit(data,  7);
  mcpDacSendBit(data,  6);
  mcpDacSendBit(data,  5);
  mcpDacSendBit(data,  4);
  mcpDacSendBit(data,  3);
  mcpDacSendBit(data,  2);
  mcpDacSendBit(data,  1);
  mcpDacSendBit(data,  0);
  mcpDac1CsHigh();
}

inline void mcpDac1BSend(uint16_t data) {
  mcpDac1CsLow();
  // send DAC config bits
  mcpDacSdiHigh();
  mcpDacSckPulse();  // DAC B
  mcpDacSdiHigh();
  mcpDacSckPulse();  // buffered REF

  mcpDacSckPulse();  // 1X gain
  mcpDacSckPulse();  // no SHDN
  // send 12 data bits
  mcpDacSendBit(data, 11);
  mcpDacSendBit(data, 10);
  mcpDacSendBit(data,  9);
  mcpDacSendBit(data,  8);
  mcpDacSendBit(data,  7);
  mcpDacSendBit(data,  6);
  mcpDacSendBit(data,  5);
  mcpDacSendBit(data,  4);
  mcpDacSendBit(data,  3);
  mcpDacSendBit(data,  2);
  mcpDacSendBit(data,  1);
  mcpDacSendBit(data,  0);
  mcpDac1CsHigh();
}

inline void mcpDac2ASend(uint16_t data) {
  mcpDac2CsLow();
  // send DAC config bits
  mcpDacSdiLow();
  mcpDacSckPulse();  // DAC A
  mcpDacSdiHigh();
  mcpDacSckPulse();  // buffered REF

  mcpDacSckPulse();  // 1X gain
  mcpDacSckPulse();  // no SHDN
  // send 12 data bits
  mcpDacSendBit(data, 11);
  mcpDacSendBit(data, 10);
  mcpDacSendBit(data,  9);
  mcpDacSendBit(data,  8);
  mcpDacSendBit(data,  7);
  mcpDacSendBit(data,  6);
  mcpDacSendBit(data,  5);
  mcpDacSendBit(data,  4);
  mcpDacSendBit(data,  3);
  mcpDacSendBit(data,  2);
  mcpDacSendBit(data,  1);
  mcpDacSendBit(data,  0);
  mcpDac2CsHigh();
}

inline void mcpDac2BSend(uint16_t data) {
  mcpDac2CsLow();
  // send DAC config bits
  mcpDacSdiHigh();
  mcpDacSckPulse();  // DAC B
  mcpDacSdiHigh();
  mcpDacSckPulse();  // buffered REF

  mcpDacSckPulse();  // 1X gain
  mcpDacSckPulse();  // no SHDN
  // send 12 data bits
  mcpDacSendBit(data, 11);
  mcpDacSendBit(data, 10);
  mcpDacSendBit(data,  9);
  mcpDacSendBit(data,  8);
  mcpDacSendBit(data,  7);
  mcpDacSendBit(data,  6);
  mcpDacSendBit(data,  5);
  mcpDacSendBit(data,  4);
  mcpDacSendBit(data,  3);
  mcpDacSendBit(data,  2);
  mcpDacSendBit(data,  1);
  mcpDacSendBit(data,  0);
  mcpDac2CsHigh();
}


inline void mcpDac3ASend(uint16_t data) {
  mcpDac3CsLow();
  // send DAC config bits
  mcpDacSdiLow();
  mcpDacSckPulse();  // DAC A
  mcpDacSdiHigh();
  mcpDacSckPulse();  // buffered REF

  mcpDacSckPulse();  // 1X gain
  mcpDacSckPulse();  // no SHDN
  // send 12 data bits
  mcpDacSendBit(data, 11);
  mcpDacSendBit(data, 10);
  mcpDacSendBit(data,  9);
  mcpDacSendBit(data,  8);
  mcpDacSendBit(data,  7);
  mcpDacSendBit(data,  6);
  mcpDacSendBit(data,  5);
  mcpDacSendBit(data,  4);
  mcpDacSendBit(data,  3);
  mcpDacSendBit(data,  2);
  mcpDacSendBit(data,  1);
  mcpDacSendBit(data,  0);
  mcpDac3CsHigh();
}

inline void mcpDac3BSend(uint16_t data) {
  mcpDac3CsLow();
  // send DAC config bits
  mcpDacSdiHigh();
  mcpDacSckPulse();  // DAC B
  mcpDacSdiHigh();
  mcpDacSckPulse();  // buffered REF

  mcpDacSckPulse();  // 1X gain
  mcpDacSckPulse();  // no SHDN
  // send 12 data bits
  mcpDacSendBit(data, 11);
  mcpDacSendBit(data, 10);
  mcpDacSendBit(data,  9);
  mcpDacSendBit(data,  8);
  mcpDacSendBit(data,  7);
  mcpDacSendBit(data,  6);
  mcpDacSendBit(data,  5);
  mcpDacSendBit(data,  4);
  mcpDacSendBit(data,  3);
  mcpDacSendBit(data,  2);
  mcpDacSendBit(data,  1);
  mcpDacSendBit(data,  0);
  mcpDac3CsHigh();
}


#endif //mcpDac_h
