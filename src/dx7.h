/**
 *
 * Copyright (c) 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 *
 */

#ifndef DX7_H_INCLUDED
#define DX7_H_INCLUDED

#include "msfa/synth.h"
#include "msfa/synth_unit.h"
#include "msfa/ringbuffer.h"

void _trace(const char *source, const char *fmt, ...);

#define DX7_ID "0.0.1"

#ifdef DEBUG
    #define DX7_VERSION DX7_ID " DEBUG"
    #define TRACE(fmt, ...) _trace(__PRETTY_FUNCTION__,fmt,##__VA_ARGS__)
#else
    #define DX7_VERSION DX7_ID
    #define TRACE(fmt, ...)
#endif

//==============================================================================
// Global vars
RingBuffer ring_buffer_;
static const float scaler = 0.00003051757813;

//==============================================================================

class DX7_Voice : public lvtk::Voice
{
  public:
    DX7_Voice(double rate);
    ~DX7_Voice();
    void on(unsigned char key, unsigned char velocity);
    void off(unsigned char velocity);
    unsigned char get_key(void) const;
//    void render(uint32_t from, uint32_t to);
//    void post_process(uint32_t from, uint32_t to);
    void add_midi(uint8_t msg1, uint8_t msg2, uint8_t msg3);

  protected:
    unsigned char m_key;
    double m_rate;

  private:
};

//==============================================================================

class DX7 : public lvtk::Synth<DX7_Voice, DX7>
{
  public:
    DX7(double rate);
    ~DX7();
    void render(uint32_t from, uint32_t to);
    void post_process(uint32_t from, uint32_t to);

  protected:

  private:
    SynthUnit *synth_unit_;
    int16_t* outbuf16_;
    uint32_t bufsize_;
};

// GLOBALS
const uint8_t init_voice[] =
      { 99, 99, 99, 99, 99, 99, 99, 00, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
        99, 99, 99, 99, 99, 99, 99, 00, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
        99, 99, 99, 99, 99, 99, 99, 00, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
        99, 99, 99, 99, 99, 99, 99, 00, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
        99, 99, 99, 99, 99, 99, 99, 00, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
        99, 99, 99, 99, 99, 99, 99, 00, 39, 0, 0, 0, 0, 0, 0, 0, 99, 0, 1, 0, 7,
        99, 99, 99, 99, 50, 50, 50, 50, 0, 0, 1, 35, 0, 0, 0, 1, 0, 3, 24,
        73, 78, 73, 84, 32, 86, 79, 73, 67, 69 };

uint8_t data[156]; // program data

#endif  // DX7_H_INCLUDED
