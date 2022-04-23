import toml
from . import schema
import re
from .errors import CerealPackException
from .property_types import length_length
from .validation import is_int

def load_toml(file_path):
    try:
        return toml.load(file_path)
    except toml.decoder.TomlDecodeError as e:
        raise CerealPackException(file_path, 'TOML decode error: {}'.format(str(e)))

def validate_uint32(file_path, val, err_desc):
    if type(val) != int:
        raise CerealPackException(file_path, '{} is not an integer'.format(err_desc))
    if val < 0:
        raise CerealPackException(file_path, '{} cannot be negative'.format(err_desc))
    if val > 0xFFFFFFFF:
        raise CerealPackException(file_path, '{} exceeds UINT32_MAX '.format(err_desc))

def parse_globals(file_path):
    raw = load_toml(file_path)

    if 'lengths' in raw:
        if not isinstance(raw['lengths'], dict):
            raise CerealPackException(file_path, 'expected "lengths" to be a dictionary')
        for name, val in raw['lengths'].items():
            validate_uint32(file_path, val, 'length ' + name)

    if 'max_cereal_pack_serial_length' in raw:
        validate_uint32(file_path, raw['max_cereal_pack_serial_length'], '"max_cereal_pack_serial_length"')

    return raw

def parse_schema(file_path, globals=None):
    raw = load_toml(file_path)

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


    # validate enums
    enums = {}
    if 'enums' in raw:
        enums = raw['enums']
        if not isinstance(enums, dict):
            raise CerealPackException(file_path, '"enums" should be a dictionary of enums where the key is the name of each enum')
        for name, enum in enums.items():
            if not isinstance(enum, dict):
                raise CerealPackException(file_path, 'enum "{}" should be a dictionary'.format(name))

            for val_name, val in enum.items():
                err_pre = 'enum "{}.{}"'.format(name, val_name)
                if not is_int(val):
                    raise CerealPackException(file_path, err_pre, 'value must be an integer')
                if int(val) < 0:
                    raise CerealPackException(file_path, err_pre, 'value must be positive')
            if len(set(enum.values())) != len(enum.values()):
                raise CerealPackException(file_path, err_pre, 'enum "{}" values are not unique'.format(name))
            if 0 not in map(lambda v: int(v), enum.values()):
                raise CerealPackException(file_path, err_pre, 'enum "{}" must contain a `0` value, this is the default'.format(name))

    return schema.Schema(file_path, raw['name'], enums, raw['props'], namespace, order, globals)

def load_schemas(files, globals_file=None):
    schemas = {}

    globals = parse_globals(globals_file) if globals_file else None
    for file in files:
        s = parse_schema(file, globals)
        if s.name_with_namespace in schemas:
            raise CerealPackException(s.file_path, 'Schema has same name as "{}"'.format(schemas[s.name_with_namespace].file_path))
        schemas[s.name_with_namespace] = s

    # resolve references
    for schema in schemas.values():
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
    for name, schema in schemas.items():
        try:
            resolve_max_length(schema)
        except RecursionError:
            raise CerealPackException(schema.file_path, 'unable to resolve references due to circular reference')

        if globals and 'max_cereal_pack_serial_length' in globals:
            if schema.max_length() > globals['max_cereal_pack_serial_length']:
                raise CerealPackException(
                    schema.file_path,
                    'schema exceeds max length defined in "{}": {} > {}'.format(globals_file, schema.max_length(), globals['max_cereal_pack_serial_length'])
                )

    if globals:
        return schemas, globals
    return schemas

if __name__ == '__main__':
    import sys
    schemas = load_schemas(sys.argv[1:])

    for _, s in schemas.items():
        print(s)
