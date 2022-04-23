def is_int(val):
    return type(val) == int or (type(val) == str and val.isnumeric())

