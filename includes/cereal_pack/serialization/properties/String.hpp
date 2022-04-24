#ifndef _CEREAL_PACK_STRING_HPP_
#define _CEREAL_PACK_STRING_HPP_

#include "Property.hpp"
#include "../CerealPackException.hpp"
#include <stdint.h>
#include <string>
#include <cstring>

namespace cereal_pack {
    template <size_t max_string_length>
    class String: public Property {
        public:
            String() = default;

            String(const char* value) {
                m_value = value;
            }

            String(const std::string value) {
                m_value = value;
            }

            String<max_string_length>& operator=(const String<max_string_length> &rhs) {
                if (this == &rhs) return *this;
                m_value = rhs.m_value;
                return *this;
            }

            String(const String<max_string_length> &other) {
                *this = other;
            }

            bool operator==(const String<max_string_length> &rhs) const {
                return m_value == rhs.m_value;
            }

            bool operator!=(const String<max_string_length> &rhs) const {
                return !(*this == rhs);
            }

            virtual void reset() override {
                m_value = "";
            }

            virtual size_t serialize(void* buffer) const override {
                strcpy((char*)buffer, m_value.c_str());
                return serial_length();
            }

            virtual size_t deserialize(const void* buffer) override {
                const char* c = (const char*)buffer;
                while (*(c++)) {
                    unsigned int len = c - (const char*)buffer;
                    if (!length_is_valid(len)) {
                        throw CerealPackException("Unable to deserialize `String`, it exceeds max length");
                    }

                }
                m_value = (char*)buffer;
                return serial_length();
            }

            virtual size_t max_serial_length() const override {
                return max_string_length + 1;
            }

            virtual size_t serial_length() const override {
                return 1 + m_value.size();
            }

            void set(const std::string& str) {
                if (!length_is_valid(str.size())) {
                    throw CerealPackException("Unable to set `String`, it exceeds max length");
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

            std::string m_value = "";
    };
};

#endif //_CEREAL_PACK_STRING_HPP_
