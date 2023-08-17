import sys

print(sys.path)

import fastset


s = fastset.Set()

print(s.add(0))
print(s.add(0))
print(s.add(1))

print(s.min(), s.max())

print(len(s))

print(s.min(), s.max())

print(s.pop_min(), s.pop_max())

print(len(s))

s.add(0)
s.add(1)
s.add(2)

print(s.next(), s.prev())

print(s.to_list())

print(s.get(), s.erase(), s[1])
print(s.pop(0))
print(s.index(1))
print(s)
