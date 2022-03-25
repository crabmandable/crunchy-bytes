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
    declare = ""
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

    for prop in schema.props.values():
        klass = class_for_prop(prop)

        declare += """
    $CLASS$ m_$NAME$;
        """.replace("$CLASS$", klass).replace("$NAME$", prop.name)

    template = template.replace("$PROPERTIES$", declare)

    #prop ptrs
    ptrs = ", ".join(map(lambda i: "&m_" + i.name, ordered_props.values()))
    template = template.replace("$PROPERTY_PTRS$", ptrs)

    # getters
    getters = ""
    for p in schema.props:
        getters += getter_template.replace("$NAME$", p)
    template = template.replace("$GETTERS$", getters)

    # assignment
    assignements = ""
    for p in schema.props:
        assignements += assignment_template.replace("$NAME$", p)
    template = template.replace("$ASSIGNMENT$", assignements)

    #includes
    to_include = [i.replace("::", "/") for i in schema.references]
    includes = "\n".join(map(lambda i: "#include \"" + i + ".hpp\"", to_include))
    template = template.replace("$INCLUDES$", includes)


    template = re.compile("\s*\n\s*\n").sub("\n", template)
    return template


if __name__ == '__main__':
    if not sys.argv[1:]:
        raise Exception("no schema files provided")

    schemas = load_schemas(sys.argv[1:])

    for _, s in schemas.items():
        print(header_file(s))
