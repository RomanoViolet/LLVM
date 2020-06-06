#include <cassert>
#include <clang-c/Index.h>

#include <cstdlib>
#include <iostream>
#include <string>
#include <utility>

using LineColumn = std::pair< unsigned, unsigned >;

struct ClassDetails {
  std::string _name;
  std::string _namespace;
  std::string _baseclass;
};

struct Data {
  CXCursorKind _cursorKind;
  std::string _tranlationUnitName;
  ClassDetails _classDetails;
};

std::string toString( CXString cxString )
{
  std::string string = clang_getCString( cxString );
  clang_disposeString( cxString );
  return string;
}

CXChildVisitResult visitForFirstPass( CXCursor cursor, CXCursor parent, CXClientData clientData )
{
  // Parse the header only for items of interest
  const CXCursorKind kind = clang_getCursorKind( cursor );
  // CXString name = clang_getCursorSpelling( cursor );
  Data *data = static_cast< Data * >( clientData );

  std::cout << toString( clang_getCursorKindSpelling( kind ) ) << std::endl;

  switch ( kind ) {
    case CXCursorKind ::CXCursor_ClassDecl: {
      // AST hierarchy for top level class: Namespace - Class - Baseclass.
      const CXType type = clang_getCursorType( cursor );
      std::cout << toString( clang_getTypeSpelling( type ) ) << std::endl;
      data->_classDetails._name = toString( clang_getCursorSpelling( cursor ) );
      data->_classDetails._namespace.append( "::" + toString( clang_getCursorSpelling( parent ) ) );
      break;
    }

    case CXCursorKind::CXCursor_CXXBaseSpecifier: {
      // base class. Parent is ClassDecl.
      assert( clang_getCursorKind( parent ) == CXCursorKind::CXCursor_ClassDecl
              && "Base Class Without Matching Derived Class." );
      // do we know the derived class?
      const CXType type = clang_getCursorType( cursor );
      std::cout << toString( clang_getTypeSpelling( type ) ) << std::endl;
      if ( data->_classDetails._name.compare( toString( clang_getCursorSpelling( parent ) ) )
           == 0 ) {
        // we know the derived class.
        data->_classDetails._baseclass = toString( clang_getCursorSpelling( cursor ) );
      }
      break;
    }

    default:
      break;
  }

  return CXChildVisit_Recurse;
}  // visitForFirstPass

CXChildVisitResult visit( CXCursor cursor, CXCursor parent, CXClientData clientData )
{
  // Parse the header only for items of interest
  CXCursorKind kind = clang_getCursorKind( cursor );
  // CXString name = clang_getCursorSpelling( cursor );
  Data *data = static_cast< Data * >( clientData );

  switch ( kind ) {
    case CXCursorKind ::CXCursor_ClassDecl: {
      // AST hierarchy for top level class: Namespace - Class - Baseclass.
      data->_classDetails._name = toString( clang_getCursorSpelling( cursor ) );
      data->_classDetails._namespace.append( "::" + toString( clang_getCursorSpelling( parent ) ) );
      break;
    }

    case CXCursorKind::CXCursor_CXXBaseSpecifier: {
      // base class. Parent is ClassDecl.
      assert( clang_getCursorKind( parent ) == CXCursorKind::CXCursor_ClassDecl
              && "Base Class Without Matching Derived Class." );
      // do we know the derived class?
      if ( data->_classDetails._name.compare( toString( clang_getCursorSpelling( parent ) ) )
           == 0 ) {
        // we know the derived class.
        data->_classDetails._baseclass = toString( clang_getCursorSpelling( cursor ) );
      }
      break;
    }

    default:
      break;
  }
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
  CXIndex index = clang_createIndex( /*excludeDeclarationsFromPCH=*/true,
                                     /*displayDiagnostics=*/true );

  // See https://clang.llvm.org/doxygen/group__CINDEX__TRANSLATION__UNIT.html
  // for the possible options (last argument).

  unsigned flags = CXTranslationUnit_Flags::CXTranslationUnit_SkipFunctionBodies
                   | CXTranslationUnit_Flags::CXTranslationUnit_IgnoreNonErrorsFromIncludedFiles
                   | CXTranslationUnit_Flags::CXTranslationUnit_Incomplete
                   | CXTranslationUnit_SingleFileParse;

  constexpr const char *defaultArguments[] = {
      "-x", "c++", "-std=c++11", "-Xclang", "-fsyntax-only", "-I/workspaces/LLVM/CoreFunctions" };

  // First Pass to get class-level information out.
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
    Data data;
    traverseForFirstPass( tu, data );
    clang_disposeTranslationUnit( tu );
  }
  clang_disposeIndex( index );

  // Second Pass
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
    Data data;
    traverse( tu, data );
    clang_disposeTranslationUnit( tu );
  }
  clang_disposeIndex( index );
}
