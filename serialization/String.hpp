#ifndef _CEREAL_PACK_STRING_HPP_
#define _CEREAL_PACK_STRING_HPP_
#include "Property.hpp"
#include <stdint.h>
#include <string>
#include <cstring>

namespace cereal_pack {
    template<unsigned int max_string_length>
    class DynamicLengthBuffer: public Property {
        public:
            virtual void reset() override {
                m_value.clear();
            }

            virtual int serialize(void* buffer) const override {
                strcpy((char*)buffer, m_value.c_str());
                return serial_length();
            }

            virtual int deserialize(const void* buffer) override {
                const char* c = (const char*)buffer;
                while (*(c++)) {
                    unsigned int len = c - (const char*)buffer;
                    if (!length_is_valid(len)) {
                        //TODO real error
                        throw "Unable to deserialize string, it exceeds max length";
                    }

                }
                m_value = (char*)buffer;
                return serial_length();
            }

            virtual unsigned int max_serial_length() const override {
                return max_string_length + 1;
            }

            virtual unsigned int serial_length() const override {
                return 1 + m_value.size();
            }

            void set(const std::string& str) {
                if (!length_is_valid(str.size())) {
                    // TODO real err
                    throw "Unable to set string, its too long";
                }
                m_value = str;
            }

            std::string& get() { return m_value; }
            const std::string& get() const { return m_value; }

        private:
            bool length_is_valid(unsigned int length) const {
                if (length > max_string_length) {
                    return false;
                }
                if (length > m_value.max_size()) {
                    return false;
                }
                return true;
            }

            std::string m_value;
    };
};

#endif //_CEREAL_PACK_STRING_HPP_
