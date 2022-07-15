#include <Python.h>
#include <arrayobject.h>
#include <stdlib.h>
#include "utils.hh"



/** Convert a c++ 2D vector into a numpy array
 *
 * @param const vector< vector<T> >& vec : 2D vector data
 * @return PyArrayObject* array : converted numpy array
 *
 * Transforms an arbitrary 2D C++ vector into a numpy array. Throws in case of
 * unregular shape. The array may contain empty columns or something else, as
 * long as it's shape is square.
 *
 * Warning this routine makes a copy of the memory!
 */
template<typename T>
static PyArrayObject* vector_to_nparray(const vector< vector<T> >& vec, int type_num = PyArray_DOUBLE){ 
   // if convert Double use PyArray_DOUBLE
   
   // rows not empty
   if( !vec.empty() ){

      // column not empty
      if( !vec[0].empty() ){

        npy_intp nRows = vec.size();
        npy_intp nCols = vec[0].size();
        npy_intp dims[2] = {nRows, nCols};
        PyArrayObject* vec_array = NULL;
        vec_array = (PyArrayObject *) PyArray_SimpleNew(2, dims, type_num);

        T *vec_array_pointer = (T*) PyArray_DATA(vec_array);


        // copy vector line by line ... maybe could be done at one
        for (size_t iRow=0; iRow < vec.size(); ++iRow){
          if( vec[iRow].size() != nCols){
             Py_DECREF(vec_array); // delete
             throw(string("Can not convert vector<vector<T>> to np.array, since c++ matrix shape is not uniform."));
          }

          copy(vec[iRow].begin(),vec[iRow].end(),vec_array_pointer+iRow*nCols);
        }
        return (vec_array);

     // Empty columns
     } else {
        npy_intp dims[2] = {(npy_intp)vec.size(), 0};
        return (PyArrayObject*) PyArray_ZEROS(2, dims, PyArray_FLOAT, 0);
     }



   // no data at all
   } else {
      npy_intp dims[2] = {0, 0};
      return (PyArrayObject*) PyArray_ZEROS(2, dims, PyArray_FLOAT, 0);
   }
   return NULL;

}


/** Convert a c++ vector into a numpy array
 *
 * @param const vector<T>& vec : 1D vector data
 * @return PyArrayObject* array : converted numpy array
 *
 * Transforms an arbitrary C++ vector into a numpy array. Throws in case of
 * unregular shape. The array may contain empty columns or something else, as
 * long as it's shape is square.
 *
 * Warning this routine makes a copy of the memory!
 */
template<typename T>
static PyArrayObject* vector_to_nparray(const vector<T>& vec, int type_num = PyArray_DOUBLE){

   // rows not empty
   if( !vec.empty() ){

       size_t nRows = vec.size();
       npy_intp dims[1] = {nRows};

       PyArrayObject* vec_array = (PyArrayObject *) PyArray_SimpleNew(1, dims, type_num);
       T *vec_array_pointer = (T*) PyArray_DATA(vec_array);

       copy(vec.begin(),vec.end(),vec_array_pointer);
       return vec_array;

   // no data at all
   } else {
      npy_intp dims[1] = {0};
      return (PyArrayObject*) PyArray_ZEROS(1, dims, PyArray_FLOAT, 0);
   }

}