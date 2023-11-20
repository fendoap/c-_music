#ifndef OSC_H
#define OSC_H

#define _USE_MATH_DEFINES
#include <vector>
#include <cmath>
#include <math.h>
#include <initializer_list>
#include <cstdint>
#include <string>
#include <random>
/*
class Config {
 public:
   static const int blocksize=128;
   static const double saplerate = 48000;
}
*/

extern int BUFFER_SIZE;
extern int SAMPLE_RATE;

class  var{

 public:
  std::vector<double>  data;
  var(const std::vector<double>& vec): data(vec) {}
  var(double size):data(static_cast<std::size_t>(size),0.0){}
  var(double size, double initialValue) : data(static_cast<std::size_t>(size), initialValue) {}
  var(std::initializer_list<double> init_list):data(init_list){}

  const double& operator[](size_t index){
    return data[index];
  }
  var operator[](const var& index) {
      var result = index;
      for (int i = 0; i < index.data.size(); ++i) {
          std::size_t index_i = static_cast<std::size_t>(index.data[i]);
          if (index_i < 0) { index_i = 0; }
          if (index_i > data.size()) { index_i = data.size() - 1; }
          result.data[i] = data[index_i];
      }
      return result;
  }
};

inline double wrap(double value){
   double result=fmod(value,1.0);
   if(result <0.0){
	result += 1.0;
   }
   return result;
}



class Osc{
public:
  Osc();
//  std::vector<double> osc(double frequency);
  var osc(const var& frequency);
  var phasor(const var& frequency);
  var saw(const var& frequency);
  var osc(double frequency);
  var phasor(double frequency);
  var saw(double frequency);
  var noise();
  var rect(const var& frequency);
  var rect(double frequency);
  var table(const var& table, const var& frequency);
  var table(const var& table, double frequency);
 private:
 // int block=Config::blocksize;
  int blocksize=BUFFER_SIZE;
  //std::vector<double> phase;
  std::vector<double> freq;
  double samplerate = SAMPLE_RATE;
  double fr;
  double TWOPI = 2.0*M_PI;
  uint32_t state;
 // double rate=Config::samplerate;
};

class Fn {
public:
    Fn();
    var sah(const var& input, const var& mod, double border);
private:
    double sah_val = 0.0;
    double pre_val = 0.0;
};


class Count{
public:
   Count(double max_samps,unsigned int max_counts);
   void on();
   unsigned int num();
   var trig();
   double rnd_f(double min,double max);
   int rnd_i(double min, double max);

private:
   unsigned int max_samp;
   unsigned int max_count;
   unsigned int last;
   unsigned int count_now;
   int blocksize=BUFFER_SIZE;
   std::vector<double> trig_table;
   double rnd_val_d=0.0;
   int rnd_val_i = 0;
   bool rn_flag = true;
};

class Env{
public:
        Env(double attack,double release);
        var on(const var& trig);
        var on(const var& trig,double exp_val);
private:
   double count;
   std::vector<double> env_table;
   int state;
   int blocksize=BUFFER_SIZE;
   double attack;
   double release;
};

class Del{
public:
        Del(double max_del);
        void write(const var& input);
        var read(double del_time);
        var read(const var& del_time);
        var allpass(double del_time,double gain, const var& input);

private:
        double max_del;
        std::vector<double>buffer;
        std::vector<double>ffbuffer;
        int blocksize=BUFFER_SIZE;
        int index;
        int ff_index;
        int buf_size;
};



class Filt {
public:
    Filt();
    var biquad(std::string fmode,const var& frequency,const var& q,const var& input);
    var biquad(std::string fmode,double frequency, double q, const var& input);
    var biquad(std::string fmode, const var& frequency, double q, const var& input);
    var biquad(std::string fmode, double frequency, const var& q, const var& input);

private:
    std::string mode;    // low high band
    int blocksize = BUFFER_SIZE;
    int samplerate = SAMPLE_RATE;
    double freq;
    double q_factor;
    double alpha = 0;
    double omega = 0;
    double del_ff0 = 0;
    double del_ff1 = 0;
    double del_ff2 = 0;
    double del_fb1 = 0;
    double del_fb2 = 0;
    double a0 = 0;
    double a1 = 0;
    double a2 = 0;
    double b0 = 0;
    double b1 = 0;
    double b2 = 0;
    double twopi = 2.0 * M_PI;

};






#endif 
