from os import path
import sys
sys.path.append(".")
import re
from collections import OrderedDict
from .templates import header_template, assignment_template, getter_template
from .property_types import property_types

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
            length_const = str(prop.max_items)
            if prop.reference:
                ref_class = property_types['reference']['class'].replace("$REFERENCE$", prop.reference.name_with_namespace)
            else:
                ref_class = class_for_prop(prop.item_prop)
            klass = klass.replace("$CLASS$", ref_class)
            length_const = str(prop.max_items)
        elif prop.type in ["string", "dynamic_length_buffer"]:
            length_const = str(prop.dict['max_length'])
        elif prop.type == "const_length_buffer":
            length_const = str(prop.max_length)
        elif prop.type == "reference":
            klass = klass.replace('$REFERENCE$', prop.reference.name_with_namespace)

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
    for p in schema.props:
        getters += getter_template.replace("$NAME$", p)
    template = template.replace("$GETTERS$", getters)

    # assignment
    assignements = []
    for p in schema.props:
        assignements.append(assignment_template.replace("$NAME$", p))
    template = replace_placeholder(template, 'ASSIGNMENT', assignements)

    # includes
    to_include = ['#include "{}.hpp"'.format(i.replace("::", "/")) for i in schema.references]
    template = replace_placeholder(template, 'INCLUDES', to_include)

    # constants
    constants = []
    for p in schema.props.values():
        if p.type == 'set':
            constants.append('static constexpr size_t {}_max_items = {};'.format(p.name, p.max_items))
            if p.item_prop and p.item_prop.length_constant:
                constants.append('static constexpr size_t {}_item_max_length = {};'.format(p.name, p.item_prop.length_constant))
        elif p.length_constant:
            constants.append('static constexpr size_t {}_max_length = {};'.format(p.name, p.length_constant))

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
