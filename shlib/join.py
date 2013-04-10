#!/usr/bin/python

import sys
import optparse


def main():
    parser = optparse.OptionParser(usage='%prog [OPTIONS] file1 file2')
    parser.add_option('-1', '--FIELD1', dest='f1',
            help='filed of file 1')
    parser.add_option('-2', '--FIELD2', dest='f2',
            help='filed of file 2')
    parser.add_option('-F', '--FIELD_SPERATOR', dest='sp',
            help='sperator of each field', default='\t')
    options, args = parser.parse_args()

    if not options.f1:
        parser.error("filed of file 1 must be provied")
    else:
        k1 = int(options.f1) - 1
    if not options.f2:
        parser.error("filed of file 2 must be provied")
    else:
        k2 = int(options.f2) - 1
    if not options.sp:
        sp = '\t'
    else:
        sp = options.sp

    file1 = file(args[0], 'r')

    f1d = {}
    while True:
        line = file1.readline()
        if not line:
            break
        cols = line[:-1].split(sp)
        key = cols[k1]
        selected_cols = []
        selected_cols.extend(cols[:k1])
        selected_cols.extend(cols[k1+1:])
        f1d[key] = selected_cols

    pass

    file1.close()

    file2 = file(args[1], 'r')

    while True:
        line = file2.readline()
        if not line:
            break
        cols = line[:-1].split(sp)
        key = cols[k2]
        if f1d.has_key(key):
            selected_cols = [key]
            selected_cols.extend(f1d[key])
            selected_cols.extend(cols[:k2])
            selected_cols.extend(cols[k2+1:])
            print sp.join(selected_cols)
    pass

    return 0


if __name__ == "__main__":
    sys.exit(main())

