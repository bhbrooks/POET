#include<Python.h>
#include<numpy/arrayobject.h>
#include<math.h>
#include<omp.h>

#define IND(a,i) *((double *)(a->data+i*a->strides[0]))

static PyObject *linramp(PyObject *self, PyObject *args, PyObject *keywds);


static PyObject *linramp(PyObject *self, PyObject *args, PyObject *keywds)
{
  PyObject *etc;
  PyArrayObject *x,*y, *rampparams;
  double a,b,x0;
  int i;
  npy_intp dims[1];

  static char *kwlist[] = {"rampparams","x","etc",NULL};

  //  etc = PyList_New(0);

  if(!PyArg_ParseTupleAndKeywords(args,keywds,"OO|O",kwlist,&rampparams,&x,&etc))
    {
      return NULL;
    }

  a    = IND(rampparams,0);
  b    = IND(rampparams,1);
  x0   = IND(rampparams,2);

  dims[0] = x->dimensions[0];

  y = (PyArrayObject *) PyArray_SimpleNew(1,dims,PyArray_DOUBLE);


  #pragma omp parallel for
  for(i=0;i<dims[0];i++)
    {
      IND(y,i) = a*(IND(x,i)-x0)+b;
    }


  return PyArray_Return(y);
}

static char linramp_doc[] = "\
  This function creates a model that fits a ramp using a linear polynomial.\n\
\n\
  Parameters\n\
  ----------\n\
    x0: time offset\n\
    a: coefficient of first term\n\
    b: constant\n\
    x: Array of time/phase points\n\
\n\
  Returns\n\
  -------\n\
    This function returns the flux values for the ramp models \n\
\n\
  Revisions\n\
  ---------\n\
  2008-07-07	Kevin Stevenson, UCF  \n\
			kevin218@knights.ucf.edu\n\
		Original version\n\
  2010-12-25    Nate Lust, UCF\n\
                natelust at linux dot com\n\
                Converted to C\n\
";


static PyMethodDef linramp_methods[] = {
  {"linramp",(PyCFunction)linramp,METH_VARARGS|METH_KEYWORDS,linramp_doc},{NULL}};

void initlinramp(void)
{
  Py_InitModule("linramp",linramp_methods);
  import_array();
}
