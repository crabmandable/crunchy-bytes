### Globals
The generated code will _always_ include a file called `crunchy_byte_globals.hpp`.

This will include the namespace `crunchy_bytes::globals`, containing constants &
definitions. By default the only thing in here will be `max_crunchy_bytes_serial_length`,
but you can add to this by supplying a global definitions file.

Globals can also be used to avoid repeating constant & enum definitions in multiple
schema files.

To modify/add to this namespace you can supply global definitions as a `.toml` file

#### Supplying global definitions
Sometimes lengths or enums might need to be shared between multiple schemas. In
order to facilitate this you can supply a file with global definitions
that can be referenced in your schemas.

##### Using CMake
You can define a globals file to use by setting the cmake variable `CRUNCHY_BYTE_GLOBALS`
```cmake
set(CRUNCHY_BYTE_GLOBALS "${CMAKE_CURRENT_SOURCE_DIR}/crunchy_byte_globals.toml")
```

##### Manually
The globals file can be passed to `crunchy_bytes.py` with the `--globals` or `-g` option
```sh
python crunchy_bytes.py -g ./my_globals.toml -s ./my-schemas/* -o generated-classes/
```

The file can contain the keys:
* `max_crunchy_bytes_serial_length`

    This is a `uint32_t` that will be directly added to the `crunchy_bytes::globals`
    namespace, and will will enforce that _no_ schema ever exceeds this length
    when serialized (an error will be thrown during code generation).

    If you do not supply this feild, it will still appear in the `crunchy_bytes::gloabls`
    namespace, with the maximum length that any of your schemas can be when serialized.

* `lengths`

    This should be a dictionary of `uint32_t` constants that can be used to define
    `max_length`, `length`, or `max_items` of schema properties

* `enums`

    This should be a dictionary of enum definitions that can be used by `enum`
    properties

E.g.
```toml
max_crunchy_bytes_serial_length = 8192

[lengths]
max_number_of_marshmallows = 100

[enums]
    [enums.marshmallow_color]
    pink = 0
    blue = 1
```
