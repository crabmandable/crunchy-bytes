from .errors import CrunchyBytesException

def is_int(val):
    return type(val) == int or (type(val) == str and val.isnumeric())

def validate_uint32(file_path, val, err_desc):
    if type(val) != int:
        raise CrunchyBytesException(file_path, '{} is not an integer'.format(err_desc))
    if val < 0:
        raise CrunchyBytesException(file_path, '{} cannot be negative'.format(err_desc))
    if val > 0xFFFFFFFF:
        raise CrunchyBytesException(file_path, '{} exceeds UINT32_MAX '.format(err_desc))

def validate_enums(file_path, enums):
        if not isinstance(enums, dict):
            raise CrunchyBytesException(file_path, '"enums" should be a dictionary of enums where the key is the name of each enum')
        for name, enum in enums.items():
            if not isinstance(enum, dict):
                raise CrunchyBytesException(file_path, 'enum "{}" should be a dictionary'.format(name))

            for val_name, val in enum.items():
                err_pre = 'enum "{}.{}"'.format(name, val_name)
                if not is_int(val):
                    raise CrunchyBytesException(file_path, err_pre, 'value must be an integer')
                if int(val) < 0:
                    raise CrunchyBytesException(file_path, err_pre, 'value must be positive')

            if len(set(enum.values())) != len(enum.values()):
                raise CrunchyBytesException(file_path, 'enum "{}" values are not unique'.format(name))
            if 0 not in map(lambda v: int(v), enum.values()):
                raise CrunchyBytesException(file_path, 'enum "{}" must contain a `0` value, this is the default'.format(name))
