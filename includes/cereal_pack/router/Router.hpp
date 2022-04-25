#ifndef _CEREAL_PACK_ROUTER_HPP_
#define _CEREAL_PACK_ROUTER_HPP_
#include <unordered_map>
#include <functional>
#include <stdint.h>
#include <string>

namespace cereal_pack {
    namespace router {
        struct IngestedHeader {
            uint32_t header_length;
            std::string schema_name;

            IngestedHeader(uint32_t header_length, std::string schema_name) :
                header_length(std::move(header_length)), schema_name(std::move(schema_name))
            {}
        };

        template<class Header>
        class Router {
            public:
                Router(std::function<IngestedHeader(Header)> ingeset_header) :
                    ingeset_header(ingeset_header) {}

                template <class T>
                bool attatch_route(std::function<void(const T&)> callback) {
                    return m_handlers.insert({
                        T::constants::schema_name,
                        [callback](uint8_t* data) {
                            T schema;
                            schema.deserialize(data);
                            callback(schema);
                        }
                    }).second;
                }
                bool handle_message(uint8_t* data) {
                    Header header;
                    header.deserialize(data);
                    auto ingested = ingeset_header(header);
                    if (m_handlers.find(ingested.schema_name) == m_handlers.end()) {
                        return false;
                    }
                    auto handler = m_handlers[ingested.schema_name];
                    handler(data + ingested.header_length);
                    return true;
                }
            private:
                using hander_t = std::function<void(uint8_t*)>;
                std::unordered_map<std::string, hander_t> m_handlers;
                std::function<IngestedHeader(Header)> ingeset_header;
        };
    };
};
#endif //_CEREAL_PACK_ROUTER_HPP_
