#include <string>
#include <iostream>
#include "CError.h"

using namespace std;

// init static members: by default use std out stream
CError::StreamListType CError::mStreamList(1, &cout);

/******************************************************************************
*     CError
******************************************************************************/

CError::CError(){
  mID= ID_Ok;
  mFunctionName= "";
  mMessage= "";
  //StreamAdd(cout); // static member is now initialized in definition of mStreamList
  mEmittedChars= 0;
}

/******************************************************************************
*     ~CError
******************************************************************************/

CError::~CError(){
}

/******************************************************************************
*     AddComment
******************************************************************************/
/*
void CError::AddComment(char* Comment){
  mMessage += Comment;
}

void CError::AddComment(string& Comment){
  mMessage += Comment;
}
*/
/******************************************************************************
*     EmitComment
******************************************************************************/
/*
void CError::EmitComment(char* Comment){
  StreamListType::iterator i;
  for( i= mStreamList.begin(); i != mStreamList.end(); i++ ){
    **i << Comment;
  }
}

void CError::EmitComment(string& Comment){
  StreamListType::iterator i;
  for( i= mStreamList.begin(); i != mStreamList.end(); i++ ){
    **i << Comment;
  }
}
*/

/******************************************************************************
*     FunctionName
******************************************************************************/

void CError::FunctionName(char* Name){
  mFunctionName= Name;
}

char* CError::FunctionName(){
  return mFunctionName;
}

/******************************************************************************
*     ID
******************************************************************************/

CError::IDType CError::ID(){
  return mID;
}

/******************************************************************************
*     Message
******************************************************************************/

string& CError::Message(){
  return mMessage;
}

/******************************************************************************
*     MessageEmit
******************************************************************************/

void CError::MessageEmit(){
  StreamListType::iterator i;
  string s;
  s= mMessage.substr(mEmittedChars, mMessage.length() - mEmittedChars);
  for( i= mStreamList.begin(); i != mStreamList.end(); i++ ){
    if( mFunctionName[0] != '\0' ){
      **i << "\nError while calling \"" << mFunctionName << "\"" << endl;
    }
    **i << s;
  }
  mEmittedChars= mMessage.length();
}

/******************************************************************************
*     StreamAdd
******************************************************************************/

void CError::StreamAdd(ostream& Stream){
  // if the stream is already listed, remove it to avoid clones
  mStreamList.remove(&Stream);
  mStreamList.push_back(&Stream);
}

/******************************************************************************
*     StreamRemove
******************************************************************************/

void CError::StreamRemove(ostream& Stream){
  mStreamList.remove(&Stream);
}

/******************************************************************************
*     FileConv
******************************************************************************/
void CError::FileConv(string& FileName){
  mMessage+= "\nConversion error occured while reading file \"" + FileName +
    "\".\n";
  MessageEmit();
  mID= ID_FileConv;
}

/******************************************************************************
*     FileEOF
******************************************************************************/
void CError::FileEOF(string& FileName){
  mMessage+= "\nUnexpected end of file \"" + FileName + "\" found.\n";
  MessageEmit();
  mID= ID_FileEOF;
}

/******************************************************************************
*     FileI
******************************************************************************/
void CError::FileI(string& FileName){
  mMessage+= "\nCould not open file \"" + FileName + "\" for reading.\n";
  MessageEmit();
  mID= ID_FileI;
}

/******************************************************************************
*     FileIO
******************************************************************************/
void CError::FileIO(string& FileName){
  mMessage+= "\nCould not open file \"" + FileName +
    "\" for read/write operations.\n";
  MessageEmit();
  mID= ID_FileIO;
}

/******************************************************************************
*     FileO
******************************************************************************/
void CError::FileO(string& FileName){
  mMessage+= "\nCould not open file \"" + FileName + "\" for writing.\n";
  MessageEmit();
  mID= ID_FileO;
}

/******************************************************************************
*     FileR
******************************************************************************/
void CError::FileR(string& FileName){
  mMessage+= "\nGeneral reading error for file \"" + FileName + "\"\n";
  MessageEmit();
  mID= ID_FileR;
}

/******************************************************************************
*     FileW
******************************************************************************/
void CError::FileW(string& FileName){
  mMessage+= "\nGeneral writing error for file \"" + FileName + "\"\n";
  MessageEmit();
  mID= ID_FileW;
}

/******************************************************************************
*     InitFailed
******************************************************************************/
void CError::InitFailed(char* VarName){
  mMessage += "\nInitialization of \"";
  mMessage += VarName;
  mMessage += "\" failed.\n";
  MessageEmit();
  mID= ID_InitFailed;
}

/******************************************************************************
*     OutOfMemory
******************************************************************************/
void CError::OutOfMemory(string& VarName){
  mMessage+= "\nCould not allocate memory for \"" + VarName + "\".\n";
  MessageEmit();
  mID= ID_OutOfMemory;
}

void CError::OutOfMemory(char* VarName){
  mMessage += "\nCould not allocate memory for \"";
  mMessage += VarName;
  mMessage += "\".\n";
  MessageEmit();
  mID= ID_OutOfMemory;
}

/******************************************************************************
*     OutOfRange
******************************************************************************/
void CError::OutOfRange(char* VarName){
  mMessage += "\nValue(s) in \"";
  mMessage += VarName;
  mMessage += "\" are out of allowed range.\n";
  MessageEmit();
  mID= ID_OutOfRange;
}

