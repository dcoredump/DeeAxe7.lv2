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

  set_params();

  TRACE("Bye");
}

DX7::~DX7()
{
  TRACE("Hi");

  delete synth_unit_;
  delete [] outbuf16_;

  TRACE("Bye");
}

void DX7::pre_process(uint32_t from, uint32_t to)
{
  set_params();
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

void DX7::set_params(void)
{
  // OP6
  synth_unit_->unpacked_patch_[0]=static_cast<char>(*p(p_op6_eg_rate_1));
  synth_unit_->unpacked_patch_[1]=static_cast<char>(*p(p_op6_eg_rate_2));
  synth_unit_->unpacked_patch_[2]=static_cast<char>(*p(p_op6_eg_rate_3));
  synth_unit_->unpacked_patch_[3]=static_cast<char>(*p(p_op6_eg_rate_4));
  synth_unit_->unpacked_patch_[4]=static_cast<char>(*p(p_op6_eg_level_1));
  synth_unit_->unpacked_patch_[5]=static_cast<char>(*p(p_op6_eg_level_2));
  synth_unit_->unpacked_patch_[6]=static_cast<char>(*p(p_op6_eg_level_3));
  synth_unit_->unpacked_patch_[7]=static_cast<char>(*p(p_op6_eg_level_4));
  synth_unit_->unpacked_patch_[8]=static_cast<char>(*p(p_op6_output_level));
  synth_unit_->unpacked_patch_[9]=static_cast<char>(*p(p_op6_mode));
  synth_unit_->unpacked_patch_[10]=static_cast<char>(*p(p_op6_f_coarse));
  synth_unit_->unpacked_patch_[11]=static_cast<char>(*p(p_op6_f_fine));
  synth_unit_->unpacked_patch_[12]=static_cast<char>(*p(p_op6_osc_detune));
  synth_unit_->unpacked_patch_[13]=static_cast<char>(*p(p_op6_break_point));
  synth_unit_->unpacked_patch_[14]=static_cast<char>(*p(p_op6_l_scale_depth));
  synth_unit_->unpacked_patch_[15]=static_cast<char>(*p(p_op6_r_scale_depth));
  synth_unit_->unpacked_patch_[16]=static_cast<char>(*p(p_op6_l_key_scale));
  synth_unit_->unpacked_patch_[17]=static_cast<char>(*p(p_op6_r_key_scale));
  synth_unit_->unpacked_patch_[18]=static_cast<char>(*p(p_op6_rate_scaling));
  synth_unit_->unpacked_patch_[19]=static_cast<char>(*p(p_op6_a_mod_sens_));
  synth_unit_->unpacked_patch_[20]=static_cast<char>(*p(p_op6_key_velocity));
  // OP5
  synth_unit_->unpacked_patch_[21]=static_cast<char>(*p(p_op5_eg_rate_1));
  synth_unit_->unpacked_patch_[22]=static_cast<char>(*p(p_op5_eg_rate_2));
  synth_unit_->unpacked_patch_[23]=static_cast<char>(*p(p_op5_eg_rate_3));
  synth_unit_->unpacked_patch_[24]=static_cast<char>(*p(p_op5_eg_rate_4));
  synth_unit_->unpacked_patch_[25]=static_cast<char>(*p(p_op5_eg_level_1));
  synth_unit_->unpacked_patch_[26]=static_cast<char>(*p(p_op5_eg_level_2));
  synth_unit_->unpacked_patch_[27]=static_cast<char>(*p(p_op5_eg_level_3));
  synth_unit_->unpacked_patch_[28]=static_cast<char>(*p(p_op5_eg_level_4));
  synth_unit_->unpacked_patch_[29]=static_cast<char>(*p(p_op5_output_level));
  synth_unit_->unpacked_patch_[30]=static_cast<char>(*p(p_op5_mode));
  synth_unit_->unpacked_patch_[31]=static_cast<char>(*p(p_op5_f_coarse));
  synth_unit_->unpacked_patch_[32]=static_cast<char>(*p(p_op5_f_fine));
  synth_unit_->unpacked_patch_[33]=static_cast<char>(*p(p_op5_osc_detune));
  synth_unit_->unpacked_patch_[34]=static_cast<char>(*p(p_op5_break_point));
  synth_unit_->unpacked_patch_[35]=static_cast<char>(*p(p_op5_l_scale_depth));
  synth_unit_->unpacked_patch_[36]=static_cast<char>(*p(p_op5_r_scale_depth));
  synth_unit_->unpacked_patch_[37]=static_cast<char>(*p(p_op5_l_key_scale));
  synth_unit_->unpacked_patch_[38]=static_cast<char>(*p(p_op5_r_key_scale));
  synth_unit_->unpacked_patch_[39]=static_cast<char>(*p(p_op5_rate_scaling));
  synth_unit_->unpacked_patch_[40]=static_cast<char>(*p(p_op5_a_mod_sens_));
  synth_unit_->unpacked_patch_[41]=static_cast<char>(*p(p_op5_key_velocity));
  // OP4
  synth_unit_->unpacked_patch_[42]=static_cast<char>(*p(p_op4_eg_rate_1));
  synth_unit_->unpacked_patch_[43]=static_cast<char>(*p(p_op4_eg_rate_2));
  synth_unit_->unpacked_patch_[44]=static_cast<char>(*p(p_op4_eg_rate_3));
  synth_unit_->unpacked_patch_[45]=static_cast<char>(*p(p_op4_eg_rate_4));
  synth_unit_->unpacked_patch_[46]=static_cast<char>(*p(p_op4_eg_level_1));
  synth_unit_->unpacked_patch_[47]=static_cast<char>(*p(p_op4_eg_level_2));
  synth_unit_->unpacked_patch_[48]=static_cast<char>(*p(p_op4_eg_level_3));
  synth_unit_->unpacked_patch_[49]=static_cast<char>(*p(p_op4_eg_level_4));
  synth_unit_->unpacked_patch_[50]=static_cast<char>(*p(p_op4_output_level));
  synth_unit_->unpacked_patch_[51]=static_cast<char>(*p(p_op4_mode));
  synth_unit_->unpacked_patch_[52]=static_cast<char>(*p(p_op4_f_coarse));
  synth_unit_->unpacked_patch_[53]=static_cast<char>(*p(p_op4_f_fine));
  synth_unit_->unpacked_patch_[54]=static_cast<char>(*p(p_op4_osc_detune));
  synth_unit_->unpacked_patch_[55]=static_cast<char>(*p(p_op4_break_point));
  synth_unit_->unpacked_patch_[56]=static_cast<char>(*p(p_op4_l_scale_depth));
  synth_unit_->unpacked_patch_[57]=static_cast<char>(*p(p_op4_r_scale_depth));
  synth_unit_->unpacked_patch_[58]=static_cast<char>(*p(p_op4_l_key_scale));
  synth_unit_->unpacked_patch_[59]=static_cast<char>(*p(p_op4_r_key_scale));
  synth_unit_->unpacked_patch_[60]=static_cast<char>(*p(p_op4_rate_scaling));
  synth_unit_->unpacked_patch_[61]=static_cast<char>(*p(p_op4_a_mod_sens_));
  synth_unit_->unpacked_patch_[62]=static_cast<char>(*p(p_op4_key_velocity));
  // OP3
  synth_unit_->unpacked_patch_[63]=static_cast<char>(*p(p_op3_eg_rate_1));
  synth_unit_->unpacked_patch_[64]=static_cast<char>(*p(p_op3_eg_rate_2));
  synth_unit_->unpacked_patch_[65]=static_cast<char>(*p(p_op3_eg_rate_3));
  synth_unit_->unpacked_patch_[66]=static_cast<char>(*p(p_op3_eg_rate_4));
  synth_unit_->unpacked_patch_[67]=static_cast<char>(*p(p_op3_eg_level_1));
  synth_unit_->unpacked_patch_[68]=static_cast<char>(*p(p_op3_eg_level_2));
  synth_unit_->unpacked_patch_[69]=static_cast<char>(*p(p_op3_eg_level_3));
  synth_unit_->unpacked_patch_[70]=static_cast<char>(*p(p_op3_eg_level_4));
  synth_unit_->unpacked_patch_[71]=static_cast<char>(*p(p_op3_output_level));
  synth_unit_->unpacked_patch_[72]=static_cast<char>(*p(p_op3_mode));
  synth_unit_->unpacked_patch_[73]=static_cast<char>(*p(p_op3_f_coarse));
  synth_unit_->unpacked_patch_[74]=static_cast<char>(*p(p_op3_f_fine));
  synth_unit_->unpacked_patch_[75]=static_cast<char>(*p(p_op3_osc_detune));
  synth_unit_->unpacked_patch_[76]=static_cast<char>(*p(p_op3_break_point));
  synth_unit_->unpacked_patch_[77]=static_cast<char>(*p(p_op3_l_scale_depth));
  synth_unit_->unpacked_patch_[78]=static_cast<char>(*p(p_op3_r_scale_depth));
  synth_unit_->unpacked_patch_[79]=static_cast<char>(*p(p_op3_l_key_scale));
  synth_unit_->unpacked_patch_[80]=static_cast<char>(*p(p_op3_r_key_scale));
  synth_unit_->unpacked_patch_[81]=static_cast<char>(*p(p_op3_rate_scaling));
  synth_unit_->unpacked_patch_[82]=static_cast<char>(*p(p_op3_a_mod_sens_));
  synth_unit_->unpacked_patch_[83]=static_cast<char>(*p(p_op3_key_velocity));
  // OP2
  synth_unit_->unpacked_patch_[84]=static_cast<char>(*p(p_op2_eg_rate_1));
  synth_unit_->unpacked_patch_[85]=static_cast<char>(*p(p_op2_eg_rate_2));
  synth_unit_->unpacked_patch_[86]=static_cast<char>(*p(p_op2_eg_rate_3));
  synth_unit_->unpacked_patch_[87]=static_cast<char>(*p(p_op2_eg_rate_4));
  synth_unit_->unpacked_patch_[88]=static_cast<char>(*p(p_op2_eg_level_1));
  synth_unit_->unpacked_patch_[89]=static_cast<char>(*p(p_op2_eg_level_2));
  synth_unit_->unpacked_patch_[90]=static_cast<char>(*p(p_op2_eg_level_3));
  synth_unit_->unpacked_patch_[91]=static_cast<char>(*p(p_op2_eg_level_4));
  synth_unit_->unpacked_patch_[92]=static_cast<char>(*p(p_op2_output_level));
  synth_unit_->unpacked_patch_[93]=static_cast<char>(*p(p_op2_mode));
  synth_unit_->unpacked_patch_[94]=static_cast<char>(*p(p_op2_f_coarse));
  synth_unit_->unpacked_patch_[95]=static_cast<char>(*p(p_op2_f_fine));
  synth_unit_->unpacked_patch_[96]=static_cast<char>(*p(p_op2_osc_detune));
  synth_unit_->unpacked_patch_[97]=static_cast<char>(*p(p_op2_break_point));
  synth_unit_->unpacked_patch_[98]=static_cast<char>(*p(p_op2_l_scale_depth));
  synth_unit_->unpacked_patch_[99]=static_cast<char>(*p(p_op2_r_scale_depth));
  synth_unit_->unpacked_patch_[100]=static_cast<char>(*p(p_op2_l_key_scale));
  synth_unit_->unpacked_patch_[101]=static_cast<char>(*p(p_op2_r_key_scale));
  synth_unit_->unpacked_patch_[102]=static_cast<char>(*p(p_op2_rate_scaling));
  synth_unit_->unpacked_patch_[103]=static_cast<char>(*p(p_op2_a_mod_sens_));
  synth_unit_->unpacked_patch_[104]=static_cast<char>(*p(p_op2_key_velocity));
  // OP1
  synth_unit_->unpacked_patch_[105]=static_cast<char>(*p(p_op1_eg_rate_1));
  synth_unit_->unpacked_patch_[106]=static_cast<char>(*p(p_op1_eg_rate_2));
  synth_unit_->unpacked_patch_[107]=static_cast<char>(*p(p_op1_eg_rate_3));
  synth_unit_->unpacked_patch_[108]=static_cast<char>(*p(p_op1_eg_rate_4));
  synth_unit_->unpacked_patch_[109]=static_cast<char>(*p(p_op1_eg_level_1));
  synth_unit_->unpacked_patch_[110]=static_cast<char>(*p(p_op1_eg_level_2));
  synth_unit_->unpacked_patch_[111]=static_cast<char>(*p(p_op1_eg_level_3));
  synth_unit_->unpacked_patch_[112]=static_cast<char>(*p(p_op1_eg_level_4));
  synth_unit_->unpacked_patch_[113]=static_cast<char>(*p(p_op1_output_level));
  synth_unit_->unpacked_patch_[114]=static_cast<char>(*p(p_op1_mode));
  synth_unit_->unpacked_patch_[115]=static_cast<char>(*p(p_op1_f_coarse));
  synth_unit_->unpacked_patch_[116]=static_cast<char>(*p(p_op1_f_fine));
  synth_unit_->unpacked_patch_[117]=static_cast<char>(*p(p_op1_osc_detune));
  synth_unit_->unpacked_patch_[118]=static_cast<char>(*p(p_op1_break_point));
  synth_unit_->unpacked_patch_[119]=static_cast<char>(*p(p_op1_l_scale_depth));
  synth_unit_->unpacked_patch_[120]=static_cast<char>(*p(p_op1_r_scale_depth));
  synth_unit_->unpacked_patch_[121]=static_cast<char>(*p(p_op1_l_key_scale));
  synth_unit_->unpacked_patch_[122]=static_cast<char>(*p(p_op1_r_key_scale));
  synth_unit_->unpacked_patch_[123]=static_cast<char>(*p(p_op1_rate_scaling));
  synth_unit_->unpacked_patch_[124]=static_cast<char>(*p(p_op1_a_mod_sens_));
  synth_unit_->unpacked_patch_[125]=static_cast<char>(*p(p_op1_key_velocity));
  // Global for all OPs
  synth_unit_->unpacked_patch_[126]=static_cast<char>(*p(p_pitch_eg_rate_1));
  synth_unit_->unpacked_patch_[127]=static_cast<char>(*p(p_pitch_eg_rate_2));
  synth_unit_->unpacked_patch_[128]=static_cast<char>(*p(p_pitch_eg_rate_3));
  synth_unit_->unpacked_patch_[129]=static_cast<char>(*p(p_pitch_eg_rate_4));
  synth_unit_->unpacked_patch_[130]=static_cast<char>(*p(p_pitch_eg_level_1));
  synth_unit_->unpacked_patch_[131]=static_cast<char>(*p(p_pitch_eg_level_2));
  synth_unit_->unpacked_patch_[132]=static_cast<char>(*p(p_pitch_eg_level_3));
  synth_unit_->unpacked_patch_[133]=static_cast<char>(*p(p_pitch_eg_level_4));
  synth_unit_->unpacked_patch_[134]=static_cast<char>(*p(p_algorithm));
  synth_unit_->unpacked_patch_[135]=static_cast<char>(*p(p_feedback));
  synth_unit_->unpacked_patch_[136]=static_cast<char>(*p(p_osc_key_sync));
  synth_unit_->unpacked_patch_[137]=static_cast<char>(*p(p_lfo_speed));
  synth_unit_->unpacked_patch_[138]=static_cast<char>(*p(p_lfo_delay));
  synth_unit_->unpacked_patch_[139]=static_cast<char>(*p(p_lfo_pm_depth));
  synth_unit_->unpacked_patch_[140]=static_cast<char>(*p(p_lfo_am_depth));
  synth_unit_->unpacked_patch_[141]=static_cast<char>(*p(p_lfo_key_sync));
  synth_unit_->unpacked_patch_[142]=static_cast<char>(*p(p_lfo_wave));
  synth_unit_->unpacked_patch_[143]=static_cast<char>(*p(p_p_mode_sens_));
  synth_unit_->unpacked_patch_[144]=static_cast<char>(*p(p_middle_c));
  // 10 bytes (145-154) are the name of the patch
  synth_unit_->unpacked_patch_[155]=0x3f; // operator on/off
}

// override the run() method
void DX7::run (uint32_t sample_count)
{
    const LV2_Atom_Sequence* seq = p<LV2_Atom_Sequence> (p_lv2_events_in);
    float* output = p(p_lv2_audio_out_1);
    uint32_t last_frame = 0, num_this_time = 0;

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
         output[j] = static_cast<float> (outbuf16_[i]) * scaler;
      
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
         output[j] = static_cast<float> (outbuf16_[i]) * scaler;
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

  TRACE("Bye");
}

void DX7_Voice::off(unsigned char velocity)
{
  TRACE("Hi");

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
