property_types = {
        'bool': {
            'const_length': True,
            'predefined_length': 1,
            'variable_length': False,
            'class': 'crunchy_bytes::Primitive<bool>'
            },

        'int8_t': {
            'const_length': True,
            'predefined_length': 1,
            'variable_length': False,
            'class': 'crunchy_bytes::Primitive<int8_t>'
            },

        'uint8_t': {
            'const_length': True,
            'predefined_length': 1,
            'variable_length': False,
            'class': 'crunchy_bytes::Primitive<uint8_t>'
            },

        'int16_t': {
            'const_length': True,
            'predefined_length': 2,
            'variable_length': False,
            'class': 'crunchy_bytes::Primitive<int16_t>'
            },

        'uint16_t': {
            'const_length': True,
            'predefined_length': 2,
            'variable_length': False,
            'class': 'crunchy_bytes::Primitive<uint16_t>'
            },

        'int32_t': {
            'const_length': True,
            'predefined_length': 4,
            'variable_length': False,
            'class': 'crunchy_bytes::Primitive<int32_t>'
            },

        'uint32_t': {
            'const_length': True,
            'predefined_length': 4,
            'variable_length': False,
            'class': 'crunchy_bytes::Primitive<uint32_t>'
            },

        'int64_t': {
                'const_length': True,
                'predefined_length': 8,
                'variable_length': False,
                'class': 'crunchy_bytes::Primitive<int64_t>'
                },

        'uint64_t': {
                'const_length': True,
                'predefined_length': 8,
                'variable_length': False,
                'class': 'crunchy_bytes::Primitive<uint64_t>'
                },

        'float': {
            'const_length': True,
            'predefined_length': 4,
            'variable_length': False,
            'class': 'crunchy_bytes::Primitive<float>'
            },

        'double': {
            'const_length': True,
            'predefined_length': 8,
            'variable_length': False,
            'class': 'crunchy_bytes::Primitive<double>'
            },


        'const_length_buffer': {
                'const_length': True,
                'predefined_length': False,
                'variable_length': False,
                'class': 'crunchy_bytes::ConstLengthBuffer<$LENGTH$>'
                },

        'dynamic_length_buffer': {
                'const_length': False,
                'predefined_length': False,
                'variable_length': True,
                'encoding_length': 4,
                'class': 'crunchy_bytes::DynamicLengthBuffer<$LENGTH$>'
                },

        'string': {
                'const_length': False,
                'predefined_length': False,
                'variable_length': True,
                'encoding_length': 1,
                'class': 'crunchy_bytes::String<$LENGTH$>'
                },

        'reference': {
                'const_length': False,
                'predefined_length': False,
                'variable_length': False,
                'class': 'crunchy_bytes::Reference<$REFERENCE$>'
                },

        'set': {
                'const_length': False,
                'predefined_length': False,
                'variable_length': False,
                'class': 'crunchy_bytes::Set<$CLASS$, $LENGTH$>'
                },

        'enum': {
                'const_length': True,
                'predefined_length': 4,
                'variable_length': False,
                'class': 'crunchy_bytes::Primitive<$CLASS$>'
                },
}

length_length = 4
