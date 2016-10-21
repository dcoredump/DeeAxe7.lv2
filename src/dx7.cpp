// from: http://ll-plugins.nongnu.org/lv2pftci/#A_synth

#include <lvtk/synth.hpp>
#include "dx7.peg"
#include "dx7.h"
#include "freqlut.h"
#include "exp2.h"
#include "sin.h"
#include "pitchenv.h"
#include "msfa/synth.h"
#include "msfa/ringbuffer.h"

DX7::DX7(double rate) : lvtk::Synth<DX7_Voice, DX7>(p_n_ports, p_lv2_events_in)
{
  TRACE("Hi");

  Freqlut::init(rate);
  Exp2::init();
  Sin::init();
  Lfo::init(rate);
  PitchEnv::init(rate);
        
  bufsize_=256;
  outbuf16_=new int16_t[bufsize_];
  synth_unit_=new SynthUnit(&ring_buffer_);

  add_voices(new DX7_Voice(rate));
  add_audio_outputs(p_lv2_audio_out_1);

  TRACE("Bye");
}

DX7::~DX7()
{
  TRACE("Hi");

  delete synth_unit_;
  delete [] outbuf16_;

  TRACE("Bye");
}

void DX7::render(uint32_t from, uint32_t to)
{
  uint32_t i;

  TRACE("Hi");

  synth_unit_->GetSamples(to-from, outbuf16_);

  for (i = from; i < to; ++i)
    p(p_lv2_audio_out_1)[i]+=outbuf16_[i] * scaler;

  TRACE("Bye");
}

void DX7::post_process(uint32_t from, uint32_t to)
{
  uint32_t i;

  TRACE("Hi");

  for (i = from; i < to; ++i)
  {
    p(p_lv2_audio_out_1)[i] *= *p(p_output);
  }

  TRACE("Bye");
}

// override the run() method
void DX7::run(uint32_t sample_count)
{
    //D* synth = static_cast<D*>(this);
    class DX7* synth = this;

    // Zero output buffers so voices can add to them
    for (unsigned i = 0; i < m_audio_ports.size(); ++i)
        std::memset(p(m_audio_ports[i]), 0, sizeof(float) * sample_count);


    // Make the port buffers available to the voices
    for (unsigned i = 0; i < m_voices.size(); ++i)
        m_voices[i]->set_port_buffers(Parent::m_ports);

    const LV2_Atom_Sequence* seq = p<LV2_Atom_Sequence> (m_midi_input);
    uint32_t last_frame = 0;

    for (LV2_Atom_Event* ev = lv2_atom_sequence_begin (&seq->body);
         !lv2_atom_sequence_is_end(&seq->body, seq->atom.size, ev);
         ev = lv2_atom_sequence_next (ev))
    {
       synth->pre_process (last_frame, ev->time.frames);
       for (uint32_t i = 0; i < m_voices.size(); ++i)
          m_voices[i]->render (last_frame, ev->time.frames);
       synth->post_process (last_frame, ev->time.frames);

       if (ev->body.type == m_midi_type)
          synth->handle_midi (ev->body.size, (uint8_t*) LV2_ATOM_BODY (&ev->body));
       else
          synth->handle_atom_event (ev);

       last_frame = ev->time.frames;
    }

    if (last_frame < sample_count)
    {
       synth->pre_process (last_frame, sample_count);
       //for (uint32_t i = 0; i < m_voices.size(); ++i)
       //   m_voices[i]->render (last_frame, sample_count);
       synth->render (last_frame, sample_count);
       synth->post_process (last_frame, sample_count);
    }
}

//==============================================================================

DX7_Voice::DX7_Voice(double rate) : m_key(lvtk::INVALID_KEY), m_rate(rate)
{
  TRACE("Hi");

  TRACE("Bye");
}

DX7_Voice::~DX7_Voice()
{
  TRACE("Hi");

  TRACE("Bye");
}

void DX7_Voice::on(unsigned char key, unsigned char velocity)
{
  TRACE("Hi");

  m_key = key;

  add_midi(0x91, key, velocity);

  TRACE("Bye");
}

void DX7_Voice::off(unsigned char velocity)
{
  TRACE("Hi");

  add_midi(0x81, m_key, velocity);
  m_key = lvtk::INVALID_KEY;

  TRACE("Bye");
}

unsigned char DX7_Voice::get_key(void) const
{
  return m_key;
}

void DX7_Voice::render(uint32_t from, uint32_t to)
{
  uint32_t i;

  TRACE("Hi");

  TRACE("Bye");
}

/* void DX7_Voice::render(uint32_t from, uint32_t to)
{
  uint32_t i;

  TRACE("Hi");

  synth_unit_->GetSamples(to-from, outbuf16_);

  for (i = from; i < to; ++i)
    p(p_lv2_audio_out_1)[i]+=outbuf16_[i] * scaler);

  TRACE("Bye");
} */

void DX7_Voice::add_midi(uint8_t msg1, uint8_t msg2, uint8_t msg3)
{
  uint8_t msg[3] = { msg1, msg2, msg3 };
  ring_buffer_.Write(msg, 3);
}

#ifdef DEBUG
void _trace(const char *source, const char *fmt, ...) {
    char output[4096];
    va_list argptr;
    va_start(argptr, fmt);
    vsnprintf(output, 4095, fmt, argptr);
    va_end(argptr);
    printf("%s: %s\n",source,output);
}
#endif

static int _ = DX7::register_class(p_uri);
