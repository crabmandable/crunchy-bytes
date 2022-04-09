#ifndef _CEREAL_PACK_PROPERTY_HPP_
#define _CEREAL_PACK_PROPERTY_HPP_
#include <stddef.h>

namespace cereal_pack {
    class Property {
        public:
            virtual void reset() = 0;
            virtual size_t serialize(void* buffer) const = 0;
            virtual size_t deserialize(const void* buffer) = 0;
            virtual size_t max_serial_length() const = 0;
            virtual size_t serial_length() const = 0;
    };
};
#endif //_CEREAL_PACK_PROPERTY_HPP_
