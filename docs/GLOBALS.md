### Globals
The generated code will _always_ include a file called `cereal_pack_globals.hpp`.

This will include the namespace `cereal_pack::globals`, containing constants &
definitions. By default the only thing in here will be `max_cereal_pack_serial_length`,
but you can add to this by supplying a global definitions file.

Globals can also be used to avoid repeating constant & enum definitions in multiple
schema files.

To modify/add to this namespace you can supply global definitions as a `.toml` file

#### Supplying global definitions
Sometimes lengths or enums might need to be shared between multiple schemas. In
order to facilitate this you can supply a file with global definitions
that can be referenced in your schemas.

##### Using CMake
You can define a globals file to use by setting the cmake variable `CEREAL_PACK_GLOBALS`
```cmake
set(CEREAL_PACK_GLOBALS "${CMAKE_CURRENT_SOURCE_DIR}/cereal_pack_globals.toml")
```

##### Manually
The globals file can be passed to `cereal_pack.py` with the `--globals` or `-g` option
```sh
python cereal_pack.py -g ./my_globals.toml -s ./my-schemas/* -o generated-classes/
```

The file can contain the keys:
* `max_cereal_pack_serial_length`

    This is a `uint32_t` that will be directly added to the `cereal_pack::globals`
    namespace, and will will enforce that _no_ schema ever exceeds this length
    when serialized (an error will be thrown during code generation).

    If you do not supply this feild, it will still appear in the `cereal_pack::gloabls`
    namespace, with the maximum length that any of your schemas can be when serialized.

* `lengths`

    This should be a dictionary of `uint32_t` constants that can be used to define
    `max_length`, `length`, or `max_items` of schema properties

* `enums`

    This should be a dictionary of enum definitions that can be used by `enum`
    properties

E.g.
```toml
max_cereal_pack_serial_length = 8192

[lengths]
max_number_of_marshmallows = 100

[enums]
    [enums.marshmallow_color]
    pink = 0
    blue = 1
```
