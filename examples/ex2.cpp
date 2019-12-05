#include "yaap.hpp"

int main(int argc, char* argv[]){

  yaap::ArgumentParser* args = yaap::ArgumentParser::Instance();
  args->addArg<bool>("-h", "--help", yaap::ARG::OPTIONAL, false);
  args->addArg<bool>("-v", "--version", yaap::ARG::OPTIONAL);
  args->addArg<int>("-np", "--nprocess", yaap::ARG::OPTIONAL, 1);
  args->addArg<std::vector<int> >("-i", "--index", yaap::ARG::OPTIONAL, {1,});
  args->addInput<std::string>("study", yaap::ARG::REQUIRED);
  try{
    args->parse(argc, argv);
  }
  catch(...){ 
    args->usage();
  }
  
  if( args->argVal<bool>("-h") ){
    args->usage();
  }


  std::vector<int> index = args->argVal<std::vector<int> >("-i");
  for( auto i: index){
    std::cout << "index : " << i << std::endl;
  }


  return 0;
}
