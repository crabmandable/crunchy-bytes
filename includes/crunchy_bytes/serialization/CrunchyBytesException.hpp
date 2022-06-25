#ifndef _CRUNCHY_BYTES_CRUNCHYBYTESEXCEPTION_HPP_
#define _CRUNCHY_BYTES_CRUNCHYBYTESEXCEPTION_HPP_
#include <exception>
#include <string>

namespace crunchy_bytes {
    class CrunchyBytesException : public std::exception {
        const std::string message;
        public:
            CrunchyBytesException(char const* const message) throw() :
                message{"crunchy_bytes error: " + std::string(message)} {}

            virtual const char* what() const throw() override {
                return message.c_str();
            }
    };
};
#endif //_CRUNCHY_BYTES_CRUNCHYBYTESEXCEPTION_HPP_
