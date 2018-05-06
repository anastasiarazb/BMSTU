// Automatically generated file. Don't edit!
#include "refalrts.h"


extern refalrts::RefalFunction& WriteLine;
extern refalrts::RefalFunction& Arg;
extern refalrts::RefalFunction& Inc;
extern refalrts::RefalFunction& Dec;
extern refalrts::RefalFunction& Fetch;
extern refalrts::RefalFunction& LoadFile;
extern refalrts::RefalFunction& Map;
extern refalrts::RefalFunction& Reduce;
namespace /* unnamed */ {
extern refalrts::RefalFunction& Lol;
} // unnamed namespace

extern refalrts::RefalFunction& Go;

#ifdef INTERPRET
namespace /* unnamed */ {
  namespace scope_Lol {
    static const char *filename = "little_test.cpp";
    static refalrts::RefalFunction *functions[] = {
      & WriteLine
    };
    using refalrts::idents;
    using refalrts::numbers;
    using refalrts::strings;
    static const refalrts::RASLCommand raa[] = {
      {refalrts::icThisIsGeneratedFunction, 0, 0, 0},
      {refalrts::icLoadConstants, 0, 0, 0},
      {refalrts::icIssueMemory, 6, 0, 0},
      // t.a#1/2
      {refalrts::icInitB0, 0, 0, 0},
      {refalrts::ictVarLeft, 0, 2, 0},
      {refalrts::icEmpty, 0, 0, 0},
      //DEBUG: t.a#1: 2
      //2: t.a#1
      {refalrts::icEmptyResult, 0, 0, 0},
      {refalrts::icAllocBracket, 0, refalrts::ibOpenCall, 3},
      {refalrts::icAllocFunc, 0, 0, 4},
      {refalrts::icAllocBracket, 0, refalrts::ibCloseCall, 5},
      {refalrts::icPushStack, 0, 0, 5},
      {refalrts::icPushStack, 0, 0, 3},
      {refalrts::icSpliceElem, 0, 0, 5},
      {refalrts::icSpliceSTVar, 0, 0, 2},
      {refalrts::icSpliceElem, 0, 0, 4},
      {refalrts::icSpliceElem, 0, 0, 3},
      {refalrts::icSpliceToFreeList, 0, 0, 0},
      {refalrts::icNextStep, 0, 0, 0},
      {refalrts::icEnd, 0, 0, 0}
    };
  } // namespace scope_Lol

} // unnamed namespace

namespace /* unnamed */ {
refalrts::RASLFunction descr_Lol(
  "Lol",
  scope_Lol::raa,
  scope_Lol::functions,
  scope_Lol::idents,
  scope_Lol::numbers,
  scope_Lol::strings,
  scope_Lol::filename
);
refalrts::RefalFunction& Lol = descr_Lol;

} // unnamed namespace

#else
static refalrts::FnResult func_Lol(refalrts::Iter arg_begin, refalrts::Iter arg_end) {
  refalrts::this_is_generated_function();
  // issue here memory for vars with 6 elems
  refalrts::Iter context[6];
  refalrts::zeros( context, 6 );
  // t.a#1/2
  context[0] = arg_begin;
  context[1] = arg_end;
  refalrts::move_left( context[0], context[1] );
  refalrts::move_left( context[0], context[1] );
  refalrts::move_right( context[0], context[1] );
  if( ! refalrts::tvar_left( context[2], context[0], context[1] ) )
    return refalrts::cRecognitionImpossible;
  if( ! refalrts::empty_seq( context[0], context[1] ) )
    return refalrts::cRecognitionImpossible;
  //DEBUG: t.a#1: 2
  //2: t.a#1

  refalrts::reset_allocator();
  refalrts::Iter res = arg_begin;
  if( ! refalrts::alloc_open_call( context[3] ) )
    return refalrts::cNoMemory;
  if( ! refalrts::alloc_name( context[4], & WriteLine ) )
    return refalrts::cNoMemory;
  if( ! refalrts::alloc_close_call( context[5] ) )
    return refalrts::cNoMemory;
  refalrts::push_stack( context[5] );
  refalrts::push_stack( context[3] );
  res = refalrts::splice_elem( res, context[5] );
  res = refalrts::splice_stvar( res, context[2] );
  res = refalrts::splice_elem( res, context[4] );
  res = refalrts::splice_elem( res, context[3] );
  refalrts::use( res );
  refalrts::splice_to_freelist( arg_begin, arg_end );
  return refalrts::cSuccess;
}

namespace /* unnamed */ {
refalrts::RefalNativeFunction descr_Lol(func_Lol, "Lol");
refalrts::RefalFunction& Lol = descr_Lol;

} // unnamed namespace

#endif
#ifdef INTERPRET
namespace /* unnamed */ {
  namespace scope_Go {
    static const char *filename = "little_test.cpp";
    static refalrts::RefalFunction *functions[] = {
      & Arg,
      & Lol
    };
    using refalrts::idents;
    using refalrts::numbers;
    using refalrts::strings;
    static const refalrts::RASLCommand raa[] = {
      {refalrts::icThisIsGeneratedFunction, 0, 0, 0},
      {refalrts::icLoadConstants, 0, 0, 0},
      {refalrts::icIssueMemory, 11, 0, 0},
      //
      {refalrts::icInitB0, 0, 0, 0},
      {refalrts::icEmpty, 0, 0, 0},
      {refalrts::icEmptyResult, 0, 0, 0},
      {refalrts::icAllocBracket, 0, refalrts::ibOpenCall, 2},
      {refalrts::icAllocFunc, 0, 1, 3},
      {refalrts::icAllocBracket, 0, refalrts::ibOpenBracket, 4},
      {refalrts::icAllocBracket, 0, refalrts::ibOpenCall, 5},
      {refalrts::icAllocFunc, 0, 0, 6},
      {refalrts::icAllocInt, 0, 1, 7},
      {refalrts::icAllocBracket, 0, refalrts::ibCloseCall, 8},
      {refalrts::icAllocBracket, 0, refalrts::ibCloseBracket, 9},
      {refalrts::icAllocBracket, 0, refalrts::ibCloseCall, 10},
      {refalrts::icPushStack, 0, 0, 10},
      {refalrts::icPushStack, 0, 0, 2},
      {refalrts::icSpliceElem, 0, 0, 10},
      {refalrts::icLinkBrackets, 4, 9, 0},
      {refalrts::icSpliceElem, 0, 0, 9},
      {refalrts::icPushStack, 0, 0, 8},
      {refalrts::icPushStack, 0, 0, 5},
      {refalrts::icSpliceElem, 0, 0, 8},
      {refalrts::icSpliceElem, 0, 0, 7},
      {refalrts::icSpliceElem, 0, 0, 6},
      {refalrts::icSpliceElem, 0, 0, 5},
      {refalrts::icSpliceElem, 0, 0, 4},
      {refalrts::icSpliceElem, 0, 0, 3},
      {refalrts::icSpliceElem, 0, 0, 2},
      {refalrts::icSpliceToFreeList, 0, 0, 0},
      {refalrts::icNextStep, 0, 0, 0},
      {refalrts::icEnd, 0, 0, 0}
    };
  } // namespace scope_Go

} // unnamed namespace

refalrts::RASLFunction descr_Go(
  "Go",
  scope_Go::raa,
  scope_Go::functions,
  scope_Go::idents,
  scope_Go::numbers,
  scope_Go::strings,
  scope_Go::filename
);
refalrts::RefalFunction& Go = descr_Go;

#else
static refalrts::FnResult func_Go(refalrts::Iter arg_begin, refalrts::Iter arg_end) {
  refalrts::this_is_generated_function();
  // issue here memory for vars with 11 elems
  refalrts::Iter context[11];
  refalrts::zeros( context, 11 );
  //
  context[0] = arg_begin;
  context[1] = arg_end;
  refalrts::move_left( context[0], context[1] );
  refalrts::move_left( context[0], context[1] );
  refalrts::move_right( context[0], context[1] );
  if( ! refalrts::empty_seq( context[0], context[1] ) )
    return refalrts::cRecognitionImpossible;

  refalrts::reset_allocator();
  refalrts::Iter res = arg_begin;
  if( ! refalrts::alloc_open_call( context[2] ) )
    return refalrts::cNoMemory;
  if( ! refalrts::alloc_name( context[3], & Lol ) )
    return refalrts::cNoMemory;
  if( ! refalrts::alloc_open_bracket( context[4] ) )
    return refalrts::cNoMemory;
  if( ! refalrts::alloc_open_call( context[5] ) )
    return refalrts::cNoMemory;
  if( ! refalrts::alloc_name( context[6], & Arg ) )
    return refalrts::cNoMemory;
  if( ! refalrts::alloc_number( context[7], 1UL ) )
    return refalrts::cNoMemory;
  if( ! refalrts::alloc_close_call( context[8] ) )
    return refalrts::cNoMemory;
  if( ! refalrts::alloc_close_bracket( context[9] ) )
    return refalrts::cNoMemory;
  if( ! refalrts::alloc_close_call( context[10] ) )
    return refalrts::cNoMemory;
  refalrts::push_stack( context[10] );
  refalrts::push_stack( context[2] );
  res = refalrts::splice_elem( res, context[10] );
  refalrts::link_brackets( context[4], context[9] );
  res = refalrts::splice_elem( res, context[9] );
  refalrts::push_stack( context[8] );
  refalrts::push_stack( context[5] );
  res = refalrts::splice_elem( res, context[8] );
  res = refalrts::splice_elem( res, context[7] );
  res = refalrts::splice_elem( res, context[6] );
  res = refalrts::splice_elem( res, context[5] );
  res = refalrts::splice_elem( res, context[4] );
  res = refalrts::splice_elem( res, context[3] );
  res = refalrts::splice_elem( res, context[2] );
  refalrts::use( res );
  refalrts::splice_to_freelist( arg_begin, arg_end );
  return refalrts::cSuccess;
}

refalrts::RefalNativeFunction descr_Go(func_Go, "Go");
refalrts::RefalFunction& Go = descr_Go;

#endif

//End of file
