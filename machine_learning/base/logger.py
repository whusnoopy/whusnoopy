#!/usr/bin/python
# -*- coding: utf-8 -*-
# Copyright 2011 yewen

import logging

def LOGGER(filename='', level='INFO') :
  log = logging.getLogger(__name__)
  log.setLevel(getattr(logging, "%s" % level))
  formatter = logging.Formatter('%(asctime)s [%(levelname)s] %(filename)s:%(lineno)d: %(message)s', '%y-%m-%d,%H:%M:%S')

# Console Logger
  ch = logging.StreamHandler()
  # ch.setLevel(logging.ERROR)
  ch.setFormatter(formatter)
  log.addHandler(ch)

# If log to file
  if len(filename) > 0 :
    fh = logging.FileHandler(filename)
    fh.setLevel(logging.INFO)
    fh.setFormatter(formatter)
    log.addHandler(fh)

  return log

if __name__ == '__main__' :
  LOG = LOGGER()
  LOG.info("Hello, this is a logger module")
  
