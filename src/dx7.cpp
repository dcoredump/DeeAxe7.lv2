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

  Freqlut::init(rate);
  Exp2::init();
  Sin::init();
  Lfo::init(rate);
  PitchEnv::init(rate);
        
  bufsize_=256;
  outbuf16_=new int16_t[bufsize_];
  synth_unit_=new SynthUnit(&ring_buffer_);

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
  synth_unit_->unpacked_patch_[0]=*p(p_op6_eg_rate_1);
  synth_unit_->unpacked_patch_[1]=*p(p_op6_eg_rate_2);
  synth_unit_->unpacked_patch_[2]=*p(p_op6_eg_rate_3);
  synth_unit_->unpacked_patch_[3]=*p(p_op6_eg_rate_4);
  synth_unit_->unpacked_patch_[4]=*p(p_op6_eg_level_1);
  synth_unit_->unpacked_patch_[5]=*p(p_op6_eg_level_2);
  synth_unit_->unpacked_patch_[6]=*p(p_op6_eg_level_3);
  synth_unit_->unpacked_patch_[7]=*p(p_op6_eg_level_4);
  synth_unit_->unpacked_patch_[8]=*p(p_op6_output_level);
  synth_unit_->unpacked_patch_[9]=*p(p_op6_mode);
  synth_unit_->unpacked_patch_[10]=*p(p_op6_f_coarse);
  synth_unit_->unpacked_patch_[11]=*p(p_op6_f_fine);
  synth_unit_->unpacked_patch_[12]=*p(p_op6_osc_detune);
  synth_unit_->unpacked_patch_[13]=*p(p_op6_break_point);
  synth_unit_->unpacked_patch_[14]=*p(p_op6_l_scale_depth);
  synth_unit_->unpacked_patch_[15]=*p(p_op6_r_scale_depth);
  synth_unit_->unpacked_patch_[16]=*p(p_op6_l_key_scale);
  synth_unit_->unpacked_patch_[17]=*p(p_op6_r_key_scale);
  synth_unit_->unpacked_patch_[18]=*p(p_op6_rate_scaling);
  synth_unit_->unpacked_patch_[19]=*p(p_op6_a_mod_sens_);
  synth_unit_->unpacked_patch_[20]=*p(p_op6_key_velocity);
  // OP5
  synth_unit_->unpacked_patch_[21]=*p(p_op5_eg_rate_1);
  synth_unit_->unpacked_patch_[22]=*p(p_op5_eg_rate_2);
  synth_unit_->unpacked_patch_[23]=*p(p_op5_eg_rate_3);
  synth_unit_->unpacked_patch_[24]=*p(p_op5_eg_rate_4);
  synth_unit_->unpacked_patch_[25]=*p(p_op5_eg_level_1);
  synth_unit_->unpacked_patch_[26]=*p(p_op5_eg_level_2);
  synth_unit_->unpacked_patch_[27]=*p(p_op5_eg_level_3);
  synth_unit_->unpacked_patch_[28]=*p(p_op5_eg_level_4);
  synth_unit_->unpacked_patch_[29]=*p(p_op5_output_level);
  synth_unit_->unpacked_patch_[30]=*p(p_op5_mode);
  synth_unit_->unpacked_patch_[31]=*p(p_op5_f_coarse);
  synth_unit_->unpacked_patch_[32]=*p(p_op5_f_fine);
  synth_unit_->unpacked_patch_[33]=*p(p_op5_osc_detune);
  synth_unit_->unpacked_patch_[34]=*p(p_op5_break_point);
  synth_unit_->unpacked_patch_[35]=*p(p_op5_l_scale_depth);
  synth_unit_->unpacked_patch_[36]=*p(p_op5_r_scale_depth);
  synth_unit_->unpacked_patch_[37]=*p(p_op5_l_key_scale);
  synth_unit_->unpacked_patch_[38]=*p(p_op5_r_key_scale);
  synth_unit_->unpacked_patch_[39]=*p(p_op5_rate_scaling);
  synth_unit_->unpacked_patch_[40]=*p(p_op5_a_mod_sens_);
  synth_unit_->unpacked_patch_[41]=*p(p_op5_key_velocity);
  // OP4
  synth_unit_->unpacked_patch_[42]=*p(p_op4_eg_rate_1);
  synth_unit_->unpacked_patch_[43]=*p(p_op4_eg_rate_2);
  synth_unit_->unpacked_patch_[44]=*p(p_op4_eg_rate_3);
  synth_unit_->unpacked_patch_[45]=*p(p_op4_eg_rate_4);
  synth_unit_->unpacked_patch_[46]=*p(p_op4_eg_level_1);
  synth_unit_->unpacked_patch_[47]=*p(p_op4_eg_level_2);
  synth_unit_->unpacked_patch_[48]=*p(p_op4_eg_level_3);
  synth_unit_->unpacked_patch_[49]=*p(p_op4_eg_level_4);
  synth_unit_->unpacked_patch_[50]=*p(p_op4_output_level);
  synth_unit_->unpacked_patch_[51]=*p(p_op4_mode);
  synth_unit_->unpacked_patch_[52]=*p(p_op4_f_coarse);
  synth_unit_->unpacked_patch_[53]=*p(p_op4_f_fine);
  synth_unit_->unpacked_patch_[54]=*p(p_op4_osc_detune);
  synth_unit_->unpacked_patch_[55]=*p(p_op4_break_point);
  synth_unit_->unpacked_patch_[56]=*p(p_op4_l_scale_depth);
  synth_unit_->unpacked_patch_[57]=*p(p_op4_r_scale_depth);
  synth_unit_->unpacked_patch_[58]=*p(p_op4_l_key_scale);
  synth_unit_->unpacked_patch_[59]=*p(p_op4_r_key_scale);
  synth_unit_->unpacked_patch_[60]=*p(p_op4_rate_scaling);
  synth_unit_->unpacked_patch_[61]=*p(p_op4_a_mod_sens_);
  synth_unit_->unpacked_patch_[62]=*p(p_op4_key_velocity);
  // OP3
  synth_unit_->unpacked_patch_[63]=*p(p_op3_eg_rate_1);
  synth_unit_->unpacked_patch_[64]=*p(p_op3_eg_rate_2);
  synth_unit_->unpacked_patch_[65]=*p(p_op3_eg_rate_3);
  synth_unit_->unpacked_patch_[66]=*p(p_op3_eg_rate_4);
  synth_unit_->unpacked_patch_[67]=*p(p_op3_eg_level_1);
  synth_unit_->unpacked_patch_[68]=*p(p_op3_eg_level_2);
  synth_unit_->unpacked_patch_[69]=*p(p_op3_eg_level_3);
  synth_unit_->unpacked_patch_[70]=*p(p_op3_eg_level_4);
  synth_unit_->unpacked_patch_[71]=*p(p_op3_output_level);
  synth_unit_->unpacked_patch_[72]=*p(p_op3_mode);
  synth_unit_->unpacked_patch_[73]=*p(p_op3_f_coarse);
  synth_unit_->unpacked_patch_[74]=*p(p_op3_f_fine);
  synth_unit_->unpacked_patch_[75]=*p(p_op3_osc_detune);
  synth_unit_->unpacked_patch_[76]=*p(p_op3_break_point);
  synth_unit_->unpacked_patch_[77]=*p(p_op3_l_scale_depth);
  synth_unit_->unpacked_patch_[78]=*p(p_op3_r_scale_depth);
  synth_unit_->unpacked_patch_[79]=*p(p_op3_l_key_scale);
  synth_unit_->unpacked_patch_[80]=*p(p_op3_r_key_scale);
  synth_unit_->unpacked_patch_[81]=*p(p_op3_rate_scaling);
  synth_unit_->unpacked_patch_[82]=*p(p_op3_a_mod_sens_);
  synth_unit_->unpacked_patch_[83]=*p(p_op3_key_velocity);
  // OP2
  synth_unit_->unpacked_patch_[84]=*p(p_op2_eg_rate_1);
  synth_unit_->unpacked_patch_[85]=*p(p_op2_eg_rate_2);
  synth_unit_->unpacked_patch_[86]=*p(p_op2_eg_rate_3);
  synth_unit_->unpacked_patch_[87]=*p(p_op2_eg_rate_4);
  synth_unit_->unpacked_patch_[88]=*p(p_op2_eg_level_1);
  synth_unit_->unpacked_patch_[89]=*p(p_op2_eg_level_2);
  synth_unit_->unpacked_patch_[90]=*p(p_op2_eg_level_3);
  synth_unit_->unpacked_patch_[91]=*p(p_op2_eg_level_4);
  synth_unit_->unpacked_patch_[92]=*p(p_op2_output_level);
  synth_unit_->unpacked_patch_[93]=*p(p_op2_mode);
  synth_unit_->unpacked_patch_[94]=*p(p_op2_f_coarse);
  synth_unit_->unpacked_patch_[95]=*p(p_op2_f_fine);
  synth_unit_->unpacked_patch_[96]=*p(p_op2_osc_detune);
  synth_unit_->unpacked_patch_[97]=*p(p_op2_break_point);
  synth_unit_->unpacked_patch_[98]=*p(p_op2_l_scale_depth);
  synth_unit_->unpacked_patch_[99]=*p(p_op2_r_scale_depth);
  synth_unit_->unpacked_patch_[100]=*p(p_op2_l_key_scale);
  synth_unit_->unpacked_patch_[101]=*p(p_op2_r_key_scale);
  synth_unit_->unpacked_patch_[102]=*p(p_op2_rate_scaling);
  synth_unit_->unpacked_patch_[103]=*p(p_op2_a_mod_sens_);
  synth_unit_->unpacked_patch_[104]=*p(p_op2_key_velocity);
  // OP1
  synth_unit_->unpacked_patch_[105]=*p(p_op1_eg_rate_1);
  synth_unit_->unpacked_patch_[106]=*p(p_op1_eg_rate_2);
  synth_unit_->unpacked_patch_[107]=*p(p_op1_eg_rate_3);
  synth_unit_->unpacked_patch_[108]=*p(p_op1_eg_rate_4);
  synth_unit_->unpacked_patch_[109]=*p(p_op1_eg_level_1);
  synth_unit_->unpacked_patch_[110]=*p(p_op1_eg_level_2);
  synth_unit_->unpacked_patch_[111]=*p(p_op1_eg_level_3);
  synth_unit_->unpacked_patch_[112]=*p(p_op1_eg_level_4);
  synth_unit_->unpacked_patch_[113]=*p(p_op1_output_level);
  synth_unit_->unpacked_patch_[114]=*p(p_op1_mode);
  synth_unit_->unpacked_patch_[115]=*p(p_op1_f_coarse);
  synth_unit_->unpacked_patch_[116]=*p(p_op1_f_fine);
  synth_unit_->unpacked_patch_[117]=*p(p_op1_osc_detune);
  synth_unit_->unpacked_patch_[118]=*p(p_op1_break_point);
  synth_unit_->unpacked_patch_[119]=*p(p_op1_l_scale_depth);
  synth_unit_->unpacked_patch_[120]=*p(p_op1_r_scale_depth);
  synth_unit_->unpacked_patch_[121]=*p(p_op1_l_key_scale);
  synth_unit_->unpacked_patch_[122]=*p(p_op1_r_key_scale);
  synth_unit_->unpacked_patch_[123]=*p(p_op1_rate_scaling);
  synth_unit_->unpacked_patch_[124]=*p(p_op1_a_mod_sens_);
  synth_unit_->unpacked_patch_[125]=*p(p_op1_key_velocity);
  // Global for all OPs
  synth_unit_->unpacked_patch_[126]=*p(p_pitch_eg_rate_1);
  synth_unit_->unpacked_patch_[127]=*p(p_pitch_eg_rate_2);
  synth_unit_->unpacked_patch_[128]=*p(p_pitch_eg_rate_3);
  synth_unit_->unpacked_patch_[129]=*p(p_pitch_eg_rate_4);
  synth_unit_->unpacked_patch_[130]=*p(p_pitch_eg_level_1);
  synth_unit_->unpacked_patch_[131]=*p(p_pitch_eg_level_2);
  synth_unit_->unpacked_patch_[132]=*p(p_pitch_eg_level_3);
  synth_unit_->unpacked_patch_[133]=*p(p_pitch_eg_level_4);
  synth_unit_->unpacked_patch_[134]=*p(p_algorithm);
  synth_unit_->unpacked_patch_[135]=*p(p_feedback);
  synth_unit_->unpacked_patch_[136]=*p(p_osc_key_sync);
  synth_unit_->unpacked_patch_[137]=*p(p_lfo_speed);
  synth_unit_->unpacked_patch_[138]=*p(p_lfo_delay);
  synth_unit_->unpacked_patch_[139]=*p(p_lfo_pm_depth);
  synth_unit_->unpacked_patch_[140]=*p(p_lfo_am_depth);
  synth_unit_->unpacked_patch_[141]=*p(p_lfo_key_sync);
  synth_unit_->unpacked_patch_[142]=*p(p_lfo_wave);
  synth_unit_->unpacked_patch_[143]=*p(p_p_mode_sens_);
  synth_unit_->unpacked_patch_[144]=*p(p_middle_c);
  // 10 bytes (145-154) are the name of the patch
  synth_unit_->unpacked_patch_[155]=0x3f; // operator on/off => All OPs on

  TRACE("Bye");
}

// override the run() method
void DX7::run (uint32_t sample_count)
{
    const LV2_Atom_Sequence* seq = p<LV2_Atom_Sequence> (p_midi_in);
    float* output = p(p_audio_out);
    uint32_t last_frame = 0, num_this_time = 0;
    float level=*p(p_output)*scaler;

    for (LV2_Atom_Event* ev = lv2_atom_sequence_begin (&seq->body);
         !lv2_atom_sequence_is_end(&seq->body, seq->atom.size, ev);
         ev = lv2_atom_sequence_next (ev))
    {
       //set_params(); // pre_process: copy actual voice params

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
