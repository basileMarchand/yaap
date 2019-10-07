#define CATCH_CONFIG_MAIN

#include "catch2/catch.hpp"
#include "yaap.hpp"


TEST_CASE( "Find argument and return NULL if not found", "[single-file]" ) {
  yaap::ArgumentParser args = *yaap::ArgumentParser::Instance();
  args.addArg<bool>("-h", "--help", yaap::ARG::OPTIONAL, false);
  REQUIRE( args.find_arg("-h") != nullptr );
  REQUIRE( args.find_arg("--help") != nullptr );
  REQUIRE( args.find_arg("--fake") == nullptr );
}

TEST_CASE( "Get value", "[get]"){
  yaap::ArgumentParser args = *yaap::ArgumentParser::Instance();
  args.addArg<int>("-n", "--number", yaap::ARG::OPTIONAL, 1);
  args.addArg<std::string>("-s", "--string", yaap::ARG::OPTIONAL, "fake");
  args.addArg<bool>("-b", "--bool", yaap::ARG::OPTIONAL, true);
  REQUIRE( args.argVal<int>("-n") == 1 );
  REQUIRE( args.argVal<std::string>("-s") == "fake");
  REQUIRE( args.argVal<bool>("-b") == true );
}

TEST_CASE( "Set value from cmd line", "[set]"){
  yaap::ArgumentParser args = *yaap::ArgumentParser::Instance();
  args.addArg<int>("-n", "--number", yaap::ARG::OPTIONAL, 1);
  args.addArg<std::string>("-s", "--string", yaap::ARG::OPTIONAL, "fake");
  args.addArg<bool>("-b", "--bool", yaap::ARG::OPTIONAL, true);
  std::vector<std::string> cmdline = {"yaap_test", "-n", "10", "-s", "coucou", "-b", "false"};
  args.parse(cmdline.size(), (char**)(&cmdline[0]));
  REQUIRE( args.argVal<int>("-n") != 1 );
  REQUIRE( args.argVal<int>("-n") == 10 );
  REQUIRE( args.argVal<std::string>("-s") != "fake");
  REQUIRE( args.argVal<std::string>("-s") == "coucou");
  REQUIRE( args.argVal<bool>("-b") != true );
  REQUIRE( args.argVal<bool>("-b") == false );
}

TEST_CASE( "Argument name verification", "[set verification]"){
  yaap::ArgumentParser args = *yaap::ArgumentParser::Instance();
  args.addArg<int>("-n", "--number", yaap::ARG::OPTIONAL, 1);
  std::vector<std::string> cmdline = {"yaap_test", "-M", "10"};
 
  auto failed=[&args, &cmdline](){
    try{
      args.parse(cmdline.size(), (char**)(&cmdline[0]));
      return false;
    }
    catch(const std::invalid_argument& ia){
      return true;
    }
  };
  REQUIRE( failed()==true );
}








