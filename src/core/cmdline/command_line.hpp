#pragma once
#include <variant>
#include <functional>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>
#include <sstream>
#include <map>
#include <core/core.hpp>

namespace cx::core::cmdline
{
    // to be used when visisting std::variant.
    template <typename ...Ts>
    struct ArgumentVisitor : Ts... {
        ArgumentVisitor(const Ts&... args) : Ts(args)... {}
        using Ts::operator()...;
    };

    template <typename ...Ts>
    auto make_argument_visitor(Ts... lambdas) {
        return ArgumentVisitor<Ts...>{lambdas...};
    }
    // Defines if command line argument is a 
    // flag: -f 
    // a single value: -v 10
    // multi value: --foo one_value two_value three_value
    enum PropertyType {
        FLAG,
        SINGLE_VALUE,
        MULTI_VALUE // Only argument values of string, can be multi value. Multi value integers doesn't make sense.
    };

    // OptionType is a user-defined struct, containing the fields to be set by the command line
    template <typename OptionType>
    struct OptionsParser : OptionType {
        // As one can see here; std::string OptionType::* is a pointer to member of OptionType with type string, etc
        using Property = std::variant<std::string OptionType::*, int OptionType::*, cx::uint OptionType::*, bool OptionType::*>;
        using Argument = std::tuple<std::string, Property, PropertyType>;
        ~OptionsParser() = default;

        auto parse(int argument_count, const char** arguments) -> OptionType {
            std::vector<std::string_view> args{arguments, arguments+argument_count};
            args.erase(args.begin());
            for(int idx = 0; idx < argument_count; ++idx) {
                for(auto& cb : option_parsers) {
                    cb.second(idx, args);
                }
            }
            return static_cast<OptionType>(*this);
        }
        /// Factory function returning a OptionsParser parser
        static auto create(std::initializer_list<Argument> arguments) -> std::unique_ptr<OptionsParser> {
            auto command_options = std::unique_ptr<OptionsParser>(new OptionsParser{});
            for(auto arg : arguments) {
                command_options->register_parsers(arg);
            }
            return command_options;
        }

        private:
        using ArgumentParser = std::function<auto (int, const std::vector<std::string_view>&) -> void>;
        std::map<std::string, ArgumentParser> option_parsers;
        OptionsParser() = default;
        OptionsParser(const OptionsParser&) = delete;
        OptionsParser(OptionsParser&&) = delete;
        OptionsParser& operator=(const OptionsParser&) = delete;
        OptionsParser& operator=(OptionsParser&&) = delete;
        
        /// TODO: Beware - there be exception-y waters ahead. And we do not give a caught at all. Possibly will be implemented.
        auto register_parsers(Argument a) {
            auto arg_name   = std::get<0>(a);
            auto property   = std::get<1>(a);
            auto type       = std::get<2>(a);
            option_parsers[arg_name] = [this, arg_name, property, type](cx::usize id, const std::vector<std::string_view>& args) {
                if(args[id] == arg_name) {
                    auto visitor = make_argument_visitor(
                    [this, id, &args, type](std::string OptionType::* s){
                        if(id < args.size() - 1) {
                            // in a bash shell "these three words" within quotation, is considered 1 argument. We need to check for this condition
                            if(type == SINGLE_VALUE) { 
                                    if(args[id+1].find_first_of(" ") == std::string_view::npos) { // no space means no quotation marks
                                        std::stringstream value;
                                        value << args[id+1];
                                        value >> this->*s;
                                    } else {
                                        this->*s = args[id+1];
                                    }
                            } else if(type == MULTI_VALUE) {
                                this->*s = args[id+1];
                            }
                        }
                    },
                    [this, id, &args](cx::uint OptionType::* i){
                        if(id < args.size() - 1) {
                            std::stringstream value;
                            value << args[id+1];
                            value >> this->*i;
                        }
                    },
                    [this, id, &args](int OptionType::* i){
                        if(id < args.size() - 1) {
                            std::stringstream value;
                            value << args[id+1];
                            value >> this->*i;
                        }
                    },
                    [this, type](bool OptionType::* b){
                        // This is to show that, if we set a flag to something else, it will simply do nothing. Error handling will be added (maybe)
                        if(type == PropertyType::FLAG) {
                            this->*b = true;
                        }
                    });
                    std::visit(visitor, property);
                }
            };
        }

    };
} // namespace cx::util
