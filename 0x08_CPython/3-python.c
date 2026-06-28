#include <Python.h>
#include <stdio.h>

void print_python_list(PyObject *p);
void print_python_bytes(PyObject *p);
void print_python_float(PyObject *p);

/**
 * print_python_bytes - Prints basic info about a Python bytes object
 * @p: pointer to the PyObject (expected to be a PyBytesObject)
 *
 * Return: nothing
 */
void print_python_bytes(PyObject *p)
{
	Py_ssize_t size, i, limit;
	PyBytesObject *bytes;

	setbuf(stdout, NULL);
	printf("[.] bytes object info\n");

	if (!PyBytes_Check(p))
	{
		printf("  [ERROR] Invalid Bytes Object\n");
		return;
	}

	bytes = (PyBytesObject *)p;
	size = ((PyVarObject *)p)->ob_size;

	printf("  size: %ld\n", size);
	printf("  trying string: %s\n", bytes->ob_sval);

	if (size + 1 < 10)
		limit = size + 1;
	else
		limit = 10;

	printf("  first %ld bytes:", limit);
	for (i = 0; i < limit; i++)
		printf(" %02hhx", bytes->ob_sval[i]);
	printf("\n");
}

/**
 * print_python_float - Prints basic info about a Python float object
 * @p: pointer to the PyObject (expected to be a PyFloatObject)
 *
 * Return: nothing
 */
void print_python_float(PyObject *p)
{
	double value;
	char *str;

	setbuf(stdout, NULL);
	printf("[.] float object info\n");

	if (!PyFloat_Check(p))
	{
		printf("  [ERROR] Invalid Float Object\n");
		return;
	}

	value = ((PyFloatObject *)p)->ob_fval;
	str = PyOS_double_to_string(value, 'r', 0, Py_DTSF_ADD_DOT_0, NULL);
	printf("  value: %s\n", str);
	PyMem_Free(str);
}

/**
 * print_python_list - Prints basic info about a Python list object
 * @p: pointer to the PyObject (expected to be a PyListObject)
 *
 * Return: nothing
 */
void print_python_list(PyObject *p)
{
	Py_ssize_t size, i;
	PyListObject *list;
	PyObject *item;

	setbuf(stdout, NULL);

	if (!PyList_Check(p))
	{
		printf("  [ERROR] Invalid List Object\n");
		return;
	}

	size = ((PyVarObject *)p)->ob_size;
	list = (PyListObject *)p;

	printf("[*] Python list info\n");
	printf("[*] Size of the Python List = %ld\n", size);
	printf("[*] Allocated = %ld\n", list->allocated);

	for (i = 0; i < size; i++)
	{
		item = list->ob_item[i];
		printf("Element %ld: %s\n", i, item->ob_type->tp_name);
		if (PyBytes_Check(item))
			print_python_bytes(item);
		else if (PyFloat_Check(item))
			print_python_float(item);
	}
}
