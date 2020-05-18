#! python3

import struct
from random import randint

f = open('nros2bytes.dat', 'wb')

for i in range(0, 10000):
  f.write(struct.pack('>H', randint(0, 65535)))
