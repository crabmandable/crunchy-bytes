#ifndef _CEREAL_PACK_PROPERTY_HPP_
#define _CEREAL_PACK_PROPERTY_HPP_

namespace cereal_pack {
    class Property {
        public:
            virtual void reset() = 0;
            virtual int serialize(void* buffer) const = 0;
            virtual int deserialize(const void* buffer) = 0;
            virtual unsigned int max_serial_length() const = 0;
            virtual unsigned int serial_length() const = 0;
    };
};
#endif //_CEREAL_PACK_PROPERTY_HPP_
