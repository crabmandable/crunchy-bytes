from os import path
import sys
sys.path.append(".")
import re
from collections import OrderedDict
from .templates import *
from .property_types import property_types

def _length_to_const(length):
    if type(length) == int:
        return str(length)
    elif type(length) == tuple:
        return "cereal_pack::globals::" + length[0]
    else:
        raise Exception("Invalid length: {}".format(length))

def globals_header(globals):
    template = globals_template
    global_defs = []
    if 'max_cereal_pack_serial_length' in globals:
        global_defs.append('constexpr uint32_t max_cereal_pack_serial_length = {};'.format(globals['max_cereal_pack_serial_length']))

    if 'lengths' in globals:
        for name, val in globals['lengths'].items():
            global_defs.append('constexpr uint32_t {} = {};'.format(name, val))

    template = replace_placeholder(template, 'GLOBALS', global_defs)
    return template

def header_file(schema):
    template = header_template
    # schema name
    template = template.replace("$NAME$", schema.name)
    template = template.replace("$HEADERGUARD$", schema.name_with_namespace.upper().replace("::", "_") + "_H")

    # namespace
    if schema.namespace:
        template = template.replace("$NAMESPACE_START$", "namespace " + schema.namespace + " {")
        template = template.replace("$NAMESPACE_END$", "};")
    else:
        template = template.replace("$NAMESPACE_START$", "")
        template = template.replace("$NAMESPACE_END$", "")

    # props
    # order the props
    ordered_props = OrderedDict()
    for item in schema.order:
        ordered_props[item] = schema.props[item]
    for k, item in schema.props.items():
        if k not in ordered_props:
            ordered_props[k] = item

    # property declerations
    def class_for_prop(prop):
        klass = property_types[prop.type]['class']
        length_const = None

        if prop.type == "set":
            length_const = _length_to_const(prop.max_items)
            if prop.reference:
                ref_class = property_types['reference']['class'].replace("$REFERENCE$", prop.reference.name_with_namespace)
            else:
                ref_class = class_for_prop(prop.item_prop)
            klass = klass.replace("$CLASS$", ref_class)
        elif prop.type in ["string", "dynamic_length_buffer"]:
            length_const = _length_to_const(prop.length_constant)
        elif prop.type == "const_length_buffer":
            length_const = _length_to_const(prop.length_constant)
        elif prop.type == "reference":
            klass = klass.replace('$REFERENCE$', prop.reference.name_with_namespace)
        elif prop.type == "enum":
            klass = klass.replace("$CLASS$", prop.dict["enum"])

        if length_const is not None:
            klass = klass.replace('$LENGTH$', length_const)

        return klass

    declarations = []
    for prop in schema.props.values():
        klass = class_for_prop(prop)
        declarations.append('{} m_{};'.format(klass, prop.name))

    template = replace_placeholder(template, 'PROPERTIES', declarations)

    #prop ptrs
    ptrs = map(lambda i: '&m_{},'.format(i.name), ordered_props.values())
    template = replace_placeholder(template, 'PROPERTY_PTRS', ptrs)

    # getters
    getters = ""
    for p, prop in schema.props.items():
        t = reference_getter_template if prop.type == "reference" else getter_template
        getters += t.replace("$NAME$", p)
    template = template.replace("$GETTERS$", getters)

    # assignment
    assignements = []
    for p in schema.props:
        assignements.append(assignment_template.replace("$NAME$", p))
    template = replace_placeholder(template, 'ASSIGNMENT', assignements)

    # equality
    equality = []
    for p in schema.props:
        equality.append(equality_template.replace("$NAME$", p))
    template = replace_placeholder(template, 'EQUALITY', equality)

    # includes
    to_include = ['#include "{}.hpp"'.format(i.replace("::", "/")) for i in schema.references]
    if schema.uses_globals():
        to_include.append('#include "cereal_pack_globals.hpp"')
    template = replace_placeholder(template, 'INCLUDES', to_include)

    # enums
    enums = []
    for enum_name, enum in schema.enums.items():
        e = enum_template.replace('$NAME$', enum_name)
        values = []
        for name, val in enum.items():
            values.append('{} = {},'.format(name, val))
        e = replace_placeholder(e, 'VALUES', values)
        enums.append(e)
    template = replace_placeholder(template, 'ENUMS', enums)

    # constants
    constants = []
    for p in schema.props.values():
        if p.type == 'set':
            constants.append('static constexpr size_t {}_max_items = {};'.format(p.name, _length_to_const(p.max_items)))
            if p.item_prop and p.item_prop.length_constant:
                constants.append('static constexpr size_t {}_item_max_length = {};'.format(p.name, _length_to_const(p.item_prop.length_constant)))
        elif p.length_constant:
            constants.append('static constexpr size_t {}_max_length = {};'.format(p.name, _length_to_const(p.length_constant)))

    constants.append('static constexpr size_t max_serial_length = {};'.format(schema.max_length()))
    template = replace_placeholder(template, 'CONSTANTS', constants)

    # remove empty lines
    template = re.compile("\s*\n\s*\n").sub("\n", template)

    return template

def replace_placeholder(template, placeholder, lines):
    line = re.search('^\s*\${}\$'.format(placeholder), template, re.MULTILINE).group(0)
    indent = re.search('\s*\$', line).group(0)[:-1]
    return template.replace('${}$'.format(placeholder), '\n{}'.format(indent).join(lines))

if __name__ == '__main__':
    if not sys.argv[1:]:
        raise Exception("no schema files provided")

    schemas = load_schemas(sys.argv[1:])

    for _, s in schemas.items():
        print(header_file(s))
