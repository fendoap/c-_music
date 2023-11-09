#include <iostream>
#include <cmath>
#include <portaudio.h>
#include <vector>
//#include "../include/Osc.h"
#include "Osc.h"
#include "Etc.h"




Count co(48*500,16);
Count co0(48*1600,16);
Count co1(48*500,16);
Count co2(48*600,16);
Count co3(48*700,16);
Count co4(48 *400, 16);

Count rn1(48 * 200, 16);
Count rn2(48 * 500, 16);
Count rn3(48 * 400, 16);

Count co5(48 * 400, 64);

Del line1(48000 * 20);

Osc s1;Osc s2;Osc s3;Osc s4;Osc s5;Osc s6;Osc s7;
Osc s8;Osc s9;Osc s10;Osc s11;Osc s12;Osc s13;Osc s14;
Osc s15;Osc s16;Osc s17;Osc s18;Osc s19;Osc s20;Osc s21;
Osc sn1;

Osc env_osc1; Osc env_osc2;

Osc k1; Osc k2; Osc k3; Osc k4; Osc k5;

var aa({36,48,36,36,66});
var maj({48,50,52,55,55,57,59});
var trig_seq1({1,0,0,1, 0,0,1,0, 0,1,0,0, 0,0,1,0, 1,0,1,1});
var trig_seq2({0,0,0,0, 1,0,0,0, 0,0,0,0, 1,0,0,0, 1,1,0,0});
var trig_seq3({0,0,1,0, 0,0,0,0, 0,0,0,0, 0,0,1,0, 0,0,0,0});
var trig_seq4({0,1,0,0, 1,0,0,1, 1,0,0,1, 0,0,1,0, 0,1,0,0});

Env ev1(0,48*200);
Env ev2(0,48*200);
Env ev3(48*1,48*700);
Env ev_sn(0,48*200);

Env k_env(0, 48 * 1300);

Env k3_env(0, 48 * 500); Env k4_env(48 * 5, 48 * 50); Env k5_env(20, 48 * 1000.0);

Filt fil_L;
Filt fil_R;

Filt k3_f; Filt k4_f; Filt k5_f;


Del ap1(48 * 1000); Del ap2(48 * 1000); Del ap3(48 * 1000); 
Del ap4(48 * 1000); Del ap5(48 * 1000); Del ap6(48 * 1000);

Del comb1(48 * 10000); Del comb2(48 * 10000);


static int callback(const void* input, void* output, unsigned long frameCount,
                    const PaStreamCallbackTimeInfo* timeInfo,
		    PaStreamCallbackFlags statusFlags,
		    void* userData
		   ){
   float* out = (float*)output;
   unsigned long i;

   co.on(); co0.on(); co1.on(); co2.on(); co3.on(); co4.on(); co5.on(); rn1.on(); rn2.on(); rn3.on();

   double note = mtof(aa.data[co4.num() % 4]);



   var sig_L = s1.rect(note * 1.01) + s2.rect(note * 1.01) + s3.rect(note * 2.003) + s4.rect(note * 0.995);
   var sig_R = s5.rect(note * 1.0) + s6.rect(note * 1.02) + s7.rect(note * 2.002) + s8.rect(note * 0.997);

   sig_L = sig_L + 0.1*tanh(1.5*s9.saw(note * 0.5));
   sig_R = sig_R + 0.1*tanh(1.5*s10.saw(note * 0.5));

   var ev = ev1.on(co.trig()+co3.trig());

   sig_L = sig_L * ev; 
   sig_R = sig_R * ev;


   var ev_bass = ev2.on(co4.trig(), 10);

   var L_dac = tanh(fil_L.biquad("low",(300*s16.osc(1.5)+300)*ev_bass+100.0, 0.5, sig_L));
   var R_dac = tanh(fil_R.biquad("low",(300*s17.osc(1.6)+300)*ev_bass+100.0, 0.5, sig_R));

   
   if (trig_seq1[co.num()]) {
	   var ee = k_env.on(co.trig(), 4);
	   var kick = tanh(k1.osc(45 + 8000 * pow(7, 7 * (ee - 1)) * ee * 6));
	   
	   R_dac = kick;
	   L_dac = kick;
   }

   if (trig_seq2[co.num()]) {

	   var sig = 0.5*k2.osc(mtof(72));
	   R_dac = R_dac + sig;
	   L_dac = L_dac + sig;
   }

  
   var sig2(BUFFER_SIZE);
   if (trig_seq3[co.num()]) {

	   sig2 = 0.5*k3.osc(mtof(84))*k3_env.on(co3.trig());
	   //var ev= k3_env.on(co3.trig(), 2);

	//   sig2 = k3.osc(10000 * pow(4, 5 * (ev - 1)) * ev) * ev;

	 //  sig2 = k3_f.biquad("low", 12000, 0.2, sig2);

   }



	   R_dac = R_dac + sig2;
	   L_dac = L_dac + sig2;
   if (co5.num() > 16) {
	   if (trig_seq4[co4.num()]) {
		   var ev = k4_env.on(co4.trig(),6);
		   var sig_noise = k4_f.biquad("high",7000,1.5,k5.noise() * ev);
		   L_dac = L_dac + sig_noise;
		   R_dac = R_dac + sig_noise;


	   }

   }
   

   var env_test = k5_env.on(rn1.trig()+rn3.trig(), 4);
   env_test = k5_f.biquad("low", 10, 0.2, env_test);

   var f_note = (1.0+0.0*pow(env_test,30)) * mtof((-4.0+rn1.num()%7) * 12.0 + maj[rn2.rnd_i(0, 6)]);

   var L_daca = 0.9 * s20.osc(f_note);

   L_daca = L_daca * env_test;
   
  // L_dac = 0.1 * tanh(L_dac);

   var d_L = L_daca;

   L_daca = 0.7*k3_f.biquad("low", 6000, 0.2, L_daca);

   comb1.write(L_daca + 0.65 * comb1.read(48 * 1300.0));

   L_daca = 0.8*L_daca + 0.65 * comb1.read(48 * 1300.0);

   comb2.write(L_daca + 0.65 * comb2.read(48 * 600.0));

   L_daca = 0.8*L_daca + 0.65 * comb2.read(48 * 600.0);


   L_daca = ap1.allpass(4707, 0.73, L_daca);
   L_daca = ap2.allpass(2703, 0.72, L_daca);
   L_daca = ap3.allpass(3701, 0.72, L_daca);
   L_daca = ap4.allpass(1251, 0.72, L_daca);
   L_daca = ap5.allpass(1763, 0.65, L_daca);
   L_daca = ap6.allpass(677, 0.65, L_daca);
   
   L_daca = 0.15*L_daca + d_L;


 

   line1.write(L_daca);

   var ph_ev = env_osc2.phasor(0.25);
   var ph_ev1 = mod(ph_ev + 0.25, 1);
   var ph_ev2 = mod(ph_ev + 0.5, 1);
   var ph_ev3 = mod(ph_ev + 0.75, 1);

   var rev  = line1.read(48 * 8000.0 * ph_ev)*(0.5*(1-cos(2.0*M_PI*ph_ev)));
   var rev1 = line1.read(48 * 8000.0 * ph_ev1) * (0.5 * (1 - cos(2.0 * M_PI * ph_ev1)));
   var rev2 = line1.read(48 * 8000.0 * ph_ev2) * (0.5 * (1 - cos(2.0 * M_PI * ph_ev2)));
   var rev3 = line1.read(48 * 8000.0 * ph_ev3) * (0.5 * (1 - cos(2.0 * M_PI * ph_ev3)));
   var rev4 = line1.read(48 * 16000.0 * ph_ev2) * (0.5 * (1 - cos(2.0 * M_PI * ph_ev2)));
   var rev5 = line1.read(48 * 16000.0 * ph_ev3) * (0.5 * (1 - cos(2.0 * M_PI * ph_ev3)));

  


   var L_dac1 = 0.3*L_dac + L_daca + rev + rev2 + 0.3*rev4;
   var R_dac1 = 0.3*R_dac + L_daca + rev1 + rev3+ 0.3*rev5;




   for (i = 0;i< frameCount; i++){
       *out++ = 0.2*L_dac1[i];
       *out++ = 0.2*R_dac1[i];
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
