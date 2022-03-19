#ifndef _ONEBOOL_HPP_
#define _ONEBOOL_HPP_
#include "cereal_pack.hpp"

/// generation example - hand written

class OneBool : public cereal_pack::Schema {
    private:
        cereal_pack::Primitive<bool> m_bool;
    public:
        OneBool() = default;
        OneBool(OneBool &&c) = default;

        explicit OneBool(const void* buffer) {
            deserialize(buffer);
        }

        OneBool& operator=( const OneBool &c ) {
            if (this == &c) return *this;
            m_bool = c.m_bool;
            return *this;
        }

        OneBool(const OneBool &c) {
            *this = c;
        }

        auto get_bool() -> decltype(m_bool)& {
            return m_bool;
        }

        auto get_bool() const -> const decltype(m_bool)& {
            return m_bool;
        }
    private:
        const std::vector<cereal_pack::Property*> m_properties = {
            &m_bool,
        };
        virtual const std::vector<cereal_pack::Property*>& properties() const override {
            return m_properties;
        }
};
#endif //_ONEBOOL_HPP_
