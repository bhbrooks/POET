#include<Python.h>
#include<numpy/arrayobject.h>
#include<math.h>
#include<omp.h>

#define IND(a,i) *((double *)(a->data+i*a->strides[0]))

static PyObject *sincos2(PyObject *self, PyObject *args, PyObject *keywds);

static PyObject *sincos2(PyObject *self, PyObject *args, PyObject *keywds)
{
  PyObject *etc;
  PyArrayObject *x,*y, *rampparams;
  double c1a,c1o,c2a,c2o,s1a,s1o,s2a,s2o,p,c,midpt,t14,t12,pi,mod;
  int i;
  npy_intp dims[1];

  //  etc = PyList_New(0);

  static char *kwlist[] = {"rampparams","x","etc",NULL};

  if(!PyArg_ParseTupleAndKeywords(args,keywds,"OO|O",kwlist,&rampparams,&x,&etc))
    {
      return NULL;
    }

  c1a   = IND(rampparams,0);
  c1o   = IND(rampparams,1);
  c2a   = IND(rampparams,2);
  c2o   = IND(rampparams,3);
  s1a   = IND(rampparams,4);
  s1o   = IND(rampparams,5);
  s2a   = IND(rampparams,6);
  s2o   = IND(rampparams,7);
  p     = IND(rampparams,8);
  c     = IND(rampparams,9);
  midpt = IND(rampparams,10);
  t14   = IND(rampparams,11);
  t12   = IND(rampparams,12);
  pi    = 3.141592653589793;

  dims[0] = x->dimensions[0];

  y = (PyArrayObject *) PyArray_SimpleNew(1,dims,PyArray_DOUBLE);
  #pragma omp parallel for private(mod)
  for(i=0;i<dims[0];i++)
    {
      //modulus works different in c when values are negative
      //mod = fmod(IND(x,i)-midpt,p);
      mod = (IND(x,i)-midpt) - p*floor((IND(x,i)-midpt)/p);
      if ((mod >= (p-(t14-t12)/2.)) || (mod <= ((t14-t12)/2.)))
        {
          //Flatten sin/cos during eclipse
          IND(y,i) = c1a*cos(2*pi*(midpt-c1o)/p) + c2a*cos(4*pi*(midpt-c2o)/p) + s1a*sin(2*pi*(midpt-s1o)/p) + s2a*sin(4*pi*(midpt-s2o)/p) + c;
          //printf("%i, ", i);
        }
      else
        {
          //Standard calculation
          IND(y,i) = c1a*cos(2*pi*(IND(x,i)-c1o)/p) + c2a*cos(4*pi*(IND(x,i)-c2o)/p) + s1a*sin(2*pi*(IND(x,i)-s1o)/p) + s2a*sin(4*pi*(IND(x,i)-s2o)/p) + c;
        }
    }
  //printf("\n");
  return PyArray_Return(y);
}

static char sincos2_doc[] = "\
 NAME:\n\
	SINCOS2\n\
\n\
 PURPOSE:\n\
	This function creates a model that fits a sinusoid.\n\
\n\
 CATEGORY:\n\
	Astronomy.\n\
\n\
 CALLING SEQUENCE:\n\
\n\
	Result = SINCOS2([c1a,c1o,c2a,c2o,s1a,s1o,s2a,s2o,p,c,midpt,t14,t12],x)\n\
\n\
 INPUTS:\n\
    c#a/s#a     : amplitude\n\
	c#o/s#o     : phase/time offset\n\
	p           : period\n\
    c           : vertical offset\n\
	x           : Array of time/phase points\n\
\n\
 OUTPUTS:\n\
	This function returns an array of y values\n\
\n\
 PROCEDURE:\n\
\n\
 EXAMPLE:\n\
\n\
\n\
\n\
 MODIFICATION HISTORY:\n\
 	Written by:	Kevin Stevenson, UCF  	2013-11-22\n\
			kevin218@knights.ucf.edu\n\
            converted to c              2015-03-17\n\
            \n\
\n\
";

static PyMethodDef sincos2_methods[] = {
  {"sincos2",(PyCFunction)sincos2,METH_VARARGS|METH_KEYWORDS,sincos2_doc},{NULL}};

void initsincos2(void)
{
  Py_InitModule("sincos2",sincos2_methods);
  import_array();
}
