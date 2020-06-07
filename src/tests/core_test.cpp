// Basically a test application for my core utils

#include <core/cmdline/command_line.hpp>

constexpr auto SOME_DEFAULT_STR     = "User defined Default value goes here";
constexpr auto SOME_INT_DEFAULT_VAL = 10;
constexpr auto SOME_UINT_DEF_VAL    = 20;
constexpr auto SOME_FLAG_DEFAULT    = false;

// Using command line parser requires user to define their own struct like this. Current types supported inside: unsigned int (here called cx::uint), int, std::string and bool
struct CommandLineOptions {
    std::string title   {SOME_DEFAULT_STR};	    
	int some_int        {SOME_INT_DEFAULT_VAL};
    cx::uint some_uint  {SOME_UINT_DEF_VAL};
    bool some_flag      {SOME_FLAG_DEFAULT};
};
using CLO = CommandLineOptions;
namespace cmd = cx::core::cmdline;

int main(int argc, const char** argv) {
    
    // Each argument is defined like this: {"flag/option-name", pointer to member-type, FLAG/SINGLE_VALUE/MULTI_VALUE property type}
    auto parser = cmd::OptionsParser<CLO>::create({
        {"--title", &CLO::title, cmd::PropertyType::SINGLE_VALUE},
        {"-s",      &CLO::some_int, cmd::PropertyType::SINGLE_VALUE},
        {"-u",      &CLO::some_uint, cmd::PropertyType::SINGLE_VALUE},
        {"-f",      &CLO::some_flag, cmd::PropertyType::FLAG}
    });
    std::vector<std::string> args;
    auto options = parser->parse(argc, argv);
    cx::println("--- Command line options ---\nTitle: {}\nSome int: {}\nSome uint: {}\nSome flag: {}", options.title, options.some_int, options.some_uint, options.some_flag);
}

/*
Running this with:
./core_test --title "Yippie-ki-a motherfucker!" -s 100 -u 2000 -f
--- Command line options ---
Title: Yippie-ki-a motherfucker!
Some int: 100
Some uint: 2000
Some flag: true
*/
