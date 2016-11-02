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

#ifndef DEEAXE7_H_INCLUDED
#define DEEAXE7_H_INCLUDED

#include "msfa/synth.h"
#include "msfa/synth_unit.h"
#include "msfa/ringbuffer.h"

void _trace(const char *source, const char *fmt, ...);

#define DeeAxe7_ID "0.0.1"

#ifdef DEBUG
    #define DeeAxe7_VERSION DeeAxe7_ID " DEBUG"
    #define TRACE(fmt, ...) _trace(__PRETTY_FUNCTION__,fmt,##__VA_ARGS__)
#else
    #define DeeAxe7_VERSION DeeAxe7_ID
    #define TRACE(fmt, ...)
#endif

//==============================================================================
// Global vars
RingBuffer ring_buffer_;
//static const float scaler = 0.00003051757813;
static const float scaler = 0.00003;

//==============================================================================

class DeeAxe7_Voice : public lvtk::Voice
{
  public:
    DeeAxe7_Voice(double rate);
    ~DeeAxe7_Voice();
    void on(unsigned char key, unsigned char velocity);
    void off(unsigned char velocity);
    unsigned char get_key(void) const;

  protected:
    unsigned char m_key;
    double m_rate;

  private:
};

//==============================================================================

class DeeAxe7 : public lvtk::Synth<DeeAxe7_Voice, DeeAxe7>
{
  public:
    DeeAxe7(double rate);
    ~DeeAxe7();
    void run(uint32_t sample_count);

  protected:
    void set_params(void);

  private:
    SynthUnit* synth_unit_;
    int16_t* outbuf16_;
    uint32_t bufsize_;
};

#endif  // DEEAXE7_H_INCLUDED
