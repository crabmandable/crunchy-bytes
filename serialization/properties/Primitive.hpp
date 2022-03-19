#ifndef _CEREAL_PACK_PRIMITIVE_HPP_
#define _CEREAL_PACK_PRIMITIVE_HPP_
#include "Property.hpp"
#include <type_traits>
#include <cstring>

namespace cereal_pack {
    template<class T>
    class Primitive : public Property {
        static_assert(std::is_fundamental<T>::value, "Primative property must be a fundamental type");
        public:
            virtual void reset() override {
                m_value = (T)0;
            }

            virtual int serialize(void* buffer) const override {
                memcpy(buffer, &m_value, sizeof(m_value));
                return sizeof(T);
            };

            virtual int deserialize(const void* buffer) override {
                memcpy(&m_value, buffer, sizeof(m_value));
                return sizeof(T);
            };

            virtual unsigned int max_serial_length() const override {
                return sizeof(T);
            }

            virtual unsigned int serial_length() const override {
                return sizeof(T);
            }

            void set(T val) { m_value = val; }
            T get() const { return m_value; }
        private:
            T m_value = 0;
    };
};

#endif //_CEREAL_PACK_PRIMITIVE_HPP_
