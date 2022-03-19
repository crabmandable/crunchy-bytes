#ifndef _CERAL_PACK_CONSTLENGTHBUFFER_HPP_
#define _CERAL_PACK_CONSTLENGTHBUFFER_HPP_
#include "Property.hpp"
#include <stdint.h>

namespace cereal_pack {
    template<unsigned int length>
    class ConstLengthBuffer: public Property {
        public:
            virtual void reset() override {
                memset(m_value, 0, length);
            }

            virtual int serialize(void* buffer) const override {
                memcpy(m_value, buffer, length);
                return length;
            }

            virtual int deserialize(const void* buffer) override {
                memcpy(buffer, m_value, length);
                return length;
            }

            virtual unsigned int max_serial_length() const override {
                return length;
            }

            virtual unsigned int serial_length() const override {
                return length;
            }

            void set(uint8_t* data) {
                memcpy(data, m_value, length);
            }

            uint8_t* get() { return m_value; }
            const uint8_t* get() const { return m_value; }

        private:
            uint8_t m_value[length] = {0};
    };
};

#endif //_CERAL_PACK_CONSTLENGTHBUFFER_HPP_
