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

DX7::DX7(double rate) : lvtk::Synth<DX7_Voice, DX7>(p_n_ports, p_midi_in)
{
  TRACE("Hi");

  bufsize_=256;
  outbuf16_=new int16_t[bufsize_];
  synth_unit_=new SynthUnit(&ring_buffer_);

  synth_unit_->Init(rate);
  add_voices(new DX7_Voice(rate));
  add_audio_outputs(p_audio_out);

  //set_params();
  //synth_unit_->lfo_.reset(synth_unit_->unpacked_patch_ + 137);

  TRACE("Bye");
}

DX7::~DX7()
{
  TRACE("Hi");

  delete synth_unit_;
  delete [] outbuf16_;

  TRACE("Bye");
}

void DX7::set_params(void)
{
  TRACE("Hi");

  // OP6
  synth_unit_->onParam(0,static_cast<char>(*p(p_op6_eg_rate_1)));
  synth_unit_->onParam(1,static_cast<char>(*p(p_op6_eg_rate_2)));
  synth_unit_->onParam(2,static_cast<char>(*p(p_op6_eg_rate_3)));
  synth_unit_->onParam(3,static_cast<char>(*p(p_op6_eg_rate_4)));
  synth_unit_->onParam(4,static_cast<char>(*p(p_op6_eg_level_1)));
  synth_unit_->onParam(5,static_cast<char>(*p(p_op6_eg_level_2)));
  synth_unit_->onParam(6,static_cast<char>(*p(p_op6_eg_level_3)));
  synth_unit_->onParam(7,static_cast<char>(*p(p_op6_eg_level_4)));
  synth_unit_->onParam(8,static_cast<char>(*p(p_op6_output_level)));
  synth_unit_->onParam(9,static_cast<char>(*p(p_op6_mode)));
  synth_unit_->onParam(10,static_cast<char>(*p(p_op6_f_coarse)));
  synth_unit_->onParam(11,static_cast<char>(*p(p_op6_f_fine)));
  synth_unit_->onParam(12,static_cast<char>(*p(p_op6_osc_detune))+7);
  synth_unit_->onParam(13,static_cast<char>(*p(p_op6_break_point)));
  synth_unit_->onParam(14,static_cast<char>(*p(p_op6_l_scale_depth)));
  synth_unit_->onParam(15,static_cast<char>(*p(p_op6_r_scale_depth)));
  synth_unit_->onParam(16,static_cast<char>(*p(p_op6_l_key_curve)));
  synth_unit_->onParam(17,static_cast<char>(*p(p_op6_r_key_curve)));
  synth_unit_->onParam(18,static_cast<char>(*p(p_op6_rate_scaling)));
  synth_unit_->onParam(19,static_cast<char>(*p(p_op6_a_mod_sens)));
  synth_unit_->onParam(20,static_cast<char>(*p(p_op6_key_velocity)));
  // OP5
  synth_unit_->onParam(0,static_cast<char>(*p(p_op5_eg_rate_1)));
  synth_unit_->onParam(1,static_cast<char>(*p(p_op5_eg_rate_2)));
  synth_unit_->onParam(2,static_cast<char>(*p(p_op5_eg_rate_3)));
  synth_unit_->onParam(3,static_cast<char>(*p(p_op5_eg_rate_4)));
  synth_unit_->onParam(4,static_cast<char>(*p(p_op5_eg_level_1)));
  synth_unit_->onParam(5,static_cast<char>(*p(p_op5_eg_level_2)));
  synth_unit_->onParam(6,static_cast<char>(*p(p_op5_eg_level_3)));
  synth_unit_->onParam(7,static_cast<char>(*p(p_op5_eg_level_4)));
  synth_unit_->onParam(8,static_cast<char>(*p(p_op5_output_level)));
  synth_unit_->onParam(9,static_cast<char>(*p(p_op5_mode)));
  synth_unit_->onParam(10,static_cast<char>(*p(p_op5_f_coarse)));
  synth_unit_->onParam(11,static_cast<char>(*p(p_op5_f_fine)));
  synth_unit_->onParam(12,static_cast<char>(*p(p_op5_osc_detune))+7);
  synth_unit_->onParam(13,static_cast<char>(*p(p_op5_break_point)));
  synth_unit_->onParam(14,static_cast<char>(*p(p_op5_l_scale_depth)));
  synth_unit_->onParam(15,static_cast<char>(*p(p_op5_r_scale_depth)));
  synth_unit_->onParam(16,static_cast<char>(*p(p_op5_l_key_curve)));
  synth_unit_->onParam(17,static_cast<char>(*p(p_op5_r_key_curve)));
  synth_unit_->onParam(18,static_cast<char>(*p(p_op5_rate_scaling)));
  synth_unit_->onParam(19,static_cast<char>(*p(p_op5_a_mod_sens)));
  synth_unit_->onParam(20,static_cast<char>(*p(p_op5_key_velocity)));
  // OP4
  synth_unit_->onParam(42,static_cast<char>(*p(p_op4_eg_rate_1)));
  synth_unit_->onParam(43,static_cast<char>(*p(p_op4_eg_rate_2)));
  synth_unit_->onParam(44,static_cast<char>(*p(p_op4_eg_rate_3)));
  synth_unit_->onParam(45,static_cast<char>(*p(p_op4_eg_rate_4)));
  synth_unit_->onParam(46,static_cast<char>(*p(p_op4_eg_level_1)));
  synth_unit_->onParam(47,static_cast<char>(*p(p_op4_eg_level_2)));
  synth_unit_->onParam(48,static_cast<char>(*p(p_op4_eg_level_3)));
  synth_unit_->onParam(49,static_cast<char>(*p(p_op4_eg_level_4)));
  synth_unit_->onParam(50,static_cast<char>(*p(p_op4_output_level)));
  synth_unit_->onParam(51,static_cast<char>(*p(p_op4_mode)));
  synth_unit_->onParam(52,static_cast<char>(*p(p_op4_f_coarse)));
  synth_unit_->onParam(53,static_cast<char>(*p(p_op4_f_fine)));
  synth_unit_->onParam(54,static_cast<char>(*p(p_op4_osc_detune))+7);
  synth_unit_->onParam(55,static_cast<char>(*p(p_op4_break_point)));
  synth_unit_->onParam(56,static_cast<char>(*p(p_op4_l_scale_depth)));
  synth_unit_->onParam(57,static_cast<char>(*p(p_op4_r_scale_depth)));
  synth_unit_->onParam(58,static_cast<char>(*p(p_op4_l_key_curve)));
  synth_unit_->onParam(59,static_cast<char>(*p(p_op4_r_key_curve)));
  synth_unit_->onParam(60,static_cast<char>(*p(p_op4_rate_scaling)));
  synth_unit_->onParam(61,static_cast<char>(*p(p_op4_a_mod_sens)));
  synth_unit_->onParam(62,static_cast<char>(*p(p_op4_key_velocity)));
  // OP3
  synth_unit_->onParam(63,static_cast<char>(*p(p_op3_eg_rate_1)));
  synth_unit_->onParam(64,static_cast<char>(*p(p_op3_eg_rate_2)));
  synth_unit_->onParam(65,static_cast<char>(*p(p_op3_eg_rate_3)));
  synth_unit_->onParam(66,static_cast<char>(*p(p_op3_eg_rate_4)));
  synth_unit_->onParam(67,static_cast<char>(*p(p_op3_eg_level_1)));
  synth_unit_->onParam(68,static_cast<char>(*p(p_op3_eg_level_2)));
  synth_unit_->onParam(69,static_cast<char>(*p(p_op3_eg_level_3)));
  synth_unit_->onParam(70,static_cast<char>(*p(p_op3_eg_level_4)));
  synth_unit_->onParam(71,static_cast<char>(*p(p_op3_output_level)));
  synth_unit_->onParam(72,static_cast<char>(*p(p_op3_mode)));
  synth_unit_->onParam(73,static_cast<char>(*p(p_op3_f_coarse)));
  synth_unit_->onParam(74,static_cast<char>(*p(p_op3_f_fine)));
  synth_unit_->onParam(75,static_cast<char>(*p(p_op3_osc_detune))+7);
  synth_unit_->onParam(76,static_cast<char>(*p(p_op3_break_point)));
  synth_unit_->onParam(77,static_cast<char>(*p(p_op3_l_scale_depth)));
  synth_unit_->onParam(78,static_cast<char>(*p(p_op3_r_scale_depth)));
  synth_unit_->onParam(79,static_cast<char>(*p(p_op3_l_key_curve)));
  synth_unit_->onParam(80,static_cast<char>(*p(p_op3_r_key_curve)));
  synth_unit_->onParam(81,static_cast<char>(*p(p_op3_rate_scaling)));
  synth_unit_->onParam(82,static_cast<char>(*p(p_op3_a_mod_sens)));
  synth_unit_->onParam(83,static_cast<char>(*p(p_op3_key_velocity)));
  // OP2
  synth_unit_->onParam(84,static_cast<char>(*p(p_op2_eg_rate_1)));
  synth_unit_->onParam(85,static_cast<char>(*p(p_op2_eg_rate_2)));
  synth_unit_->onParam(86,static_cast<char>(*p(p_op2_eg_rate_3)));
  synth_unit_->onParam(87,static_cast<char>(*p(p_op2_eg_rate_4)));
  synth_unit_->onParam(88,static_cast<char>(*p(p_op2_eg_level_1)));
  synth_unit_->onParam(89,static_cast<char>(*p(p_op2_eg_level_2)));
  synth_unit_->onParam(90,static_cast<char>(*p(p_op2_eg_level_3)));
  synth_unit_->onParam(91,static_cast<char>(*p(p_op2_eg_level_4)));
  synth_unit_->onParam(92,static_cast<char>(*p(p_op2_output_level)));
  synth_unit_->onParam(93,static_cast<char>(*p(p_op2_mode)));
  synth_unit_->onParam(94,static_cast<char>(*p(p_op2_f_coarse)));
  synth_unit_->onParam(95,static_cast<char>(*p(p_op2_f_fine)));
  synth_unit_->onParam(96,static_cast<char>(*p(p_op2_osc_detune))+7);
  synth_unit_->onParam(97,static_cast<char>(*p(p_op2_break_point)));
  synth_unit_->onParam(98,static_cast<char>(*p(p_op2_l_scale_depth)));
  synth_unit_->onParam(99,static_cast<char>(*p(p_op2_r_scale_depth)));
  synth_unit_->onParam(100,static_cast<char>(*p(p_op2_l_key_curve)));
  synth_unit_->onParam(101,static_cast<char>(*p(p_op2_r_key_curve)));
  synth_unit_->onParam(102,static_cast<char>(*p(p_op2_rate_scaling)));
  synth_unit_->onParam(103,static_cast<char>(*p(p_op2_a_mod_sens)));
  synth_unit_->onParam(104,static_cast<char>(*p(p_op2_key_velocity)));
  // OP1
  synth_unit_->onParam(105,static_cast<char>(*p(p_op1_eg_rate_1)));
  synth_unit_->onParam(106,static_cast<char>(*p(p_op1_eg_rate_2)));
  synth_unit_->onParam(107,static_cast<char>(*p(p_op1_eg_rate_3)));
  synth_unit_->onParam(108,static_cast<char>(*p(p_op1_eg_rate_4)));
  synth_unit_->onParam(109,static_cast<char>(*p(p_op1_eg_level_1)));
  synth_unit_->onParam(110,static_cast<char>(*p(p_op1_eg_level_2)));
  synth_unit_->onParam(111,static_cast<char>(*p(p_op1_eg_level_3)));
  synth_unit_->onParam(112,static_cast<char>(*p(p_op1_eg_level_4)));
  synth_unit_->onParam(113,static_cast<char>(*p(p_op1_output_level)));
  synth_unit_->onParam(114,static_cast<char>(*p(p_op1_mode)));
  synth_unit_->onParam(115,static_cast<char>(*p(p_op1_f_coarse)));
  synth_unit_->onParam(116,static_cast<char>(*p(p_op1_f_fine)));
  synth_unit_->onParam(117,static_cast<char>(*p(p_op1_osc_detune))+7);
  synth_unit_->onParam(118,static_cast<char>(*p(p_op1_break_point)));
  synth_unit_->onParam(119,static_cast<char>(*p(p_op1_l_scale_depth)));
  synth_unit_->onParam(120,static_cast<char>(*p(p_op1_r_scale_depth)));
  synth_unit_->onParam(121,static_cast<char>(*p(p_op1_l_key_curve)));
  synth_unit_->onParam(122,static_cast<char>(*p(p_op1_r_key_curve)));
  synth_unit_->onParam(123,static_cast<char>(*p(p_op1_rate_scaling)));
  synth_unit_->onParam(124,static_cast<char>(*p(p_op1_a_mod_sens)));
  synth_unit_->onParam(125,static_cast<char>(*p(p_op1_key_velocity)));
  // Global for all OPs
  synth_unit_->onParam(126,static_cast<char>(*p(p_pitch_eg_rate_1)));
  synth_unit_->onParam(127,static_cast<char>(*p(p_pitch_eg_rate_2)));
  synth_unit_->onParam(128,static_cast<char>(*p(p_pitch_eg_rate_3)));
  synth_unit_->onParam(129,static_cast<char>(*p(p_pitch_eg_rate_4)));
  synth_unit_->onParam(130,static_cast<char>(*p(p_pitch_eg_level_1)));
  synth_unit_->onParam(131,static_cast<char>(*p(p_pitch_eg_level_2)));
  synth_unit_->onParam(132,static_cast<char>(*p(p_pitch_eg_level_3)));
  synth_unit_->onParam(133,static_cast<char>(*p(p_pitch_eg_level_4)));
  synth_unit_->onParam(134,static_cast<char>(*p(p_algorithm))-1);
  synth_unit_->onParam(135,static_cast<char>(*p(p_feedback)));
  synth_unit_->onParam(136,static_cast<char>(*p(p_osc_key_sync)));
  synth_unit_->onParam(137,static_cast<char>(*p(p_lfo_speed)));
  synth_unit_->onParam(138,static_cast<char>(*p(p_lfo_delay)));
  synth_unit_->onParam(139,static_cast<char>(*p(p_lfo_pm_depth)));
  synth_unit_->onParam(140,static_cast<char>(*p(p_lfo_am_depth)));
  synth_unit_->onParam(141,static_cast<char>(*p(p_lfo_key_sync)));
  synth_unit_->onParam(142,static_cast<char>(*p(p_lfo_wave)));
  synth_unit_->onParam(143,static_cast<char>(*p(p_p_mode_sens)));
  synth_unit_->onParam(144,static_cast<char>(*p(p_middle_c)));
  // 10 bytes (145-154) are the name of the patch
  synth_unit_->onParam(155,0x3f); // operator on/off => All OPs on

  TRACE("Bye");
}

// override the run() method
void DX7::run (uint32_t sample_count)
{
    const LV2_Atom_Sequence* seq = p<LV2_Atom_Sequence> (p_midi_in);
    float* output = p(p_audio_out);
    uint32_t last_frame = 0, num_this_time = 0;
    float level=*p(p_output)*scaler;

    set_params(); // pre_process: copy actual voice params

    for (LV2_Atom_Event* ev = lv2_atom_sequence_begin (&seq->body);
         !lv2_atom_sequence_is_end(&seq->body, seq->atom.size, ev);
         ev = lv2_atom_sequence_next (ev))
    {
       num_this_time = ev->time.frames - last_frame;

       // If it's midi, send it to the engine
       if (ev->body.type == m_midi_type)
          ring_buffer_.Write ((uint8_t*) LV2_ATOM_BODY (&ev->body), ev->body.size);
      
       // render audio from the last frame until the timestamp of this event
       synth_unit_->GetSamples (num_this_time, outbuf16_);
      
       // i is the index of the engine's buf, which always starts at 0 (i think)
       // j is the index of the plugin's float output buffer which will be the timestamp
       // of the last processed atom event.
       for (uint32_t i = 0, j = last_frame; i < num_this_time; ++i, ++j)
         output[j] = (static_cast<float> (outbuf16_[i])) * level;

       last_frame = ev->time.frames;
    }

    // render remaining samples if any left
    if (last_frame < sample_count)
    {
       // do the same thing as above except from last frame until the end of
       // the processing cycles last sample. at this point, all events have
       // already been handled.

       num_this_time = sample_count - last_frame;
       synth_unit_->GetSamples (num_this_time, outbuf16_);
       for (uint32_t i = 0, j = last_frame; i < num_this_time; ++i, ++j)
         output[j] = (static_cast<float> (outbuf16_[i])) * level;
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

  TRACE("Bye");
}

void DX7_Voice::off(unsigned char velocity)
{
  TRACE("Hi");

  m_key = lvtk::INVALID_KEY;

  TRACE("Bye");
}

unsigned char DX7_Voice::get_key(void) const
{
  return m_key;
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
