#ifndef _CEREAL_PACK_SET_HPP_
#define _CEREAL_PACK_SET_HPP_

#include "Property.hpp"
#include "../CerealPackException.hpp"
#include <type_traits>
#include <vector>
#include <array>
#include <stdint.h>
#include <stddef.h>

namespace cereal_pack {
    template<typename T> class Reference;

    template<class T, size_t max_items>
    class Set : public Property {
        static_assert(std::is_base_of<Property, T>::value, "Set property must be a set of other properties");

        public:
            Set() = default;

            Set<T, max_items>& operator=(const Set<T, max_items> &rhs) {
                if (this == &rhs) return *this;
                m_value = rhs.m_value;
                return *this;
            }

            Set(const Set<T, max_items> &other) {
                *this = other;
            }

            bool operator==(const Set<T, max_items> &rhs) const {
                return m_value == rhs.m_value;
            }

            bool operator!=(const Set<T, max_items> &rhs) const {
                return !(*this == rhs);
            }

            virtual void reset() override {
                m_value.clear();
            }

            virtual size_t serialize(void* buffer) const override {
                *(uint32_t*)buffer = m_value.size();
                uint8_t* pos = (uint8_t*)buffer + sizeof(uint32_t);
                for (const auto& item : m_value) {
                    pos += item.serialize(pos);
                }
                return pos - (uint8_t*)buffer;
            }

            virtual size_t deserialize(const void* buffer) override {
                uint32_t items = *(uint32_t*)buffer;
                if (!number_of_items_is_valid(items)) {
                    throw CerealPackException("Unable to deserialize `Set`, number of items exceeds max length");
                }
                resize(items);
                const uint8_t* pos = (uint8_t*)buffer + sizeof(uint32_t);
                for (uint32_t i = 0; i < items; i++) {
                    pos += m_value[i].deserialize(pos);
                }

                return pos - (uint8_t*)buffer;
            }

            virtual size_t max_serial_length() const override {
                T t;
                return t.max_serial_length() * max_items + sizeof(uint32_t);
            }

            virtual size_t serial_length() const override {
                unsigned int accum = 0;
                for (auto& item: m_value) {
                    accum += item.serial_length();
                }
                return accum + sizeof(uint32_t);
            }

            void set(const std::vector<T>& data) {
                if (!number_of_items_is_valid(data.size())) {
                    throw CerealPackException("Unable to set `Set`, number of items exceeds max length");
                }
                m_value = data;
            }

            template <typename A, size_t n>
            void set(const std::array<A, n>& data) {
                static_assert(std::is_convertible<A, T>::value,
                  "Expected `Container` to contain elements convertible to `T`");
                if (!number_of_items_is_valid(data.size())) {
                    throw CerealPackException("Unable to set `Set`, number of items exceeds max length");
                }
                m_value = {data.begin(), data.end()};
            }

            template <typename A, template < class ... > class Container, class ... Args >
            void set(const Container<A, Args...>& data) {
                static_assert(std::is_convertible<typename Container<A, Args...>::value_type, T>::value,
                  "Expected `Container` to contain elements convertible to `T`");

                if (!number_of_items_is_valid(data.size())) {
                    throw CerealPackException("Unable to set `Set`, number of items exceeds max length");
                }
                m_value = {data.begin(), data.end()};
            }

            template <typename... Args>
            auto push_back(Args&&... args) {
                if (!number_of_items_is_valid(1 + m_value.size())) {
                    throw CerealPackException("Unable to `push_back` item to `Set`, number of items exceeds max length");
                }
                return m_value.push_back(std::forward<Args>(args)...);
            }

            template <typename... Args>
            auto emplace_back(Args&&... args) {
                if (!number_of_items_is_valid(1 + m_value.size())) {
                    throw CerealPackException("Unable to `emplace_back` item to `Set`, number of items exceeds max length");
                }
                return m_value.emplace_back(std::forward<Args>(args)...);
            }

            const std::vector<T>& get() const {
                return m_value;
            }

            const auto& operator[](size_t pos) const {
                if constexpr (is_templated_from<T, Reference>::value) {
                    return m_value[pos].get();
                } else {
                    return m_value[pos];
                }
            }

            auto& operator[](size_t pos) {
                if constexpr (is_templated_from<T, Reference>::value) {
                    return m_value[pos].get();
                } else {
                    return m_value[pos];
                }
            }

            void resize(size_t length) {
                if (!number_of_items_is_valid(length)) {
                    throw CerealPackException("Unable to resize `Set`, number of items exceeds max length");
                }
                m_value.resize(length);
            }

            auto size() const -> typename std::vector<T>::size_type {
                return m_value.size();
            }
        private:
            template <class, template <class> class>
                struct is_templated_from : public std::false_type {};
            template <class A, template <class> class U>
                struct is_templated_from<U<A>, U> : public std::true_type {};

            bool number_of_items_is_valid(unsigned int items) const {
                if (items > max_items) {
                    return false;
                }
                if (items > m_value.max_size()) {
                    return false;
                }
                return true;
            }
            std::vector<T> m_value;
    };
}
#endif //_CEREAL_PACK_SET_HPP_
