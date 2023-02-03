#ifndef CARRAYTOOLS_H
#define CARRAYTOOLS_H

#include "CArray.h"
#include "CArray2D.h"
#include "CError.h"

#include <string>
#include <fstream>

template <class T, class TScale> bool WriteArray(CArray<T, TScale>& Array, std::string FileName) {
  std::ofstream f;
  std::size_t i, n(Array.NValues());
  f.open(FileName.c_str());
  if (!f.good()) {
    CError Error;
    Error.FileO(FileName);
    return false;
  }
  for (i = 0; i < n; i++) {
    f << Array.ToX(i) << "\t" << Array[i] << "\n";
  }
  f.close();
  if (!f.good()) {
    CError Error;
    Error.FileW(FileName);
    return false;
  }
  return true;
}

template <class T, class TIndex> bool WriteArray(CArray2DBase<T, TIndex>& Array, std::string FileName) {
  std::ofstream f;
  f.open(FileName.c_str(), std::ios::out | std::ios::binary);
  if (!f.good()) {
    CError Error;
    Error.FileO(FileName);
    return false;
  }
  // write data 
  f.write(reinterpret_cast<char*>(Array.GetPtr()), sizeof(T)*Array.GetNValuesX()*Array.GetNValuesY());
  f.close();
  if (!f.good()) {
    CError Error;
    Error.FileW(FileName);
    return false;
  }
  return true;
}

#endif
