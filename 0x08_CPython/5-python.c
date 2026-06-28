#include <Python.h>
#include <longintrepr.h>
#include <limits.h>
#include <stdio.h>

/**
 * print_python_int - Prints the value of a Python int object
 * @p: pointer to the PyObject (expected to be a PyLongObject)
 *
 * Description: Translates the Python int to a C unsigned long int and prints
 * it. If the absolute value does not fit in an unsigned long int, prints
 * "C unsigned long int overflow". If @p is not a valid int, prints
 * "Invalid Int Object".
 *
 * Return: nothing
 */
void print_python_int(PyObject *p)
{
	PyLongObject *num;
	Py_ssize_t size, i;
	int negative = 0;
	unsigned long int value = 0;

	setbuf(stdout, NULL);

	if (!PyLong_Check(p))
	{
		printf("Invalid Int Object\n");
		return;
	}

	num = (PyLongObject *)p;
	size = ((PyVarObject *)p)->ob_size;

	if (size < 0)
	{
		negative = 1;
		size = -size;
	}

	if (size == 0)
	{
		printf("0\n");
		return;
	}

	for (i = size - 1; i >= 0; i--)
	{
		if (value > (ULONG_MAX >> PyLong_SHIFT))
		{
			printf("C unsigned long int overflow\n");
			return;
		}
		value <<= PyLong_SHIFT;

		if (value > ULONG_MAX - num->ob_digit[i])
		{
			printf("C unsigned long int overflow\n");
			return;
		}
		value += num->ob_digit[i];
	}

	if (negative)
		printf("-%lu\n", value);
	else
		printf("%lu\n", value);
}
