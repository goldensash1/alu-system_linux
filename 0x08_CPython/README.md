# 0x08. CPython

CPython is the reference implementation of the Python programming language,
written in C. This project peeks under the hood of CPython by writing C
functions that inspect the internal representation of common Python objects
(lists, bytes, floats, strings and ints) and prints information about them.

The C functions are compiled into a shared library (`.so`) and called from
Python through the `ctypes` module.

## Environment

* Python version: **3.4**
* OS: **Ubuntu 14.04 LTS**
* Allowed editors: `vi`, `vim`, `emacs`
* All files end with a new line

## Files

| File | Description |
| --- | --- |
| `0-python_lists_bigO` | Big O notations for common Python list operations |
| `1-python.c` | `print_python_list` — basic info about a Python list |
| `2-python.c` | `print_python_list` + `print_python_bytes` |
| `3-python.c` | `print_python_list` + `print_python_bytes` + `print_python_float` |
| `4-python.c` | `print_python_string` — info about a Python string |
| `5-python.c` | `print_python_int` — prints a Python integer value |

## Compilation

Each C file builds into a shared library. Examples:

```
gcc -Wall -Werror -Wextra -pedantic -std=c99 -shared -Wl,-soname,PyList \
    -o libPyList.so -fPIC -I/usr/include/python3.4 1-python.c

gcc -Wall -Werror -Wextra -pedantic -std=c99 -shared -Wl,-soname,libPython.so \
    -o libPython.so -fPIC -I/usr/include/python3.4 2-python.c

gcc -Wall -Werror -Wextra -pedantic -std=c99 -shared -Wl,-soname,libPython.so \
    -o libPython.so -fPIC -I/usr/include/python3.4 3-python.c

gcc -Wall -Wextra -pedantic -Werror -std=c99 -shared -Wl,-soname,libPython.so \
    -o libPython.so -fPIC -I/usr/include/python3.4 4-python.c

gcc -Wall -Wextra -pedantic -Werror -std=c99 -shared -Wl,-soname,libPython.so \
    -o libPython.so -fPIC -I/usr/include/python3.4 5-python.c
```

## Usage

The shared library is loaded from Python with `ctypes`:

```python
import ctypes

lib = ctypes.CDLL('./libPython.so')
lib.print_python_list.argtypes = [ctypes.py_object]
l = ['hello', 'World']
lib.print_python_list(l)
```

## Author

Golden Sash
