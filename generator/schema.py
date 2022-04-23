import re
from .errors import CerealPackException
from .property_types import property_types, length_length
from .validation import is_int

class Prop:
    def _is_global_length(self, val):
        global_lengths = self.globals['lengths'] if self.globals else {}
        return type(val) == str and val in global_lengths

    def _to_length_constant(self, length):
        if self._is_global_length(length):
            return length, self.globals['lengths'][length]
        else:
            return int(length)

    def _to_raw_length(self, length):
        if self._is_global_length(length):
            return self.globals['lengths'][length]
        else:
            return int(length)

    def uses_globals(self):
        if type(self.length_constant) == tuple or type(self.max_items) == tuple:
            return True

        if self.item_prop is not None:
            return type(self.item_prop.length_constant) == tuple or type(self.item_prop.max_items) == tuple

        return False

    def __init__(self, file_path, name, dict, enums, validate=True, globals=None):
        self.file_path = file_path
        self.dict = dict
        self.name = name
        self.reference = None
        self.max_length = None
        self.max_items = None
        self.item_prop = None
        self.length_constant = None
        self.globals = globals

        err_in = 'in property "{}":'.format(self.name)

        if validate:
            if re.match('^\w+$', name) is None:
                raise CerealPackException(file_path, err_in, '"name" of property must only use alphanumeric characters and underscore, "{}" given'.format(name))

            self.validate_prop(err_in, dict, enums)

        self.type = dict['type']
        property_type = property_types[self.type]

        if type(property_type['predefined_length']) == int:
            self.max_length = property_type['predefined_length']
        elif property_type['const_length']:
            self.max_length = self._to_raw_length(dict['length'])
            self.length_constant = self._to_length_constant(dict['length'])
        elif property_type['variable_length']:
            self.max_length = self._to_raw_length(dict['max_length']) + property_type['encoding_length']
            self.length_constant = self._to_length_constant(dict['max_length'])

        if self.type == 'reference':
            self.reference = dict['reference']

        if self.type == 'enum':
            # TODO globals
            self.enum = enums[dict['enum']]

        if self.type == 'set':
            self.max_items = self._to_length_constant(dict['max_items'])
            if dict['item']['type'] == 'reference':
                self.reference = dict['item']['reference']
            else:
                item_type = property_types[dict['item']['type']]
                if item_type['predefined_length']:
                    item_length = item_type['predefined_length']
                elif item_type['const_length']:
                    item_length = self._to_raw_length(dict['item']['length'])
                else:
                    item_length = self._to_raw_length(dict['item']['max_length']) + item_type['encoding_length']

                self.max_length = length_length + (self._to_raw_length(dict['max_items']) * item_length)
                self.item_prop = Prop(self.file_path, self.name, dict['item'], enums, validate=False, globals=globals)

        # ensure max length was set unless the property contains a reference
        if self.max_length is None and self.reference is None:
            raise CerealPackException(file_path, err_in, 'unable to determine max length of property')

    def validate_prop(self, err_pre, prop_dict, enums):
        # type
        if 'type' not in prop_dict:
            raise CerealPackException(self.file_path, err_pre, '"type" not defined')
        if prop_dict['type'] not in property_types:
            raise CerealPackException(self.file_path, err_pre, 'unknown property type "{}"'.format(prop_dict['type']))

        type_to_validate = prop_dict['type']
        property_type = property_types[type_to_validate]

        if type_to_validate == 'enum':
            if 'enum' not in prop_dict:
                raise CerealPackException(self.file_path, err_pre, '"enum" key must contain an "enum"')
            if type(prop_dict['enum']) != str:
                raise CerealPackException(self.file_path, err_pre, '"enum" key must be the name of an enum')
            if prop_dict['enum'] not in enums:
                raise CerealPackException(self.file_path, err_pre, '"enum" key not found')

        # reference
        if type_to_validate == 'reference' and 'reference' not in prop_dict:
            raise CerealPackException(self.file_path, err_pre, 'reference property must contain a "reference"')

        # set
        if type_to_validate == 'set':
            # item
            if 'item' not in prop_dict:
                raise CerealPackException(self.file_path, err_pre, 'set property must contain an "item"')

            # max_items
            if 'max_items' not in prop_dict:
                raise CerealPackException(self.file_path, err_pre, 'set property must contain a "max_items"')
            if type(prop_dict['max_items']) is str:
                if not self._is_global_length(prop_dict['max_items']):
                    raise CerealPackException(self.file_path, err_pre, 'set property "max_items" is an unknown name. Expected a global length name')
            elif not is_int(prop_dict['max_items']):
                raise CerealPackException(self.file_path, err_pre, 'set property must contain an integer "max_items" or a string name of a global length')

            # don't allow a set to contain a set
            if 'type' in prop_dict['item'] and prop_dict['item']['type'] == 'set':
                raise CerealPackException(self.file_path, err_pre, 'item of set property cannot be of type "set"')

            # validate item
            self.validate_prop(err_pre + ' in "item" definition:', prop_dict['item'], enums)

        # length
        if type(property_type['predefined_length']) != int and property_type['const_length']:
            if 'length' not in prop_dict:
                raise CerealPackException(self.file_path, err_pre, '{} property must contain a "length"'.format(type_to_validate))
            if type(prop_dict['length']) is str:
                if not self._is_global_length(prop_dict['length']):
                    raise CerealPackException(self.file_path, err_pre, '{} property "length" is an unknown name. Expected a global length name'.format(type_to_validate))
            elif not is_int(prop_dict['length']):
                raise CerealPackException(self.file_path, err_pre, '{} property must contain an integer "length" or a string name of a global length'.format(type_to_validate))

        # max_length
        if property_type['variable_length']:
            if 'max_length' not in prop_dict:
                raise CerealPackException(self.file_path, err_pre, '{} property must contain a "max_length"'.format(type_to_validate))
            if not is_int(prop_dict['max_length']):
                if not self._is_global_length(prop_dict['max_length']):
                    raise CerealPackException(self.file_path, err_pre, '{} property must contain an integer "max_length"'.format(type_to_validate))

    def __str__(self):
        return str(self.dict)

class Schema:
    def uses_globals(self):
        return any(map(lambda p: p.uses_globals(), self.props.values()))

    def __init__(self, file_path, name, enums, props, namespace=None, order=[], globals=None):
        self.order = order
        self.file_path = file_path
        self.name = name
        self.namespace = namespace
        self.enums = enums

        if re.match('^\w+$', name) is None:
            raise CerealPackException(file_path,
                    '"name" must only contain alphanumeric and underscore charchters')

        if namespace:
            if re.match('^\w+(::\w+)*$', namespace) is None:
                raise CerealPackException(file_path,
                        '"namespace" must only contain alphanumeric and underscore charchters, and "::"')

            self.name_with_namespace = namespace + "::" + name
        else:
            self.name_with_namespace = name

        self.props = {p: Prop(file_path, p, props[p], enums, globals=globals) for p in props}
        self.references = set([p.reference for _, p in self.props.items() if p.reference is not None])

        for o in self.order:
            if o not in props:
                raise CerealPackException(file_path,
                        'order array contains "{}" which is not a property in this schema'.format(o))

        if len(self.order) != len(set(self.order)):
            raise CerealPackException(file_path, 'order array contains duplicates'.format(o))

    def max_length(self):
        if None in [p.max_length for _, p in self.props.items()]:
            return None
        return sum([p.max_length for _, p in self.props.items()])

    def __str__(self):
        sep = ":\n  "
        return self.name_with_namespace + sep + sep.join([name + ": " + str(p) for name, p in self.props.items()])
