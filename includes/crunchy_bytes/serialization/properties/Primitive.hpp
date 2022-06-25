#ifndef _CRUNCHY_BYTES_PRIMITIVE_HPP_
#define _CRUNCHY_BYTES_PRIMITIVE_HPP_
#include "Property.hpp"
#include <type_traits>
#include <cstring>

namespace crunchy_bytes {
    template<class T>
    class Primitive : public Property {
        static_assert(std::is_fundamental<T>::value || std::is_enum<T>::value,
                "Primative property must be a fundamental type");
        public:
            Primitive() = default;
            Primitive(const T& v) : m_value{v} {}
            operator T() const { return m_value; }

            Primitive<T>& operator=(const Primitive<T> &rhs) {
                if (this == &rhs) return *this;
                m_value = rhs.m_value;
                return *this;
            }
            Primitive(const Primitive<T> &other) {
                *this = other;
            }

            bool operator==(const Primitive<T> &rhs) const {
                return m_value == rhs.m_value;
            }

            bool operator!=(const Primitive<T> &rhs) const {
                return !(*this == rhs);
            }

            virtual void reset() override {
                m_value = (T)0;
            }

            virtual uint32_t serialize(void* buffer) const override {
                memcpy(buffer, &m_value, sizeof(m_value));
                return sizeof(T);
            };

            virtual uint32_t deserialize(const void* buffer) override {
                memcpy(&m_value, buffer, sizeof(m_value));
                return sizeof(T);
            };

            virtual uint32_t max_serial_length() const override {
                return sizeof(T);
            }

            virtual uint32_t serial_length() const override {
                return sizeof(T);
            }

            void set(T val) { m_value = val; }
            T get() const { return m_value; }
        private:
            T m_value = (T)0;
    };
};

#endif //_CRUNCHY_BYTES_PRIMITIVE_HPP_
