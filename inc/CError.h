/*!*****************************************************************
*! \file  CError.h
* \brief Administrates general error messaging
*
* $Date: 28.06.05
* \author K.J. Engel
* \par Copyright
* (C) Philips Research Labs, Weisshausstr. 2, 52066 Aachen, Germany
*
* class CError:
*
*   public enumeration type:
*
*     IDType   :  identifies error type. Definitions see below.
*
*   public administrative member functions:
*
*     CError:
*       CError();
*       Constructor.
*
*     ~CError:
*       ~CError();
*       Destructor.
*
*     FunctionName:
*       (1) void FunctionName(string& Name);
*       (2) string& FunctionName();
*       Syntax (1) stores the name "Name" of the function, in which an error
*       could occure. In case of an error invoke, this function name is
*       emitted to the streams specified by the member "StreamAdd()".
*       Syntax (2) returns the function name currently stored, or an empty
*       string, if no function name was stored yet.
*
*     ID:
*       IDType ID();
*       returns an error ID number. If no error was invoked,
*       CError::Ok is returned.
*
*     Message:
*       string& Message();
*       returns a reference to a string containing an error message.
*       An empty string is returned if no error was invoked.
*
*     MessageEmit:
*       void MessageEmit();
*       Emits an error message (if existing) to all streams, which
*       were added by the class member StreamAdd().
*
*     StreamAdd:
*       void StreamAdd(ostream& Stream);
*       Adds an output stream to a list of streams, to which the error message
*       has to be sent if invoked.
*
*     StreamRemove:
*       void StreamRemove(ostream& Stream);
*       Removes an output stream from a list of streams, to which the error
*       message has to be sent if invoked.
*
*
*   public member functions for error invoking:
*
*   To each error handled here, a respective member function exists.
*   A calling of this member function results in creating an error message, and
*   an error ID. The message is referenced by the member function "Message()",
*   and the error ID (of "IDType") is returned by the member function "ID()".
*   Furthermore, the error messages are emitted to streams which were specified
*   by the member function "StreamAdd()" before.
*   The error ID is always composed by the function name, prefixed by
*   "ID_"; i.e. the error invoking function "FileIO()" results in the member
*   function "ID()" returning the value "ID_FileIO".
*
*   following error ID are handled here:
*
*         ID_Ok           : No error
*         ID_FileConv     : Conversion error occured while reading from file
*         ID_FileEOF      : Unexpected end of file found
*         ID_FileI        : Could not open file for reading
*         ID_FileIO       : Could not open file for reading/writing
*         ID_FileO        : Could not open file for writing
*         ID_LastID       : Last ID number in the IDType enumeration list

*   File I/O error are invoked by the following list of member functions:
*
*     void FileConv(string& FileName);
*     void FileEOF (string& FileName);
*     void FileI   (string& FileName);
*     void FileIO  (string& FileName);
*     void FileO   (string& FileName);
*
*     Each of those member functions reports the "FileName" in the error message.
*
********************************************************************/

#ifndef CERROR_H
#define CERROR_H

#include <iostream>
#include <string>
#include <list>
#include <sstream>

class CError{

public:

  typedef enum {
    ID_Ok,
    ID_FileConv,
    ID_FileEOF,
    ID_FileI,
    ID_FileIO,
    ID_FileO,
    ID_FileR,
    ID_FileW,
    ID_InitFailed,
    ID_OutOfMemory,
    ID_OutOfRange,
    ID_LastId
  } IDType;

  // administrative member functions
  CError();
  ~CError();

  template <class T> void AddComment(T& Comment){
    std::string s;
    std::stringstream ss;
    ss << Comment;
    s= ss.str();
    mMessage += s;
    MessageEmit();
  }

  template <class T> void EmitComment(T& Comment){
    StreamListType::iterator i;
    for( i= mStreamList.begin(); i != mStreamList.end(); i++ ){
      **i << Comment;
    }
  }

  void FunctionName(char* Name);
  char* FunctionName();
  IDType ID();
  std::string& Message();
  void MessageEmit();
  void StreamAdd(std::ostream& Stream);
  void StreamRemove(std::ostream& Stream);

  // member functions to invoke error messages
  void FileConv(std::string& FileName);
  void FileEOF(std::string& FileName);
  void FileI(std::string& FileName);
  void FileIO(std::string& FileName);
  void FileO(std::string& FileName);
  void FileR(std::string& FileName);
  void FileW(std::string& FileName);
  void InitFailed(char* VarName);
  void OutOfMemory(std::string& VarName);
  void OutOfMemory(char* VarName);
  void OutOfRange(char* VarName);

protected:

  std::string mMessage;
  char* mFunctionName;
  IDType mID;

private:

  typedef std::list<std::ostream*> StreamListType;
  static StreamListType mStreamList;
  size_t mEmittedChars;
};

template <class T> CError& operator<<(CError& Error, T a){
  // note: class T is passed NOT by reference; this eases streaming of e.g. function results (... << function() << ...)
  // and avoids typical call-by-reference errors ( ... << (d=function1()) << (d=function2()) << ... wherein only ONE d value will be evaluated after passing by reference)
  Error.AddComment(a);
  return Error;
}

#endif
