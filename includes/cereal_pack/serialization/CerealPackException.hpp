#ifndef _CEREAL_PACK_CEREALPACKEXCEPTION_HPP_
#define _CEREAL_PACK_CEREALPACKEXCEPTION_HPP_
#include <exception>
#include <string>

namespace cereal_pack {
    class CerealPackException : public std::exception {
        const std::string message;
        public:
            CerealPackException(char const* const message) throw() :
                message{"Cereal pack error: " + std::string(message)} {}

            virtual const char* what() const throw() override {
                return message.c_str();
            }
    };
};
#endif //_CEREAL_PACK_CEREALPACKEXCEPTION_HPP_
