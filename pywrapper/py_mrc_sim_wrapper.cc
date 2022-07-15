#include"Python.h"
#include"mrc_sim.hh"
#include"normal_trace.hh"
#include"lru.hh"
#include"stdint.h"
#include <numpy/arrayobject.h>
#include "wrapper.hh"


#ifdef _cplusplus
extern "C"{

#endif


vector<vector<double>> sim_MRC(const char * filename, int64_t max_trace_len, int64_t cache_size_start, double size_factor, int64_t n_test_points)
{
    log_info(logger, "=============[args]: %s %ld %ld %lf %ld\n", filename, max_trace_len, cache_size_start, size_factor, n_test_points);
    // TraceLoader * loader = new NormalTrace(READ_ALL);
    TraceLoader * loader = new NormalTrace(BUFFER_READ);

    loader->read_file(filename, max_trace_len);
    Cache * cache = new LRUCache(1);

    MRC_SIM mrc_sim(size_factor, cache_size_start, n_test_points);

    mrc_sim.bind_data_cache(loader, cache);

    auto all_mr = mrc_sim.run_MRC();

    vector<vector<double>> ret_mrc;

    log_info(logger,"before print sim\n");

    for (auto one_mr : all_mr)
    {
        printf("cache_size %ld, hit cnt %ld, hit size %ld\n", one_mr[0], one_mr[1], one_mr[2]);

        vector<double> size_bmr_and_omr = {(1.0)*(int)(one_mr[0]), (1.0*one_mr[1])/mrc_sim.total_req_num, (1.0*one_mr[2])/mrc_sim.total_req_size};

        ret_mrc.push_back(size_bmr_and_omr);
    }

    for (auto one_mr : ret_mrc)
    {
        printf("cache_size %lf, hit ratio %lf, hit byte ratio %lf\n", one_mr[0], one_mr[1], one_mr[2]);
    }


    log_info(logger,"finish sim\n");
    loader->close_file();
    delete loader;
    delete cache;

    return ret_mrc;
}



PyObject* wrap_sim_MRC(PyObject* self, PyObject* args) 
{ 
    char * filename = NULL;
    int64_t max_trace_len;
    int64_t cache_size_start;
    double size_factor;
    int64_t n_test_points;

    if (!PyArg_ParseTuple(args, "siidi:sim_MRC", &filename, &max_trace_len, &cache_size_start, &size_factor, &n_test_points))
        return NULL; 

    printf("[args]: %s %ld %ld %lf %ld\n", filename, max_trace_len, cache_size_start, size_factor, n_test_points);


  auto ret_mrc = sim_MRC(filename, max_trace_len, cache_size_start, size_factor, n_test_points); 

  log_info(logger,"finish sim in wrapper\n");
  
  PyArrayObject* ret_vec = vector_to_nparray(ret_mrc); //2d array
  log_info(logger,"before ret in wrapper\n");
  
  return PyArray_Return(ret_vec);
}

static PyMethodDef c_MrcSimMethods[] = 
{ 
    // fab：python调用的函数名，wrap_fab：c++中的函数名
  {"runMrcSim", wrap_sim_MRC, METH_VARARGS, "run mrc simulation"}, 
  {NULL, NULL} 
}; 


static struct PyModuleDef c_MrcSimModule = {
    PyModuleDef_HEAD_INIT,
    "c_MrcSim",//模块名称
    NULL,
    -1,
    c_MrcSimMethods//方法集合
};

PyMODINIT_FUNC  PyInit_c_MrcSim() {
    import_array();
    PyModule_Create(&c_MrcSimModule);
}

#ifdef _cplusplus
}
#endif