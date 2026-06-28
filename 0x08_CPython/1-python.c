#include <Python.h>
#include <stdio.h>

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

	size = PyList_Size(p);
	list = (PyListObject *)p;

	printf("[*] Python list info\n");
	printf("[*] Size of the Python List = %ld\n", size);
	printf("[*] Allocated = %ld\n", list->allocated);

	for (i = 0; i < size; i++)
	{
		item = list->ob_item[i];
		printf("Element %ld: %s\n", i, item->ob_type->tp_name);
	}
}
