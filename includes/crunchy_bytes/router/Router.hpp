#ifndef _CRUNCHY_BYTES_ROUTER_HPP_
#define _CRUNCHY_BYTES_ROUTER_HPP_
#include <unordered_map>
#include <functional>
#include <stdint.h>
#include <string>

namespace crunchy_bytes {
    namespace router {
        template<class Header, typename SchemaKey=std::string>
        class Router {
            public:

                Router(std::function<SchemaKey(const Header&)> ingeset_header) :
                    ingeset_header(ingeset_header) {}

                template <class T>
                bool attatch_route(SchemaKey key, std::function<void(const T&)> callback) {
                    return m_handlers.insert({
                        key,
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
                    auto key = ingeset_header(header);
                    if (m_handlers.find(key) == m_handlers.end()) {
                        return false;
                    }
                    auto handler = m_handlers[key];
                    handler(data + header.serial_length());
                    return true;
                }
            protected:
                using hander_t = std::function<void(uint8_t*)>;
                std::unordered_map<SchemaKey, hander_t> m_handlers;
                std::function<SchemaKey(Header)> ingeset_header;
        };

        template<class Header>
        class BasicRouter : public Router<Header, std::string> {
            public:
                BasicRouter(std::function<std::string(Header)> ingeset_header) :
                    Router<Header>(ingeset_header) {}

                template <class T>
                bool attatch_route(std::function<void(const T&)> callback) {
                    return this->m_handlers.insert({
                        T::constants::schema_name,
                        [callback](uint8_t* data) {
                            T schema;
                            schema.deserialize(data);
                            callback(schema);
                        }
                    }).second;
                }

        };
    };
};
#endif //_CRUNCHY_BYTES_ROUTER_HPP_
