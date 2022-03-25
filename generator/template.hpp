#ifndef __$HEADERGUARD$__
#define __$HEADERGUARD$__

#include <vector>
#include "cereal_pack.hpp"

$INCLUDES$

$NAMESPACE_START$
class $NAME$ : public cereal_pack::Schema {
    private:
        $PROPERTIES$
    public:
        $NAME$() = default;
        $NAME$($NAME$ &&) = default;

        explicit $NAME$(const void* buffer) {
            deserialize(buffer);
        }

        $NAME$& operator=(const $NAME$ &rhs) {
            if (this == &rhs) return *this;
            $ASSIGNMENT$
            return *this;
        }

        $NAME$(const $NAME$ &other) {
            *this = other;
        }

        $GETTERS$
    private:
        const std::vector<cereal_pack::Property*> m_properties = {
            $PROPERTY_PTRS$
        };
        virtual const std::vector<cereal_pack::Property*>& properties() const override {
            return m_properties;
        }
};
$NAMESPACE_END$

#endif //__$HEADERGUARD$__
