# Cereal Pack 🥣

#### C++ Schema Serialization Library

![cereal-gif](./cereal_pack.gif)

## What is Cereal Pack?

`cereal_pack` is a library that serializes structured data.

You bring the data definitions, and `cereal_pack` will generate c++ classes that can
`serialize` and `deserialize` themselves into a compact binary representation that
can be easily be transferred between processes.

## Getting Started
### Requirements
`python3` and the `toml` package for python
```
pip install toml
```

### Schemas
Schemas are defined by creating [TOML](https://toml.io/en/) documents.

Each provided `.toml` file will be converted into a c++ class that inherits from
`cereal_pack::Schema`.

The only required top level properties of a TOML schema are it's `name` and its
`props`.
* `name` should be a valid c++ class name
* `props` should be a dictionary of "propeties", where the dictionary key is the propety name

Here is a very simple example:
```toml
name = "OneBool"

[props]
    [props.boolean]
    type = "bool"
```

[Check out more examples here](./test/test-schemas)

### Building
Once you have your schemas the process to generate your c++ classes is pretty
simple

#### Using CMake:
Add `cereal_pack` to your `CMakeLists.txt` as a `subdirectory`
and tell it where to find your `.toml` schemas, and where to place the generated
classes
```cmake
# Either set `CEREAL_PACK_SCHEMA_FILES` explicitly
set(CEREAL_PACK_SCHEMA_FILES
    "./my_schemas/schema_one.toml"
    "./my_schemas/schema_two.toml"
    )

# OR set `CEREAL_PACK_SCHEMA_DIR` which will recursively search the directory for .toml files
set(CEREAL_PACK_SCHEMA_DIR "./my-schemas")

# This will place the generated files in our build directory
set(CEREAL_PACK_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/my-schemas")

# add the `cereal_pack` subdirectory
add_subdirectory(${PATH_TO_CEREAL_PACK} cereal_pack_interface)
```
Then simply link `cereal_pack_interface` into your program
```cmake
target_link_libraries(my_program cereal_pack_interface)
```

The advantage of using cereal_pack's `CMakeLists.txt` is that any change to `cereal_pack`
or to your schemas will cause the generated c++ classes to be regenerated

[Check out the test suite CMakeLists.txt for another example](./test/CMakeLists.txt)

#### Manually:
First you need to use `cereal_pack.py` to generate the c++ classes
```sh
python cereal_pack.py -s ./my-schemas/* -o generated-classes/
```

Then simply include `cereal_pack` and the `generated-classes` in your build
```sh
g++ main.cpp -I./cereal_pack -I./generated-classes -o my_program
```

### Property Types:
Each "prop" should be a valid `cereal_pack` property definition, including at a minimum a `type`

#### Primitives
There are quite a few `Primitive` types that will be very familiar to any c++ developer
```c++
bool
// integers
int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t
// floats
float, double
```
These types don't need any more definition outside the type, and will be represented as
`cereal_pack::Primitive<T>` in the generated c++ schema.

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
#### Sets
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
where the c++ classes are generated. This is done to avoid path conflicts

```c++
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

## Usage
### TODO
### Why would anyone want to use this over ProtoBuf?

They probably wouldn't! :)

Seriously, [ProtoBuf](https://developers.google.com/protocol-buffers) is a very mature, and well tested library with many more features, and it's developed by freaking _Google_.
`cereal_pack` is a hobby project made by one dude. There is no way it can compete
in terms of features, or stability.

However, if you need something _small & simple_, for c++ _only_, then maybe `cereal_pack`
can work for you. Due to a slightly simpler design `cereal_pack` has a handful of advantages

#### It's tiny & embedded friendly
* The core library that supports the generated c++ classes is very small (~60Kb compiled)

* It's header only, including the generated classes

* It integrates _very_ easily into `CMake` projects

* There is no "compiler", just some python scripts that generate the c++ classes

#### Max sizes
Every `cereal_pack` schema has a _maximum_ size when serialized. Having a determinate
amount of memory required makes code safer & simpler.

#### MIT License
This permissive license means you can take this library and build on top of it
however you like.

