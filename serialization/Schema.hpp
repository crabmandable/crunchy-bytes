#ifndef _CEREAL_PACK_SCHEMA_HPP_
#define _CEREAL_PACK_SCHEMA_HPP_

#include <vector>
#include "properties/Property.hpp"
#include <stdint.h>

namespace cereal_pack {
    class Schema {
        public:
            void reset() {
                for (auto* prop : properties()) {
                    prop->reset();
                }
            }
            int serialize(void* buffer) const {
                unsigned int pos = 0;
                for (auto* p : properties()) {
                    pos += p->serialize((uint8_t*)buffer + pos);
                }
                return pos;
            }
            int deserialize(const void* buffer) {
                unsigned int pos = 0;
                for (auto* p : properties()) {
                    pos += p->deserialize((uint8_t*)buffer + pos);
                }
                return pos;
            }
            unsigned int max_serial_length() const {
                unsigned int accum = 0;
                for (auto* p : properties()) {
                    accum += p->max_serial_length();
                }
                return accum;
            }
            unsigned int serial_length() const {
                unsigned int accum = 0;
                for (auto* p : properties()) {
                    accum += p->serial_length();
                }
                return accum;
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
