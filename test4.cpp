#include <iostream>
#include <cmath>
#include <portaudio.h>
#include <vector>
//#include "../include/Osc.h"
#include "Osc.h"
#include "Etc.h"


Osc si; Osc si1; Osc si2; Osc si3;
Osc lf0; Osc lf1; Osc lf2;


var table_s(256);

Del ts(48 * 40000);
Del ap0(48 * 1000);
Del ap1(48 * 1000);
Del ap2(48 * 1000);
Del ap3(48 * 1000);


std::pair<var,var> dsp_signal_func() {
	var Lch(BUFFER_SIZE), Rch(BUFFER_SIZE);
	for (int i = 0; i < 256; ++i) {
		double s = sin(2.0 * M_PI * i / 256.0);
		s += sin(2 * M_PI * i / 256.0);
		s += sin(3 * M_PI * i / 256.0);
		table_s.data[i] = 0.3*s;
	}
	var sound = si.table(table_s,5000.0*si3.phasor(-0.054))*pow(si1.phasor(-10+10*si2.osc(0.23)),200);	
	ts.write(sound + 0.5 * ts.read(100+48 * 5000 * (1 + lf0.osc(0.1 * lf1.osc(0.031)))));
	sound = ts.read(48 * 1000);
	var ph = lf2.phasor(0.1);
	var ph2 = mod(ph + 0.5,1);
	Lch = ts.read(40*(1+sin(2*M_PI*ph)));
	Rch = ts.read(40*(1+sin(2*M_PI*ph2)));

	Lch+= 0.1*ap0.allpass(303, 0.65, ap1.allpass(777, 0.65, Lch));
	Rch+= 0.1*ap2.allpass(307, 0.65, ap3.allpass(773, 0.65, Rch));

	return std::make_pair(Lch, Rch);
}






static int callback(const void* input, void* output, unsigned long frameCount,
                    const PaStreamCallbackTimeInfo* timeInfo,
		    PaStreamCallbackFlags statusFlags,
		    void* userData
		   ){
   float* out = (float*)output;
   unsigned long i;

 //  var L_dac(BUFFER_SIZE), R_dac(BUFFER_SIZE);
   std::pair<var,var>dac_p = dsp_signal_func();

   var L_dac = dac_p.first;
   var R_dac = dac_p.second;

   for (i = 0;i< frameCount; i++){
   //    *out++ = 0.2*L_dac1[i];
   //    *out++ = 0.2*R_dac1[i];
	   *out++ = 0.9 *clip( L_dac[i],-1,1);
	   *out++ = 0.9 *clip( R_dac[i],-1,1);
   }
   return 0;
}






int main(){




  PaError err = Pa_Initialize();
  if (err != paNoError){
	std::cerr << "PortAudio error" << Pa_GetErrorText(err) << std::endl;
	return 1;
  }



 // std::cout << "portAudio initialized." <<std::endl;

  PaStream* stream;
  err = Pa_OpenDefaultStream(&stream,0,2,paFloat32,SAMPLE_RATE,BUFFER_SIZE,callback,NULL);
  if (err != paNoError) {
    std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
    return 1;
  }

  err =Pa_StartStream(stream);
  if (err != paNoError) {
	std::cerr << "PortAudio error:" << Pa_GetErrorText(err) << std::endl; 
	return 1;
  }

  Pa_Sleep(240000);

  err = Pa_StopStream(stream);
  if (err != paNoError) {
	std::cerr << "PortAudio error: " <<Pa_GetErrorText(err)  << std::endl;
  }

  err = Pa_CloseStream(stream);
  if (err != paNoError ){
	std::cerr << "PortAudio error: "  <<Pa_GetErrorText(err) <<std::endl;

  }



  Pa_Terminate();
  std::cout << "portAudio terminated." <<std::endl;


  return 0;
}
