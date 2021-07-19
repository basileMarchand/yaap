#include "yaap.hpp"

int main(int argc, char* argv[]){
  yaap::ArgumentParser* args = yaap::ArgumentParser::Instance();
  args->addArg<bool>("-h", "--help", yaap::ARG::OPTIONAL, false, "Display help message");
  /*
  args->addArg<bool>("-v", "--version", ARG::OPTIONAL);
  args->addArg<int>("-np", "--nprocess", ARG::OPTIONAL, 1);
  args->addInput<string>("study", ARG::OPTION);
  */
  args->parse(argc, argv);
  if( args->argVal<bool>("-h") ){
    args->usage();
  }
  return 0;
}
