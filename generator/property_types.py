property_types = {
        'bool': {
            'const_length': True,
            'predefined_length': 1,
            'variable_length': False,
            'class': 'cereal_pack::Primitive<bool>'
            },

        'int8_t': {
            'const_length': True,
            'predefined_length': 1,
            'variable_length': False,
            'class': 'cereal_pack::Primitive<int8_t>'
            },

        'uint8_t': {
            'const_length': True,
            'predefined_length': 1,
            'variable_length': False,
            'class': 'cereal_pack::Primitive<uint8_t>'
            },

        'int16_t': {
            'const_length': True,
            'predefined_length': 2,
            'variable_length': False,
            'class': 'cereal_pack::Primitive<int16_t>'
            },

        'uint16_t': {
            'const_length': True,
            'predefined_length': 2,
            'variable_length': False,
            'class': 'cereal_pack::Primitive<uint16_t>'
            },

        'int32_t': {
            'const_length': True,
            'predefined_length': 4,
            'variable_length': False,
            'class': 'cereal_pack::Primitive<int32_t>'
            },

        'uint32_t': {
            'const_length': True,
            'predefined_length': 4,
            'variable_length': False,
            'class': 'cereal_pack::Primitive<uint32_t>'
            },

        'int64_t': {
                'const_length': True,
                'predefined_length': 8,
                'variable_length': False,
                'class': 'cereal_pack::Primitive<int64_t>'
                },

        'uint64_t': {
                'const_length': True,
                'predefined_length': 8,
                'variable_length': False,
                'class': 'cereal_pack::Primitive<uint64_t>'
                },

        'float': {
            'const_length': True,
            'predefined_length': 4,
            'variable_length': False,
            'class': 'cereal_pack::Primitive<float>'
            },

        'double': {
            'const_length': True,
            'predefined_length': 8,
            'variable_length': False,
            'class': 'cereal_pack::Primitive<double>'
            },


        'const_length_buffer': {
                'const_length': True,
                'predefined_length': False,
                'variable_length': False,
                'class': 'cereal_pack::ConstLengthBuffer<$LENGTH$>'
                },

        'dynamic_length_buffer': {
                'const_length': False,
                'predefined_length': False,
                'variable_length': True,
                'encoding_length': 4,
                'class': 'cereal_pack::DynamicLengthBuffer<$LENGTH$>'
                },

        'string': {
                'const_length': False,
                'predefined_length': False,
                'variable_length': True,
                'encoding_length': 1,
                'class': 'cereal_pack::String<$LENGTH$>'
                },

        'reference': {
                'const_length': False,
                'predefined_length': False,
                'variable_length': False,
                'class': 'cereal_pack::Reference<$REFERENCE$>'
                },

        'set': {
                'const_length': False,
                'predefined_length': False,
                'variable_length': False,
                'class': 'cereal_pack::Set<$CLASS$, $LENGTH$>'
                },

        'enum': {
                'const_length': True,
                'predefined_length': 4,
                'variable_length': False,
                'class': 'cereal_pack::Primitive<$CLASS$>'
                },
}

length_length = 4
