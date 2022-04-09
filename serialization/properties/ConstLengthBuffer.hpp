#ifndef _CERAL_PACK_CONSTLENGTHBUFFER_HPP_
#define _CERAL_PACK_CONSTLENGTHBUFFER_HPP_
#include "Property.hpp"
#include <stdint.h>
#include <stddef.h>
#include <cstring>
#include <vector>
#include <array>

namespace cereal_pack {
    template <size_t length>
    class ConstLengthBuffer: public Property {
        public:
            ConstLengthBuffer() {
                m_value.resize(length);
                std::fill(m_value.begin(), m_value.end(), 0);
            }

            template < template < class ... > class Container, class ... Args >
            ConstLengthBuffer(const Container<uint8_t, Args...>& data) {
                if (data.size() > length) {
                    throw "Unable to construct buffer, too big";
                }
                m_value.resize(length);
                std::fill(m_value.begin(), m_value.end(), 0);
                memcpy(m_value.data(), data.data(), data.size());
            }

            template <size_t len>
            ConstLengthBuffer(const std::array<uint8_t, len>& data) {
                static_assert(len <= length, "Length must fit within the predefined buffer size");
                m_value.resize(length);
                std::fill(m_value.begin(), m_value.end(), 0);
                memcpy(m_value.data(), data.data(), data.size());
            }

            ConstLengthBuffer(const uint8_t* data) {
                m_value.resize(length);
                memcpy(m_value.data(), data, length);
            }

            bool operator==(const ConstLengthBuffer<length> &rhs) const {
                return m_value == rhs.m_value;
            }

            bool operator!=(const ConstLengthBuffer<length> &rhs) const {
                return !(*this == rhs);
            }

            virtual void reset() override {
                std::fill(m_value.begin(), m_value.end(), 0);
            }

            virtual size_t serialize(void* buffer) const override {
                memcpy(buffer, m_value.data(), length);
                return length;
            }

            virtual size_t deserialize(const void* buffer) override {
                memcpy(m_value.data(), buffer, length);
                return length;
            }

            virtual size_t max_serial_length() const override {
                return length;
            }

            virtual size_t serial_length() const override {
                return length;
            }

            void set(const uint8_t* data) {
                memcpy(m_value.data(), data, length);
            }

            void set(const ConstLengthBuffer<length>& other) {
                *this = other;
            }

            uint8_t* get() { return m_value.data(); }

            const uint8_t* get() const { return m_value.data(); }

            constexpr size_t size() {
                return length;
            }

        private:
            std::vector<uint8_t> m_value;
    };
};

#endif //_CERAL_PACK_CONSTLENGTHBUFFER_HPP_
