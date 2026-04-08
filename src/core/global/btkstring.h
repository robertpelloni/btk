/***********************************************************************
*
* BTK compatibility aliases for the CsString annex library.
* These aliases provide an incremental migration path from Cs* naming
* toward BTK-branded public API usage without changing ABI.
*
* All BTK string types are drop-in aliases for the corresponding
* CsString types, providing full Unicode support with explicit
* encoding selection (UTF-8, UTF-16, Latin-1).
*
***********************************************************************/

#ifndef BTK_STRING_COMPAT_H
#define BTK_STRING_COMPAT_H

#include <cs_char.h>
#include <cs_encoding.h>
#include <cs_string.h>
#include <cs_string_iterator.h>
#include <cs_string_view.h>

// BTK namespace alias for the full CsString namespace
namespace BtkText = CsString;

// Core character type
using BtkChar = CsString::CsChar;

// String types with explicit encoding
using BtkString       = CsString::CsString;            // default (UTF-8)
using BtkStringUtf8   = CsString::CsString_utf8;
using BtkStringUtf16  = CsString::CsString_utf16;

// String view types
using BtkStringView      = CsString::CsStringView;
using BtkStringViewUtf8  = CsString::CsStringView_utf8;
using BtkStringViewUtf16 = CsString::CsStringView_utf16;

// Template types for generic encoding/allocator support
template <typename E, typename A>
using BtkBasicString = CsString::CsBasicString<E, A>;

template <typename S>
using BtkBasicStringView = CsString::CsBasicStringView<S>;

template <typename E, typename A>
using BtkStringIterator = CsString::CsStringIterator<E, A>;

template <typename T>
using BtkStringReverseIterator = CsString::CsStringReverseIterator<T>;

// Encoding tags (matching Qt6/JUCE-style explicit encoding)
using BtkEncodingUtf8  = CsString::CsEncoding_Utf8;
using BtkEncodingUtf16 = CsString::CsEncoding_Utf16;

#endif
