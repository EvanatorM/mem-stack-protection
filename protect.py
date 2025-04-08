import sys
import os
from pwn import *

proc = gdb.debug([os.getcwd() + '/protect'])