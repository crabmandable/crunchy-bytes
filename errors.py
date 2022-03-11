class CerealPackException(Exception):
    def __init__(self, file_path, *errors):
        super().__init__('Error in schema file "{}": {}'.format(file_path, ' '.join(errors)))


