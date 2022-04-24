#ifndef _CEREAL_PACK_REFERENCE_HPP_
#define _CEREAL_PACK_REFERENCE_HPP_

#include "Property.hpp"
#include <type_traits>

class Schema;

namespace cereal_pack {
    template <class T>
    class Reference : public Property {
        static_assert(std::is_base_of<Schema, T>::value, "Reference must refer to a schema");

        public:
            Reference() = default;
            Reference(const T& v) : m_value{v} {}
            operator T() const { return m_value; }

            Reference<T>& operator=(const Reference<T> &rhs) {
                if (this == &rhs) return *this;
                m_value = rhs.m_value;
                return *this;
            }

            Reference(const Reference<T> &other) {
                *this = other;
            }

            bool operator==(const Reference<T> &rhs) const {
                return m_value == rhs.m_value;
            }

            bool operator!=(const Reference<T> &rhs) const {
                return !(*this == rhs);
            }

            virtual void reset() override {
                m_value.reset();
            }
            virtual size_t serialize(void* buffer) const override {
                return m_value.serialize(buffer);
            }
            virtual size_t deserialize(const void* buffer) override {
                return m_value.deserialize(buffer);
            }
            virtual size_t max_serial_length() const override {
                return m_value.max_serial_length();
            }
            virtual size_t serial_length() const override {
                return m_value.serial_length();
            }
            void set(const T& value) {
                m_value = value;
            }
            void set(T&& value) {
                m_value = value;
            }
            const T& get() const {
                return m_value;
            }
            T& get() {
                return m_value;
            }
        private:
            T m_value;
    };
};

#endif //_CEREAL_PACK_REFERENCE_HPP_
