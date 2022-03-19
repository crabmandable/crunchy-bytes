#ifndef _CEREAL_PACK_SCHEMA_HPP_
#define _CEREAL_PACK_SCHEMA_HPP_

#include <vector>
#include "properties/Property.hpp"
#include <stdint.h>
#include <numeric>

namespace cereal_pack {
    class Schema {
        public:
            void reset() {
                for (auto* prop : properties()) {
                    prop->reset();
                }
            }
            int serialize(void* buffer) const {
                const auto& props = properties();
                return std::accumulate(props.begin(), props.end(), 0, [&buffer](int accum, const Property* prop) {
                    return accum + prop->serialize(buffer);
                });
            }
            int deserialize(const void* buffer) {
                const auto& props = properties();
                return std::accumulate(props.begin(), props.end(), 0, [&buffer](int accum, Property* prop) {
                    return accum + prop->deserialize(buffer);
                });
            }
            unsigned int max_serial_length() const {
                const auto& props = properties();
                return std::accumulate(props.begin(), props.end(), 0, [](unsigned int accum, const Property* prop) {
                    return accum + prop->max_serial_length();
                });
            }
            unsigned int serial_length() const {
                const auto& props = properties();
                return std::accumulate(props.begin(), props.end(), 0, [](unsigned int accum, const Property* prop) {
                    return accum + prop->serial_length();
                });
            }

            unsigned int number_of_properties() const {
                return properties().size();
            }

            const Property* property_at(unsigned int index) const {
                return properties().at(index);
            }
        private:
            virtual const std::vector<Property*>& properties() const = 0;
    };
};
#endif //_CEREAL_PACK_SCHEMA_HPP_
