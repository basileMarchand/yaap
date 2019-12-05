//==============================================================================
// Copyright (C) 2019 Marchand Basile
// 
// This file is part of yaap.
// 
// yaap is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
// 
// yaap is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with yaap.  If not, see <http://www.gnu.org/licenses/>
//==============================================================================



#ifndef _YAAP_H__
#define _YAAP_H__


#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

namespace yaap{

  namespace strutil{
    bool startwith(const std::string& token, const std::string& pattern){
      bool ret = false;
      if( token.substr( 0, pattern.size()) == pattern){
	ret = true;
      }
      return ret;
    }

    void tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters = ",")
    {
      // Skip delimiters at beginning.
      std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
      // Find first non-delimiter.
      std::string::size_type pos = str.find_first_of(delimiters, lastPos);
      while (std::string::npos != pos || std::string::npos != lastPos) {
	// Found a token, add it to the vector.
	tokens.push_back(str.substr(lastPos, pos - lastPos));
	// Skip delimiters.
	lastPos = str.find_first_not_of(delimiters, pos);
	// Find next non-delimiter.
	pos = str.find_first_of(delimiters, lastPos);
      }
    }


    template<typename T>
    struct STRANS{ 
      static T convert(std::string x){ return ;}
      static T convert(int x){ return ;}
      static T convert(size_t x){return ;}
      static T convert(double x){return ;}
      static T convert(std::vector<int> x){ return ;}
    };


    template<> 
    struct STRANS<int>{ 
      static int convert(std::string x){return stoi(x);}
    };
    template<> 
    struct STRANS<bool>{ 
      static bool convert(std::string x){return bool(stoi(x));}
    };
    template<> 
    struct STRANS<unsigned int>{ 
      static unsigned int convert(std::string x){return stoi(x);}
    };
    template<> 
    struct STRANS<double>{ 
      static double convert(std::string x){
	double v=0;
	sscanf(x.c_str(), "%lf",&v);
	return v;
      }
    };

    template<typename T>
    struct STRANS<std::vector<T> >{ 
      static std::vector<T> convert(std::string x){
	std::vector<T> ret;
	std::vector<std::string> tokens;
	yaap::strutil::tokenize(x, tokens, ",");
	for( auto t: tokens){
	  ret.push_back( yaap::strutil::STRANS<T>::convert( t ) );
	}
	return ret;
      }
    };

    template<> 
    struct STRANS<std::string>{ 
      static std::string convert(std::string x){ return x;}
      static std::string convert(int x){return std::to_string(x);}
      static std::string convert(size_t x){return std::to_string(x);}
      static std::string convert(double x){return std::to_string(x);}
      template<typename T>
      static std::string convert(std::vector<T> x){
	std::string ret = "(";
	for( int i=0; i<x.size()-1; i++){
	  ret += STRANS<std::string>::convert( x[i] ) + "," ;
	}
	ret += STRANS<std::string>::convert( x[x.size()-1] ) + ")";
	return ret;
      }
    };
  }

  class ARG{
  public:
    enum arg_t{
      OPTIONAL,
      REQUIRED,
    };
    
    void info(){
      std::cout << this->_short_name <<  ", " << this->_name << std::endl ;
    }
    
    bool isOption(){
      bool ret = false;
      if( this->option == ARG::OPTIONAL ){
	ret = true;
      }
      return ret;
    }
    
    std::string name(){
      return this->_name;
    }
    
    std::string shortname(){
      return this->_short_name;
    }
    
    bool needValue(){
      return this->_need_val;
    }
    
    virtual bool hasValue(){
      return this->has_value;
    }

    virtual void setValue( std::string x)=0;
    virtual void setValue()=0;

  protected:
    arg_t option;
    bool _need_val;
    bool has_value;
    std::string _short_name;
    std::string _name;
    std::string type;
  };
  
  
  template<typename T> struct NEED{ static bool eval(){return false;} };
  template<> struct NEED<std::string>{ static bool eval(){return true;} };
  template<> struct NEED<double>{ static bool eval(){return true;} };
  template<> struct NEED<int>{ static bool eval(){return true;} };
  template<> struct NEED<bool>{ static bool eval(){return false;} };
  template<typename T> struct NEED<std::vector<T> >{ static bool eval(){return true;} };
  
  template<typename T>
  class ARGUMENT: public ARG{
  public:
    ARGUMENT(std::string short_name, std::string long_name, ARG::arg_t option){
      this->_short_name = short_name;
      this->_name = long_name;
      this->option = option;
      this->_need_val = NEED<T>::eval();
    }
    
    void setValue(std::string x){
      this->def_value  = yaap::strutil::STRANS<T>::convert( x );
      this->has_value = true;
    }
    
    void setTypedValue(T x){
      this->def_value  = x;
      this->has_value = true;
    }
    
    void setValue(){}
    
    T getValue(){
      return this->def_value;
    }
    
  private:
    T def_value;
  };
  
  template<> 
  void ARGUMENT<bool>::setValue(){
    this->def_value = !this->def_value;
  }
  
  class ArgumentParser;
  
  
  static ArgumentParser* _instance=nullptr;
  
  
  class ArgumentParser{
  private:
    int _argc;
    char** _argv;
    std::vector<ARG*> args;
    std::vector<ARG*> inputs;
    
    
    
    // ArgumentParser follow the singleton design pattern
    // implies constructor, copy constructor and assignement 
    // are private
    ArgumentParser(){
      this->args.resize(0);
      this->inputs.resize(0);
    }
    
  public:
    
    //static ArgumentParser* _instance;
    
    static ArgumentParser* Instance(){
      if( _instance == nullptr){
	_instance = new ArgumentParser;
      }
      return _instance;
    }
    
    
    template<typename T>
    static T Option( const std::string& opt_name ){
      ArgumentParser* arg = ArgumentParser::Instance();
      T ret = arg->argVal<T>(opt_name);
      return ret;
    }
    
    
    static int GetArgc(){
      ArgumentParser* arg = ArgumentParser::Instance();
      return arg->_argc;
    }
    
    static char** GetArgv(){
      ArgumentParser* arg = ArgumentParser::Instance();
      return arg->_argv;
    }
    


    template<typename T>
    void addArg(std::string short_name, std::string long_name, ARG::arg_t option){
      ARGUMENT<T>* arg = new ARGUMENT<T>(short_name, long_name, option);
      this->args.push_back(arg);
    }

    template<typename T>
    void addArg(std::string short_name, std::string long_name, ARG::arg_t option, T def_value){
      ARGUMENT<T>* arg = new ARGUMENT<T>(short_name, long_name, option);
      arg->setTypedValue( def_value );
      this->args.push_back(arg);
    }

    template<typename T>
    void addInput(std::string name, ARG::arg_t status){
      ARG* inp = new ARGUMENT<T>( name, name, status );
      this->inputs.push_back( inp );
    }
    
    
    ARG* find_arg( const std::string& arg_name ){
      std::vector<ARG*>::iterator it;
      for( it=this->args.begin(); it != this->args.end(); ++it){
	if( (*it)->name() == arg_name || (*it)->shortname() == arg_name ){
	  return (*it);
	}
      }
      return nullptr;
    }
    
    ARG* find_input( const std::string& arg_name ){
      std::vector<ARG*>::iterator it;
      for( it=this->inputs.begin(); it != this->inputs.end(); ++it){
	if( (*it)->name() == arg_name || (*it)->shortname() == arg_name ){
	  return (*it);
	}
      }
      return nullptr;
    }
    
    void parse(int argc, char* argv[]){
      this->_argc = argc;
      this->_argv = argv;
      
      int counter = 0;
      int opt_counter = 0;
      std::string token;
      while( counter < argc ){
	token = std::string(argv[counter]);
	counter++;
	if( yaap::strutil::startwith(token, "-") ){
	  ARG* arg = this->find_arg( token );
	  if( arg != NULL ){
	    opt_counter++;
	    if( arg->needValue() ){
	      token = std::string(argv[counter]);
	      counter++;
	      arg->setValue(token);
	      opt_counter++;
	    }
	    else{
	      arg->setValue();
	    }
	  }
	  else{
	    throw std::invalid_argument( token + " is not an available argument");
	  }
	}
      }
      
      this->parse_input(argc, opt_counter, argv);      
      this->verify();
    }
    
    void parse_input(int& argc, int& counter, char* argv[]){
      std::string token;
      if( (argc-1) - counter != (int)this->inputs.size()){
	return;
      }
      for( size_t i=0; i<this->inputs.size(); i++){
	ARG* arg = this->inputs[i];
	token = std::string(argv[i+counter+1]);
	arg->setValue(token);
      }
    }
    
    
    void verify(){
      std::vector<ARG*>::iterator it;
      for( it=this->args.begin(); it != this->args.end(); ++it){
	if( (*it)->hasValue() || (*it)->isOption() ){
	  continue;
	}
	else{
	  this->usage();
	  exit(1);
	}
      }
    }
    
    void usage(){
      std::cout << "usage: " << std::endl;
      std::vector<ARG*>::iterator it;
      for( it=args.begin(); it!=args.end(); ++it){
	(*it)->info();
      }
    }
    
    
    template<typename T>
    T argVal(const std::string x){
      ARG* ptr = this->find_arg(x);
      if( ptr == NULL){
	throw std::invalid_argument("The argument " + x + " is not registered in the ARGUMENT_PARSER");
      }
      ARGUMENT<T> * arg = (ARGUMENT<T>*)ptr;
      return arg->getValue();
    }
    
    template<typename T>
    T inputVal(std::string x){
      ARG* ptr = this->find_input(x);
      if( ptr == NULL){
	throw std::invalid_argument("The input " + x + "is missing");
      }
      ARGUMENT<T> * arg = (ARGUMENT<T>*)ptr;
      return arg->getValue();
    }
    
  };

}

#endif
