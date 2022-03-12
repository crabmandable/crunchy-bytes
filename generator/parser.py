import toml
import schema
import re
from errors import CerealPackException
from property_types import length_length

def parse(file_path):
    raw = toml.load(file_path)
    if 'name' not in raw:
        raise CerealPackException(file_path, 'top level field "name" not found')

    if not isinstance(raw['name'], str):
        raise CerealPackException(file_path, 'top level field "name" is not a string')

    if 'props' not in raw:
        raise CerealPackException(file_path, 'top level field "props" not found')

    if not isinstance(raw['props'], dict):
        raise CerealPackException(file_path, 'top level field "props" is not a dictionary')

    if 'namespace' in raw and not isinstance(raw['namespace'], str):
        raise CerealPackException(file_path, 'top level field "namespace" is not a string')

    if 'order' in raw and not isinstance(raw['order'], list):
        raise CerealPackException(file_path, 'top level field "order" is not an array')

    name = raw['name']
    props = raw['props']

    namespace = None
    if 'namespace' in raw:
        namespace = raw['namespace']

    order = []
    if 'order' in raw:
        order = raw['order']
        for o in order:
            if not isinstance(o, str):
                raise CerealPackException(file_path, 'top level property "order" should be an array of property names')

    return schema.Schema(file_path, raw['name'], raw['props'], namespace, order)

def load_schemas(*files):
    schemas = {}

    for file in files:
        s = parse(file)
        schemas[s.name_with_namespace] = s

    # resolve references
    for _, schema in schemas.items():
        for ref_name in schema.references:
            if ref_name not in schemas:
                raise CerealPackException(schema.file_path, 'unable to resolve reference to "{}"'.format(ref_name))

            reference = schemas[ref_name]

            for name, prop in schema.props.items():
                if prop.reference == ref_name:
                    prop.reference = reference


    # recursive function used to resolve max_length of references
    def resolve_max_length(schema):
        for _, prop in schema.props.items():
            if prop.reference is not None:
                if prop.reference.max_length() is None:
                    len = resolve_max_length(prop.reference)
                else:
                    len = prop.reference.max_length()

                if prop.type == 'set':
                    prop.max_length = length_length + (prop.max_items * len)
                else:
                    prop.max_length = len

        return schema.max_length()

    # update max_length of references
    for _, schema in schemas.items():
        try:
            resolve_max_length(schema)
        except RecursionError:
            raise CerealPackException(schema.file_path, 'unable to resolve references due to circular reference')

    return schemas

if __name__ == '__main__':
    import sys
    schemas = load_schemas(*sys.argv[1:])

    for _, s in schemas.items():
        print(s)
