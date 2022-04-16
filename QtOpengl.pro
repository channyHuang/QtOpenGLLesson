TEMPLATE = subdirs

SOURCE_DIR = $$files($${PWD}/*_*, false)

SUBDIRS += $$SOURCE_DIR
