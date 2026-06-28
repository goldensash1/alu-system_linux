#include <Python.h>
#include <stdio.h>

/**
 * print_python_string - Prints basic info about a Python string object
 * @p: pointer to the PyObject (expected to be a PyUnicodeObject)
 *
 * Return: nothing
 */
void print_python_string(PyObject *p)
{
	Py_ssize_t length;

	setbuf(stdout, NULL);
	printf("[.] string object info\n");

	if (!PyUnicode_Check(p))
	{
		printf("  [ERROR] Invalid String Object\n");
		return;
	}

	length = ((PyASCIIObject *)p)->length;

	if (PyUnicode_IS_COMPACT_ASCII(p))
		printf("  type: compact ascii\n");
	else
		printf("  type: compact unicode object\n");

	printf("  length: %ld\n", length);
	printf("  value: %s\n", (char *)PyUnicode_AsUTF8(p));
}
