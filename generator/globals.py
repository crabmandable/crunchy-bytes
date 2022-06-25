class Globals:
    enums = {}
    lengths = {}
    max_crunchy_bytes_serial_length = None

    def __init__(self, globals_dict):
        if not globals_dict:
            return
        self.lengths = globals_dict['lengths']
        self.enums = globals_dict['enums']
        self.max_crunchy_bytes_serial_length = globals_dict['max_crunchy_bytes_serial_length']
