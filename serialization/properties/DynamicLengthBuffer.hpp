#ifndef _CEREAL_PACK_DYNAMICLENGTHBUFFER_HPP_
#define _CEREAL_PACK_DYNAMICLENGTHBUFFER_HPP_

#include "Property.hpp"
#include <stdint.h>
#include <vector>
#include <cstring>

namespace cereal_pack {
    template <size_t max_buffer_length>
    class DynamicLengthBuffer: public Property {
        public:
            virtual void reset() override {
                m_value.clear();
            }

            virtual int serialize(void* buffer) const override {
                *(uint32_t*)buffer = m_value.size();
                memcpy((uint8_t*)buffer + sizeof(uint32_t), m_value.data(), m_value.size());
                return serial_length();
            }

            virtual int deserialize(const void* buffer) override {
                uint32_t l = *(uint32_t*)buffer;
                if (!length_is_valid(l)) {
                    //TODO real error
                    throw "Unable to deserialize buffer, it exceeds max length";
                }
                resize(l);
                memcpy(m_value.data(), (uint8_t*)buffer + sizeof(uint32_t), l);
                return serial_length();
            }

            virtual unsigned int max_serial_length() const override {
                return max_buffer_length + sizeof(uint32_t);
            }

            virtual unsigned int serial_length() const override {
                return sizeof(uint32_t) + m_value.size();
            }

            void set(uint8_t* data, unsigned int length) {
                if (!length_is_valid(length)) {
                    //TODO real error
                    throw "Unable to set buffer, its too big";
                }
                resize(length);
                memcpy(data, m_value.data(), length);
            }

            void set(const std::vector<uint8_t>& data) {
                if (!length_is_valid(data.size())) {
                    //TODO real error
                    throw "Unable to set buffer, its too big";
                }
                m_value = data;
            }

            void set(std::vector<uint8_t>&& data) {
                if (!length_is_valid(data.size())) {
                    //TODO real error
                    throw "Unable to set buffer, its too big";
                }
                m_value = data;
            }

            const std::vector<uint8_t>& get() const { return m_value; }

            auto size() const -> typename std::vector<uint8_t>::size_type {
                return m_value.size();
            }
        private:
            bool length_is_valid(unsigned int length) const {
                if (length > max_buffer_length) {
                    return false;
                }
                if (length > m_value.max_size()) {
                    return false;
                }
                return true;
            }
            void resize(unsigned int length) {
                if (length != m_value.size()) {
                    m_value.resize(length);
                }
            }
            std::vector<uint8_t> m_value {0};
    };
};

#endif //_CEREAL_PACK_DYNAMICLENGTHBUFFER_HPP_
