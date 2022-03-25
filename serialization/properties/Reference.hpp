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
            virtual void reset() override {
                m_value.reset();
            }
            virtual int serialize(void* buffer) const override {
                return m_value.serialize(buffer);
            }
            virtual int deserialize(const void* buffer) override {
                return m_value.deserialize(buffer);
            }
            virtual unsigned int max_serial_length() const override {
                return m_value.max_serial_length();
            }
            virtual unsigned int serial_length() const override {
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
        private:
            T m_value;
    };
};

#endif //_CEREAL_PACK_REFERENCE_HPP_
