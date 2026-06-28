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
	long size, i, limit;
	PyBytesObject *bytes;

	printf("[.] bytes object info\n");

	if (!PyBytes_Check(p))
	{
		printf("  [ERROR] Invalid Bytes Object\n");
		fflush(stdout);
		return;
	}

	bytes = (PyBytesObject *)p;
	size = (long)((PyVarObject *)p)->ob_size;

	printf("  size: %ld\n", size);
	printf("  trying string: %s\n", bytes->ob_sval);

	limit = (size + 1 < 10) ? size + 1 : 10;

	printf("  first %ld bytes:", limit);
	for (i = 0; i < limit; i++)
		printf(" %02x", (unsigned char)bytes->ob_sval[i]);
	printf("\n");
	fflush(stdout);
}

/**
 * print_python_float - Prints basic info about a Python float object
 * @p: pointer to the PyObject (expected to be a PyFloatObject)
 *
 * Return: nothing
 */
void print_python_float(PyObject *p)
{
	double val;
	char buf[100];
	int i, has_dot;

	printf("[.] float object info\n");

	if (!PyFloat_Check(p))
	{
		printf("  [ERROR] Invalid Float Object\n");
		fflush(stdout);
		return;
	}

	val = ((PyFloatObject *)p)->ob_fval;
	snprintf(buf, sizeof(buf), "%.16g", val);

	has_dot = 0;
	for (i = 0; buf[i] != '\0'; i++)
	{
		if (buf[i] == '.' || buf[i] == 'e' || buf[i] == 'E' ||
		    buf[i] == 'n' || buf[i] == 'N' || buf[i] == 'i' || buf[i] == 'I')
		{
			has_dot = 1;
			break;
		}
	}
	if (!has_dot)
	{
		buf[i] = '.';
		buf[i + 1] = '0';
		buf[i + 2] = '\0';
	}

	printf("  value: %s\n", buf);
	fflush(stdout);
}

/**
 * print_python_list - Prints basic info about a Python list object
 * @p: pointer to the PyObject (expected to be a PyListObject)
 *
 * Return: nothing
 */
void print_python_list(PyObject *p)
{
	long size, i;
	PyListObject *list;
	PyObject *item;

	printf("[*] Python list info\n");

	if (!PyList_Check(p))
	{
		printf("  [ERROR] Invalid List Object\n");
		fflush(stdout);
		return;
	}

	size = (long)((PyVarObject *)p)->ob_size;
	list = (PyListObject *)p;

	printf("[*] Size of the Python List = %ld\n", size);
	printf("[*] Allocated = %ld\n", (long)list->allocated);
	fflush(stdout);

	for (i = 0; i < size; i++)
	{
		item = list->ob_item[i];
		printf("Element %ld: %s\n", i, item->ob_type->tp_name);
		fflush(stdout);
		if (PyBytes_Check(item))
			print_python_bytes(item);
		else if (PyFloat_Check(item))
			print_python_float(item);
	}
}
