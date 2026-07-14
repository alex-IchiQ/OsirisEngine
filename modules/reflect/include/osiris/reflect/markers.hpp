#pragma once

// Reflection markers. PROPERTY() becomes a clang annotation while the codegen tool
// parses the header (it defines OSIRIS_CODEGEN) and vanishes in the real build.
//
// CLASS() is an opt-in marker placed before the type for the reader. C++ does not
// allow a class attribute before the "struct"/"class" keyword, so the tool detects a
// reflected type by the presence of PROPERTY() fields rather than by a class attribute.

#define CLASS(...)

#if defined(OSIRIS_CODEGEN)
    #define PROPERTY(...) [[clang::annotate("osiris.property")]]
#else
    #define PROPERTY(...)
#endif
