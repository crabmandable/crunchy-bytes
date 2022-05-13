### Schemas
Schemas are defined by creating [TOML](https://toml.io/en/) documents.

Each provided `.toml` file will be converted into a C++ class that inherits from
`cereal_pack::Schema`.

The only required top level properties of a TOML schema are it's `name` and its
`props`.
* `name` should be a valid C++ class name
* `props` should be a dictionary of "propeties", where the dictionary key is the propety name

Here is a very simple example:
```toml
name = "OneBool"

[props]
    [props.boolean]
    type = "bool"
```

[Check out more examples here](./test/test-schemas)

### Property Types:
Each "prop" should be a valid `cereal_pack` property definition, including at a minimum a `type`

#### Primitives
There are quite a few `Primitive` types that will be very familiar to any C++ developer
```C++
bool
// integers
int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t
// floats
float, double
```
These types don't need any more definition outside the type, and will be represented as
`cereal_pack::Primitive<T>` in the generated C++ schema.

E.g.
```toml
[props.is_something]
type = "bool"

[props.num_things]
type = "uint32_t"
```

___
#### Variable length data
There are 2 types for byte arrays (think `std::vector<uint8_t>`),
`const_length_buffer` and `dynamic_length_buffer`, and a `string` type (think `std::string`).

##### const_length_buffer
`const_length_buffer` is always serialized as the same length, and it is useful for
data that is a fixed size, like a digest. It is not really encoded in any special
way and gets [de]serialized "as is".

It is represented by a `cereal_pack::ConstLengthBuffer<size_t>` in the generated schema

E.g.
```toml
[props.sha256_digest]
type = "const_length_buffer"
length = 32
```

___
##### dynamic_length_buffer
`dynamic_length_buffer` is a buffer than can vary in length, with a predefined
maximum length. It is serialized by writing it's length as a `uint32_t`, followed
by its contents.

It is represented by a `cereal_pack::DynamicLengthBuffer<size_t>` in the generated schema

E.g.
```toml
[props.signature]
type = "dynamic_length_buffer"
max_length = 72
```
___

In addition to binary data there is a `string` type
##### string
This should be pretty self explanatory.
It only supports UTF8, is represented by `cereal_pack::String<size_t>`,
and is encoded as a c-style null terminated string.

```toml
[props.name]
type = "string"
max_length = 255
```

___
#### Enum
An `enum` type shouuld be fairly self explanatory. It is a field that can hold
one of a discreet set of values, encoded as a `uint32_t`. The enum can either be
defined within the schema, or defined globally ([see cereal_pack globals](./GLOBALS.md)). The
enum should be defined separately to the property, and then set as the properties
"enum"

It is represented by a `cereal_pack::Primitive<T>` in the generated schema,
where T is the enum type.

E.g.
```toml
[enums]
[enums.grain_t]
# there MUST be a `0` value, and it will be used as the default
unknown = 0
oats = 1
wheat = 2
# the values do not have to be continuous
spelt = 5
# or in any particular order
barley = 4
einkorn = 6

[props]
[props.grain]
type = "enum"
enum = "grain_t"

# the same enum can be used for multiple properties
[props.grain_two]
type = "enum"
enum = "grain_t"
```
___
#### Reference
A `reference` type can _refer_ to any other schema. This is represented by a
`cereal_pack::Reference<T>`, where `T` is the schema being referenced.
The `reference` key provided must include the schema's namespace (if applicable).

E.g.
```toml
[props.reference_to_one_bool]
type = "reference"
reference = "OneBool"

[props.reference_to_something_with_namespace]
type = "reference"
reference = "crabmandable::MyCoolSchema"
```

___
#### Set
A `set` property is an array of any _other_ property type.
This could be a set of primitives, buffers, strings, or references. The `set`
must be given a `max_items` and should contain an `item` dictionary defining the
item's type.

It is not possible to make a set of sets.

E.g.
```toml
[props.set_of_bools]
type = "set"
max_items = 5
item = { type = "bool" }

[props.set_of_references]
type = "set"
max_items = 5
item = { type = "reference", reference = "OneBool" }

[props.set_of_buffers]
type = "set"
max_items = 5
item = { type = "const_length_buffer", length = 24 }
```

### Namespaces
To put a schema inside a namespace of your choosing, just set the the `namespace`
top level key for the schema. It may include nested namespaces.
```toml
namespace = "cereal_pack_testing::test"
```
Using a namespace will create a subdirectory with the same name, in the output directory
where the C++ classes are generated. This is done to avoid path conflicts

```C++
#include "my_schemas/cereal_pack_testing/test/Example.hpp"
cereal_pack_testing::test::Example e;
```

### Order
The order that properties will be [de]serialized in is determined by the order of
definition by default. The order can be set explicitly by providing an `order`
array with the property names.
```toml
order = ['property_one', 'property_three', 'property_two', 'property_four']
```

This doesn't have to include _every_ property, any omitted will just use the
default order _after_ all properties in the `order` array

