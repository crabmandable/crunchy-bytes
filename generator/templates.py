header_template = """
#ifndef __$HEADERGUARD$__
#define __$HEADERGUARD$__

#include <vector>
#include "crunchy_bytes/crunchy_bytes.hpp"

$INCLUDES$

$NAMESPACE_START$
class $NAME$ : public crunchy_bytes::Schema {
    public:
        struct constants {
            static constexpr const char * schema_name = "$NAME_WITH_NAMESPACE$";
            $CONSTANTS$
            private:
                constants();
        };
        $ENUMS$
    private:
        $PROPERTIES$
    public:
        $NAME$() = default;
        $NAME$($NAME$ &&) = default;

        const char * const schema_name() const override {
            return constants::schema_name;
        };

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


        bool operator==(const $NAME$ &rhs) const {
            $EQUALITY$
            return true;
        }

        bool operator!=(const $NAME$ &rhs) const {
            return !(*this == rhs);
        }

        $GETTERS$
    private:
        const std::vector<crunchy_bytes::Property*> m_properties = {
            $PROPERTY_PTRS$
        };
        virtual const std::vector<crunchy_bytes::Property*>& properties() const override {
            return m_properties;
        }
};
$NAMESPACE_END$

#endif //__$HEADERGUARD$__
"""

getter_template = """
        auto $NAME$() -> decltype(m_$NAME$)& {
            return m_$NAME$;
        }

        auto $NAME$() const -> const decltype(m_$NAME$)& {
            return m_$NAME$;
        }
"""

reference_getter_template = """
        auto $NAME$() -> decltype(m_$NAME$.get())& {
            return m_$NAME$.get();
        }

        auto $NAME$() const -> decltype(m_$NAME$.get())& {
            return m_$NAME$.get();
        }
"""

assignment_template = 'm_$NAME$ = rhs.m_$NAME$;'

equality_template = 'if (m_$NAME$ != rhs.m_$NAME$) return false;'

enum_template = """
        enum class $NAME$ : uint32_t {
            $VALUES$
        };
"""

globals_template = """
#ifndef __CRUNCHY_BYTES_GLOBALS_H__
#define __CRUNCHY_BYTES_GLOBALS_H__
namespace crunchy_bytes {
    namespace globals {
        $GLOBALS$
    };
};
#endif // __CRUNCHY_BYTES_GLOBALS_H__
"""
