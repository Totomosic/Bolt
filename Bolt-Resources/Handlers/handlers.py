import sys
import os
import itertools

from Handlers.XML.XMLwriter import *

FILE_TYPE_UNKNOWN = 0
FILE_TYPE_IMAGE = 1

class ResourceHandler:
    def __init__(self, write_function, arguments):
        self.arguments = arguments
        self.write_function = write_function
        
RESOURCE_HANDLERS = {}
