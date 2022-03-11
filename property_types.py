property_types = {
        'bool': {'const_length': True, 'predefined_length': 1, 'variable_length': False},
        'int8': {'const_length': True, 'predefined_length': 1, 'variable_length': False},
        'uint8': {'const_length': True, 'predefined_length': 1, 'variable_length': False},
        'int16': {'const_length': True, 'predefined_length': 2, 'variable_length': False},
        'uint16': {'const_length': True, 'predefined_length': 2, 'variable_length': False},
        'int32': {'const_length': True, 'predefined_length': 4, 'variable_length': False},
        'uint32': {'const_length': True, 'predefined_length': 4, 'variable_length': False},
        'int64': {'const_length': True, 'predefined_length': 8, 'variable_length': False},
        'uint64': {'const_length': True, 'predefined_length': 8, 'variable_length': False},
        'const_length_buffer': {'const_length': True, 'predefined_length': False, 'variable_length': False},
        'dynamic_length_buffer': {'const_length': False, 'predefined_length': False, 'variable_length': True, 'encoding_length': 4},
        'string': {'const_length': False, 'predefined_length': False, 'variable_length': True, 'encoding_length': 1},
        'reference': {'const_length': False, 'predefined_length': False, 'variable_length': False},
        'set': {'const_length': False, 'predefined_length': False, 'variable_length': False},
        }

length_length = 4
