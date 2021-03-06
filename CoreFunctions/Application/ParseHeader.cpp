/**
 * @file ParseHeader.cpp
 * @author RomanoViolet
 * @brief This is a wrapper for instantiating and invoking the parser RomanoViolet::StateMachine
 * class.
 * @version 1.0
 * @date 2020-06-28
 *
 * @copyright Copyright (c) 2020
 * @see Based on Peter Goldsborough's AST parser example. See README.md for the link to Peter's
 * repository.
 */

#include "StateMachine.hpp"
#include <cassert>
#include <clang-c/Index.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using LineColumn = std::pair< unsigned, unsigned >;

struct IODetails {
  std::string _ioName;
  std::string _type;
  std::string _direction;
};

struct ClassDetails {
  std::string _name;
  std::string _namespace = "";
  std::string _baseclass;
};

struct Data {
  CXCursorKind _cursorKind;
  std::string _tranlationUnitName;
  ClassDetails _classDetails;
  std::vector< IODetails > io;
  std::string _temp;
  IODetails _tempIODetails;
  bool insideClass = false;
  bool insideIODeclaration = false;
  RomanoViolet::StateMachine *p;
};

std::string toString( CXString cxString )
{
  std::string string = clang_getCString( cxString );
  clang_disposeString( cxString );
  return string;
}

CXChildVisitResult visitForFirstPass( CXCursor cursor, CXCursor parent, CXClientData clientData )
{
  // return result -- useful for stopping further exploration of AST
  // defaults to recursive exploration until explicitly stopped.
  CXChildVisitResult result = CXChildVisitResult::CXChildVisit_Recurse;

  // Parse the header only for items of interest
  const CXCursorKind kind = clang_getCursorKind( cursor );
  // CXString name = clang_getCursorSpelling( cursor );
  Data *data = static_cast< Data * >( clientData );

  // collect all successive namespaces
  if ( kind == CXCursorKind ::CXCursor_Namespace ) {
    if ( data->_temp.empty( ) ) {
      data->_temp = toString( clang_getCursorSpelling( cursor ) );
    } else {
      data->_temp.append( "::" + toString( clang_getCursorSpelling( cursor ) ) );
    }
  }

  // in order to bot collect all possible non-consecutive namespaces, clear the variable
  // when the current cursor is not a namespace and neither is the parent.
  if ( ( kind != CXCursorKind ::CXCursor_Namespace )
       && ( clang_getCursorKind( parent ) != CXCursorKind ::CXCursor_Namespace ) ) {
    data->_temp.clear( );
  }

  switch ( kind ) {
    case CXCursorKind ::CXCursor_ClassDecl: {
      // AST hierarchy for top level class: Namespace - Class - Baseclass.
      data->_classDetails._namespace = data->_temp;
      data->_classDetails._name = toString( clang_getCursorSpelling( cursor ) );
      // data->_classDetails._namespace.append( "::" + toString( clang_getCursorSpelling( parent ) )
      // );

      // stop any further exploration
      result = CXChildVisitResult::CXChildVisit_Break;
      break;
    }

    default:
      break;
  }

  return result;
}  // visitForFirstPass

CXChildVisitResult visit( CXCursor cursor, CXCursor parent, CXClientData clientData )
{
  // CXString name = clang_getCursorSpelling( cursor );
  ( void )parent;
  Data *data = static_cast< Data * >( clientData );
  data->p->AdvanceStateMachine( cursor );

  // clang visitor should visit the next sibling of this cursor without visiting the children.
  return CXChildVisit_Recurse;
}  // visit

void traverse( CXTranslationUnit tu, Data &data )
{
  CXCursor root = clang_getTranslationUnitCursor( tu );

  // name of this translation unit
  data._tranlationUnitName = toString( clang_getTranslationUnitSpelling( tu ) );

  clang_visitChildren( root, visit, &data );
}

void traverseForFirstPass( CXTranslationUnit tu, Data &data )
{
  CXCursor root = clang_getTranslationUnitCursor( tu );

  // name of this translation unit
  data._tranlationUnitName = toString( clang_getTranslationUnitSpelling( tu ) );

  clang_visitChildren( root, visitForFirstPass, &data );
}

auto main( int argc, const char *argv[] ) -> int
{
  ( void )argc;
  CXIndex index = clang_createIndex( /*excludeDeclarationsFromPCH=*/true,
                                     /*displayDiagnostics=*/true );

  // See https://clang.llvm.org/doxygen/group__CINDEX__TRANSLATION__UNIT.html
  // for the possible options (last argument).

  unsigned flags = CXTranslationUnit_Flags::CXTranslationUnit_SkipFunctionBodies
                   | CXTranslationUnit_Flags::CXTranslationUnit_IgnoreNonErrorsFromIncludedFiles
                   | CXTranslationUnit_Flags::CXTranslationUnit_Incomplete
                   | CXTranslationUnit_Flags::CXTranslationUnit_SingleFileParse;

  constexpr const char *defaultArguments[] = {
      "-x", "c++", "-std=c++11", "-Xclang", "-fsyntax-only", "-I/workspaces/LLVM/CoreFunctions" };

  // Application-scope data storage
  Data data;

  // First Pass to get class-level information out.
  // The objective of the first pass is to find out the class declared in the test file
  // "Component.hpp" and pass it to the second parse stage.
  // If the exact class name is known before-hand, the first stage may be removed.

  CXTranslationUnit tu = clang_parseTranslationUnit( index,
                                                     /*source_filename=*/argv[1],
                                                     /*command_line_args=*/defaultArguments,
                                                     /*num_command_line_args=*/6,
                                                     /*unsaved_files=*/nullptr,
                                                     /*num_unsaved_files=*/0,
                                                     /*options=*/flags );

  if ( tu == nullptr ) {
    std::cerr << "Unable to parse translation unit. Quitting.\n";
  } else {
    traverseForFirstPass( tu, data );
    clang_disposeTranslationUnit( tu );
  }
  clang_disposeIndex( index );

  // Second Pass: Aggregate required details of the requested class.
  RomanoViolet::StateMachine p{ data._classDetails._name };
  index = clang_createIndex( /*excludeDeclarationsFromPCH=*/true,
                             /*displayDiagnostics=*/true );
  flags = CXTranslationUnit_Flags::CXTranslationUnit_SkipFunctionBodies
          | CXTranslationUnit_Flags::CXTranslationUnit_IgnoreNonErrorsFromIncludedFiles
          | CXTranslationUnit_Flags::CXTranslationUnit_Incomplete
          | CXTranslationUnit_Flags::CXTranslationUnit_DetailedPreprocessingRecord;

  tu = clang_parseTranslationUnit( index,
                                   /*source_filename=*/argv[1],
                                   /*command_line_args=*/defaultArguments,
                                   /*num_command_line_args=*/6,
                                   /*unsaved_files=*/nullptr,
                                   /*num_unsaved_files=*/0,
                                   /*options=*/flags );

  if ( tu == nullptr ) {
    std::cerr << "Unable to parse translation unit. Quitting.\n";
  } else {
    data.p = &p;
    traverse( tu, data );
    data.p->print( );
    clang_disposeTranslationUnit( tu );
  }
  clang_disposeIndex( index );
}
