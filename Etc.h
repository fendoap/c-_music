#ifndef ETC_H
#define ETC_H

#include <iostream>
#include <cmath>
#include <vector>
#include "Osc.h"




// var +-*/ var
inline var operator+(const var& vec,const var& vec1){
	var result = vec;
	for (int i=0;i<vec.data.size();++i){
		result.data[i] = vec.data[i] + vec1.data[i];
	}
	return result;
}
inline var operator-(const var& vec,const var& vec1){
	var result = vec;
	for (int i=0;i<vec.data.size();++i){
		result.data[i] = vec.data[i] - vec1.data[i];
	}
	return result;
}
inline var operator*(const var& vec,const var& vec1){
	var result = vec;
	for(int i=0;i<vec.data.size();++i){
		result.data[i] = vec.data[i] * vec1.data[i];
	}
	return result;
}
inline var operator/(const var& vec,const var& vec1){
	var result = vec;
	for(int i=0;i<vec.data.size();++i){
		result.data[i] = vec.data[i] / vec1.data[i];
	}
	return result;
}

// var +-*/ double
inline var operator+(const var& vec,double value){
	var result = vec;
	for(int i=0;i<vec.data.size();++i){
		result.data[i] = vec.data[i] + value;
	}
	return result;
}
inline var operator-(const var& vec,double value){
	var result = vec;
	for(int i=0;i<vec.data.size();++i){
		result.data[i] = vec.data[i] - value;
	}
	return result;
}
inline var operator*(const var& vec,double value){
	var result = vec;
	for(int i=0;i<vec.data.size();++i){
		result.data[i] = vec.data[i] * value;
	}
	return result;
}
inline var operator/(const var& vec,double value){
	var result = vec;
	for(int i=0;i<vec.data.size();++i){
		result.data[i] = vec.data[i] / value;
	}
	return result;
}

// += operator
inline var operator+=(var& vec, const var& vec1) {
	
	for (int i = 0; i < vec.data.size(); ++i) {
		vec.data[i] += vec1.data[i];
	}
	return vec;
}
inline var operator+=(var& vec, double value) {
	for (int i = 0; i < vec.data.size(); ++i) {
		vec.data[i] += value;
	}
	return vec;
}

// double +-*/ var
inline var operator+(double value,const var& vec){
        var result = vec;
	for (int i=0;i<vec.data.size();++i){
		result.data[i] = value + vec.data[i];
	}
	return result;
}
inline var operator-(double value,const var& vec){
	var result = vec;
	for(int i=0;i<vec.data.size();++i){
         	result.data[i] = value - vec.data[i];
	}
	return result;
}
inline var operator*(double value,const var& vec){
	var result = vec;
	for(int i=0;i<vec.data.size();++i){
		result.data[i] = value * vec.data[i];
	}
	return result;
}
inline var operator/(double value,const var& vec){
	var result = vec;
	for(int i=0;i<vec.data.size();++i){
		result.data[i] = value / vec.data[i];
	}
	return  result;
}

//expr math var ex) tanh(var)

inline var sin(const var& vec){
	var result = vec;
	for(int i=0;i<vec.data.size();++i){
		result.data[i] = sin(vec.data[i]);
	}
	return result;
}
inline var cos(const var& vec){
	var result = vec;
	for(int i=0;i<vec.data.size();++i){
		result.data[i] = cos(vec.data[i]);
	}
	return result;
}
inline var tan(const var& vec){
	var result = vec;
	for(int i=0;i<vec.data.size();++i){
		result.data[i] = tan(vec.data[i]);
	}
	return result;
}
inline var exp(const var& vec){
	var result = vec;
	for(int i=0;i<vec.data.size();++i){
        	result.data[i] = exp(vec.data[i]);
	}
	return result;
}
inline var log(const var& vec){
	var result = vec;
	for(int i=0;i<vec.data.size();++i){
		result.data[i] = log(vec.data[i]);
	}
	return result;
}
inline var log10(const var& vec){
	var result = vec;
	for(int i=0;i<vec.data.size();++i){
		result.data[i] = log10(vec.data[i]);
	}
	return result;
}
inline var log2(const var& vec){
	var result = vec;
	for(int i=0;i<vec.data.size();++i){
		result.data[i] = log2(vec.data[i]);
	}
	return result;
}
inline var pow(const var& vec,const double y){
	var result = vec;
	for(int i=0;i<vec.data.size();++i){
		result.data[i] = pow(vec.data[i],y);
	}
	return result;
}
inline var pow(double x,const var& vec){
	var result = vec;
	for(int i=0;i<vec.data.size();++i){
		result.data[i] = pow(x,vec.data[i]);
	}
	return result;
}

inline var sqrt(const var& vec){
	var result = vec;
	for(int i=0;i<vec.data.size();++i){
		result.data[i] = sqrt(vec.data[i]);
	}
	return result;
}

inline var mod(const var& vec,double mod) {
	var result = vec;
	for (int i = 0; i < vec.data.size(); ++i) {
		result.data[i] = fmod(vec.data[i] ,mod);
	}
	return result;
}

inline var round(const var& vec) {
	var result = vec;
	for (int i = 0; i < vec.data.size(); ++i) {
		result.data[i] = std::round(vec.data[i]);
	}
	return result;
}

inline var abs(const var& vec) {
	var result = vec;
	for (int i = 0; i < vec.data.size(); ++i) {
		result.data[i] = std::fabs(vec.data[i]);
	}
	return result;
}

inline var floor(const var& vec){
	var result = vec;
	for (int i = 0; i < vec.data.size(); ++i) {
		result.data[i] = std::floor(vec.data[i]);
	}
	return result;
}



inline var tanh(const var& vec){
	var result = vec;
	for(int i=0;i<vec.data.size();++i){
		result.data[i] = tanh(vec.data[i]);
	}
	return result;
}
inline var clip(const var& vec,double min,double max){
	var result = vec;
	for(int i=0;i<vec.data.size();++i){
		result.data[i] = std::min(std::max(vec.data[i],min),max);
	}
	return result;
}
inline double clip(double value,double min,double max){
	return std::min(std::max(value,min),max);
}

inline double mtof(double note){
	double a=440;
	return  (a/32)*std::pow(2,((note - 9)/12.0));
}

inline var mtof(const var& note) {
	var result = note;
	double a = 440;
	for (int i = 0; i < note.data.size(); ++i) {
		double note_i = note.data[i];
		result.data[i] = (a / 32) * std::pow(2,((note_i - 9) / 12.0));
	}
	return result;
}

inline var mtof(const var& note,const var& index){
	var result=index;
	double a=440;
	for(int i=0;i<index.data.size();++i){
		result.data[i] = (a/32)*std::pow(2,((note.data[static_cast<int>(fmod(index.data[i],note.data.size()))]-9)/12.0));
	}
	return result;
}














#endif
