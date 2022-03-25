#ifndef _CERAL_PACK_CONSTLENGTHBUFFER_HPP_
#define _CERAL_PACK_CONSTLENGTHBUFFER_HPP_
#include "Property.hpp"
#include <stdint.h>
#include <stddef.h>
#include <cstring>
#include <vector>

namespace cereal_pack {
    template <size_t length>
    class ConstLengthBuffer: public Property {
        public:
            ConstLengthBuffer() {
                m_value.resize(length);
                std::fill(m_value.begin(), m_value.end(), 0);
            }

            virtual void reset() override {
                std::fill(m_value.begin(), m_value.end(), 0);
            }

            virtual int serialize(void* buffer) const override {
                memcpy(buffer, m_value.data(), length);
                return length;
            }

            virtual int deserialize(const void* buffer) override {
                memcpy(m_value.data(), buffer, length);
                return length;
            }

            virtual unsigned int max_serial_length() const override {
                return length;
            }

            virtual unsigned int serial_length() const override {
                return length;
            }

            void set(uint8_t* data) {
                memcpy(data, m_value.data(), length);
            }

            uint8_t* get() { return m_value.data(); }
            const uint8_t* get() const { return m_value.data(); }

        private:
            std::vector<uint8_t> m_value;
    };
};

#endif //_CERAL_PACK_CONSTLENGTHBUFFER_HPP_
