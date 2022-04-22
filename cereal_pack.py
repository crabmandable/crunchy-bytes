import argparse
from generator import *
import sys
import pathlib

def path_of_class(root, schema):
    return pathlib.Path(root, *schema.name_with_namespace.split('::')[:-1], schema.name + '.hpp')

if __name__ == '__main__':
    arg_parser = argparse.ArgumentParser(description='Generate cereal pack c++ classes from a list of schemas.')

    arg_parser.add_argument('--schemas', '-s', metavar='schema', nargs='+', required=True,
                        help='schema files to genearte classes from')

    arg_parser.add_argument('--output', '-o', metavar='output_dir', dest='out_dir', required=True,
                        help='output directory to place generated classes')

    arg_parser.add_argument('--globals', '-g', metavar='globals', dest='globals', required=False,
                        help='globals.toml file that defines global constants & enums')

    arg_parser.add_argument('--cmake', '-c', dest='cmake', required=False, default=False, action='store_true',
                        help='output the paths for the classes that will be generated, seperated with `;`')

    args = arg_parser.parse_args()

    schemas, globals = parser.load_schemas(args.schemas, args.globals)

    if args.cmake:
        outputs = list(map(lambda s: str(path_of_class(args.out_dir, s)), schemas.values()))
        sys.stdout.write(';'.join(outputs))
        sys.exit()

    pathlib.Path(args.out_dir).mkdir(exist_ok=True)

    if globals:
        with open(pathlib.Path(args.out_dir, 'cereal_pack_globals.hpp'), 'w') as file:
            file.write(generate.globals_header(globals))

    # todo write globals to file
    for s in schemas.values():
        schema_dir = path_of_class(args.out_dir, s).parent
        schema_dir.mkdir(parents=True, exist_ok=True)
        with open(schema_dir / (s.name + '.hpp'), 'w') as file:
            file.write(generate.header_file(s))
