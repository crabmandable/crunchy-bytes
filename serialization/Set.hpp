#ifndef _CEREAL_PACK_SET_HPP_
#define _CEREAL_PACK_SET_HPP_

#include "Property.hpp"
#include <type_traits>
#include <vector>
#include <numeric>
#include <stdint.h>
#include <stddef.h>

namespace cereal_pack {
    template<class T, unsigned int max_items>
    class Set : public Property {
        static_assert(std::is_base_of<T, Property>::value, "Set property must be a set of other properties");
        public:
            virtual void reset() override {
                m_value.clear();
            }

            virtual int serialize(void* buffer) const override {
                *(uint32_t*)buffer = m_value.size();
                uint8_t* pos = (uint8_t*)buffer + sizeof(uint32_t);
                for (const auto& item : m_value) {
                    pos += item.serialize(pos);
                }
                return pos - (uint8_t*)buffer;
            }

            virtual int deserialize(const void* buffer) override {
                uint32_t items = *(uint32_t*)buffer;
                if (!number_of_items_is_valid(items)) {
                    //TODO real error
                    throw "Unable to deserialize set, it exceeds max length";
                }
                resize(items);
                const uint8_t* pos = (uint8_t*)buffer + sizeof(uint32_t);
                for (uint32_t i = 0; i < items; i++) {
                    pos += m_value[i].deserialize(pos);
                }

                return pos - (uint8_t*)buffer;
            }

            virtual unsigned int max_serial_length() const override {
                T t;
                return t.max_serial_length() * max_items + sizeof(uint32_t);
            }

            virtual unsigned int serial_length() const override {
                int item_len = std::accumulate(m_value.begin(), m_value.end(), [](const auto& item) {
                    return item.serial_length();
                });
                return item_len + sizeof(uint32_t);
            }

            void set(const std::vector<T>& data) {
                if (!number_of_items_is_valid(data.size())) {
                    //TODO real err
                    throw "Unable to set Set, too many items";
                }
                m_value = data;
            }

            template <typename... Args>
            auto push_back(Args&&... args) {
                if (!number_of_items_is_valid(1 + m_value.size())) {
                    //TODO real err
                    throw "Unable to set Set, too many items";
                }
                return m_value.push_back(std::forward<Args>(args)...);
            }

            template <typename... Args>
            auto emplace_back(Args&&... args) {
                if (!number_of_items_is_valid(1 + m_value.size())) {
                    //TODO real err
                    throw "Unable to set Set, too many items";
                }
                return m_value.emplace_back(std::forward<Args>(args)...);
            }

            const std::vector<T>& get(const std::vector<T>& data) const {
                return m_value;
            }
        private:
            bool number_of_items_is_valid(unsigned int items) const {
                if (items > max_items) {
                    return false;
                }
                if (items > m_value.max_size()) {
                    return false;
                }
                return true;
            }
            void resize(unsigned int length) {
                if (length != m_value.size()) {
                    m_value.resize(length);
                }
            }
            std::vector<T> m_value {0};
    };
}
#endif //_CEREAL_PACK_SET_HPP_
