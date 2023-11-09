//#include "../include/Osc.h"
#include "Osc.h"


int BUFFER_SIZE = 256;
int SAMPLE_RATE =  48000;


Osc::Osc(){
   blocksize=BUFFER_SIZE;
   freq.resize(blocksize,0);
   fr=0;
   state=123456789;
}

var Osc::osc(const var& frequency){
   var result(blocksize);
   for(int i=0;i<blocksize;++i){
	double ad = frequency.data[i]/samplerate;
	fr += ad;
	fr = wrap(fr);
	result.data[i] = sin(fr*TWOPI);
   }
   return result;
}
var Osc::phasor(const var& frequency){
   var result(blocksize);
   for(int i=0;i<blocksize;++i){
	double ad = frequency.data[i]/samplerate;
	fr += ad;
	fr = wrap(fr);
	result.data[i] =  fr;
   }
   return result;
}
var Osc::saw(const var& frequency){
   var result(blocksize);
   for(int i=0;i<blocksize;++i){
	double ad =frequency.data[i]/samplerate;
	fr += ad;
	fr = wrap(fr);
	result.data[i] =fr*2.0 -1.0;
   }
   return result;
}
var Osc::rect(const var& frequency) {
	var result(blocksize);
	for (int i = 0; i < blocksize; ++i) {
		double ad = frequency.data[i] / samplerate;
		fr += ad;
		fr = wrap(fr);
		if (fr > 0.5) {
			result.data[i] = 1.0;
		}
		else {
			result.data[i] = -1.0;
		}
	}
	return result;
}

var Osc::osc(double frequency){
  // std::vector<double> result(blocksize,0);
   var result(blocksize);
   for (int i=0;i<blocksize;++i){
	freq[i]=frequency;
   }
   for(int i=0;i<blocksize;++i){
	float ad = freq[i] /samplerate;
        fr +=ad;
        fr =wrap(fr);
        result.data[i] = sin(fr*TWOPI);
   }
   //  return var(result);
     return result;
}

var Osc::phasor(double frequency){
    var result(blocksize);
    for(int i=0;i<blocksize;++i){
	float ad = frequency /samplerate;
	fr +=ad;
	fr = wrap(fr);
        result.data[i] = fr;
    }
    return result;
}

var Osc::saw(double frequency){
    var result(blocksize);
    for(int i=0;i<blocksize;++i){
	float ad = frequency / samplerate;
	fr += ad;
	fr = wrap(fr);
	result.data[i] = fr*2.0-1.0;
    }
    return result;
}

var Osc::rect(double frequency) {
	var result(blocksize);
	for (int i = 0; i < blocksize; ++i) {
		double ad = frequency / samplerate;
		fr += ad;
		fr = wrap(fr);
		if (fr > 0.5) {
			result.data[i] = 1.0;
		}
		else {
			result.data[i] = -1.0;
		}
	}
	return result;
}

var Osc::noise(){
    var result(blocksize);
    for(int i=0;i<blocksize;++i){
	state ^= state<<13;
        state ^= state>>17;
        state ^= state<<5;
        result.data[i] = double(state)/double(0xffffffff)*2.0-1.0;
    }
    return result;
}

Del::Del(double max_dels){
     max_del=max_dels;
     buffer.resize(static_cast<int>(max_del));
     ffbuffer.resize(static_cast<int>(max_del));
     index=0;
     ff_index=0;
     buf_size=static_cast<int>(max_del);
}

void Del::write(const var& input){
     for(int i=0;i<blocksize;++i){
        buffer[index]=input.data[i];
		index=(index+1)%buf_size;
     }
}

var Del::read(double del_time) {
	var result(blocksize);
	for (int i = 0; i < blocksize; ++i) {
		int readindex = (index - static_cast<int>(del_time) + buf_size + i) % buf_size;
		result.data[i] = buffer[readindex];
	}
	return result;
}

var Del::read(const var& del_time) {
	var result(blocksize);
	for (int i = 0; i < blocksize; ++i) {
		double del_samps = abs(del_time.data[i]);
		int del_int = floor(del_samps);
		double frac = del_samps - del_int;
		int r1 = (index - del_int + buf_size + i) % buf_size;
		int r2 = (r1 - 1 + buf_size) % buf_size;
		double y1 = buffer[r1];
		double y2 = buffer[r2];
		result.data[i] = y1 + frac*(y2 - y1);
	}
	return result;
}

var Del::allpass(double del_time, double gain, const var& input) {
	var result(blocksize);
	for (int i = 0; i < blocksize; ++i) {
		int read_index = (index - static_cast<int>(del_time) + buf_size) % buf_size;
		double sum = gain * input.data[i] + buffer[read_index] ;
		buffer[index] = -gain*buffer[read_index] + input.data[i];
		index = (index + 1) % buf_size;
		result.data[i] = sum;
	}
	return result;
}



Count::Count(double max_samps,unsigned int max_counts){
     max_samp=static_cast<int> (max_samps); max_count=max_counts; last=0;count_now=0;
     trig_table.resize(blocksize,0.0);
}

void Count::on(){
    for(int i=0;i<blocksize;++i){
		last=last+1;
	   if(last>=max_samp){
			count_now=(count_now+1)%max_count;
			last=0;
			trig_table[i] = 1;
			rn_flag = true;
       }
	   else{trig_table[i] = 0;
	   }
    }
}


unsigned int Count::num(){
        return count_now;
}

var Count::trig(){
   var result(trig_table);
   return result;
}

double Count::rnd_f(double min, double max) {
	if (rn_flag) {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_real_distribution<> distrib(min, max);
		rnd_val_d = distrib(gen);
		rn_flag = false;
	}
	return rnd_val_d;
}

int Count::rnd_i(double min, double max) {
	if (rn_flag) {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_int_distribution<> distrib(min, max);
		rnd_val_i = distrib(gen);
		rn_flag = false;
	}
	return rnd_val_i;
}

Env::Env(double att,double rel){
	attack=att;release=rel;count=0;state=0;
	env_table.resize(blocksize,0.0);
}

var Env::on(const var& trig){
	var result = trig;
	double sig=0;
	for(int i=0;i<trig.data.size();++i){
		sig=0;
		if(trig.data[i]>=1){
 			state=1;
   			count=0;
                }
		if(state==1){
			count+=1;
			if(count>=attack){
				state=2;
				count=0;
			}
			sig=count/attack;
		}
		if(state==2){
			count+=1;
			if(count>=release){
				state=0;
			}
			sig=1-count/release;
		}
		result.data[i] = sig;
 		}
 	return result;
}

var Env::on(const var& trig,double exp_val){
	var result =trig;
	double sig=0;
	for(int i=0;i<trig.data.size();++i){
		sig=0;
		if(trig.data[i]==1){
			state=1;
			count=0;
		}
		if(state==1){
			count+=1;
			if(count>=attack){
				state=2;
				count=0;
			}
			sig=count/attack;
		}
		if(state==2){
			count+=1;
			if(count>=release){
				state=0;
			}
			sig=1-pow(count/release,1.0/exp_val);
		}
		result.data[i] = sig;
	}
	return result;
}


Filt::Filt():mode("None"),freq(100.0), q_factor(0.5) {
	blocksize = BUFFER_SIZE;
	samplerate = SAMPLE_RATE;
}



var Filt::biquad(std::string mode,const var& frequency,const var& Q,const var& input) {
	var result(blocksize);
	var f_var = frequency;
	for (int i = 0; i < input.data.size(); ++i) {
		double sum = 0;
		freq = f_var.data[i];
		double q_val = std::max(Q.data[i], 0.0001);
		del_ff0 = input.data[i];
		omega = twopi * freq / samplerate;
		double sinw = sin(omega); double cosw = cos(omega);
		double alpha = sinw / (2 * q_val);

		if (mode == "low") {
			a0 = 1.0 + alpha;
			a1 = -2.0 * cosw;
			a2 = 1.0 - alpha;
			b0 = (1.0 - cosw) / 2.0;
			b1 = 1.0 - cosw;
			b2 = (1.0 - cosw) / 2.0;
		}
		else if (mode == "high") {
			a0 = 1.0 + alpha;
			a1 = -2.0 * cosw;
			a2 = 1.0 - alpha;
			b0 = (1.0 + cosw) / 2.0;
			b1 = -(1.0 + cosw);
			b2 = (1.0 + cosw) / 2.0;
		}
		else if (mode == "bp") {
			a0 = 1.0 + alpha;
			a1 = -2.0 * cosw;
			a2 = 1.0 - alpha;
			b0 = alpha;
			b1 = 0.0;
			b2 = -alpha;
		}
		double ff0 = b0 / a0;
		double ff1 = b1 / a0;
		double ff2 = b2 / a0;
		double fb1 = a1 / a0;
		double fb2 = a2 / a0;
		sum = ff0 * del_ff0 + ff1 * del_ff1 + ff2 * del_ff2 - fb1 * del_fb1 - fb2 * del_fb2;
		result.data[i] = sum;
		del_ff2 = del_ff1;
		del_ff1 = del_ff0;
		del_fb2 = del_fb1;
		del_fb1 = sum;
			
	}
	return result;
}


var Filt::biquad(std::string fmode, double frequency, double q, const var& input) {
	var freq_v(blocksize, frequency);
	var q_v(blocksize, q);
	var result = input;
	result = biquad(fmode, freq_v, q_v, input);
	return result;
}

var Filt::biquad(std::string fmode, const var& frequency, double q, const var& input) {
	var q_v(blocksize, q);
	var result = input;
	result = biquad(fmode, frequency, q_v, input);
	return result;
}

var Filt::biquad(std::string fmode, double frequency, const var& q, const var& input) {
	var freq_v(blocksize, frequency);
	var result = input;
	result = biquad(fmode, freq_v, q, input);
	return result;
}


Fn::Fn() {
}

var Fn::sah(const var& input, const var& mod, double border) {
	var result = input;
	for (int i = 0; i < input.data.size(); ++i) {
		double diff = (border - mod.data[i]) * (border - pre_val);
		if (diff <= 0.0) {
			sah_val = input.data[i];
		}
		result.data[i] = sah_val;
		pre_val = input.data[i];
	}
	return result;
}
