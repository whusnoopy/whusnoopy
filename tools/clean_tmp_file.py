#!/usr/bin/python
# -*- coding: utf-8 -*-

import os
import sys

def cleandir(dir, suffix):
  print 'start clean %s' % dir
  file_list = [f for f in os.listdir(dir) if os.path.isfile(os.path.join(dir,f))]
  file_list = [os.path.join(dir, f) for f in file_list if os.path.splitext(f)[1] in suffix]
  for f in file_list:
    print 'clean %s' % f
    os.remove(f)
  dir_list = [os.path.join(dir, f) for f in os.listdir(dir) if os.path.isdir(os.path.join(dir, f))]
  for d in dir_list:
    if not d.endswith('.svn'):
      cleandir(d, suffix)

def main():
  clean_suffix = ['.pyc', '.swp']
  root_dir = os.getcwd()
  cleandir(root_dir, clean_suffix)

if __name__ == '__main__':
  sys.exit(main())
