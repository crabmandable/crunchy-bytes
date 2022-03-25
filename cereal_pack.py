import argparse
from generator import *
import sys
import pathlib

if __name__ == '__main__':
    arg_parser = argparse.ArgumentParser(description='Generate cereal pack c++ classes from a list of schemas.')

    arg_parser.add_argument('--schemas', '-s', metavar='schema', nargs='+', required=True,
                        help='schema files to genearte classes from')

    arg_parser.add_argument('--output', '-o', metavar='output_dir', dest='out_dir', required=True,
                        help='output directory to place generated classes')

    args = arg_parser.parse_args()

    schemas = parser.load_schemas(args.schemas)
    pathlib.Path(args.out_dir).mkdir(exist_ok=True)

    for s in schemas.values():
        schema_dir = pathlib.Path(args.out_dir, *s.name_with_namespace.split('::')[:-1])
        schema_dir.mkdir(parents=True, exist_ok=True)
        with open(schema_dir / (s.name + '.hpp'), 'w') as file:
            file.write(generate.header_file(s))
