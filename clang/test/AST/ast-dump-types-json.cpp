// RUN: %clang_cc1 -triple x86_64-unknown-unknown -fsyntax-only -ast-dump=json -ast-dump-filter Test %s | FileCheck %s

namespace NS {
struct S {};
}

struct T {
  int I;
  void F();
};

typedef struct T TestElaboratedType1;
typedef NS::S TestElaboratedType2;

#define CDECL __attribute__((cdecl))
typedef void (CDECL *TestMacroQualifiedType)();

typedef void (T::* TestMemberFunctionPointerType)();
typedef int T::*TestMemberDataPointerType;

typedef int TestQualTypePrinting(const char* c);

typedef int TestUsingShadowDeclType;
namespace TestNamespaceWithUsingShadowType {
using ::TestUsingShadowDeclType;
}

// NOTE: CHECK lines have been autogenerated by gen_ast_dump_json_test.py


// CHECK:  "kind": "TypedefDecl",
// CHECK-NEXT:  "loc": {
// CHECK-NEXT:   "offset": 204,
// CHECK-NEXT:   "file": "{{.*}}",
// CHECK-NEXT:   "line": 12,
// CHECK-NEXT:   "col": 18,
// CHECK-NEXT:   "tokLen": 19
// CHECK-NEXT:  },
// CHECK-NEXT:  "range": {
// CHECK-NEXT:   "begin": {
// CHECK-NEXT:    "offset": 187,
// CHECK-NEXT:    "col": 1,
// CHECK-NEXT:    "tokLen": 7
// CHECK-NEXT:   },
// CHECK-NEXT:   "end": {
// CHECK-NEXT:    "offset": 204,
// CHECK-NEXT:    "col": 18,
// CHECK-NEXT:    "tokLen": 19
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "name": "TestElaboratedType1",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "desugaredQualType": "T",
// CHECK-NEXT:   "qualType": "struct T"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ElaboratedType",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "struct T"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "RecordType",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "T"
// CHECK-NEXT:      },
// CHECK-NEXT:      "decl": {
// CHECK-NEXT:       "id": "0x{{.*}}",
// CHECK-NEXT:       "kind": "CXXRecordDecl",
// CHECK-NEXT:       "name": "T"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:  "kind": "TypedefDecl",
// CHECK-NEXT:  "loc": {
// CHECK-NEXT:   "offset": 239,
// CHECK-NEXT:   "file": "{{.*}}",
// CHECK-NEXT:   "line": 13,
// CHECK-NEXT:   "col": 15,
// CHECK-NEXT:   "tokLen": 19
// CHECK-NEXT:  },
// CHECK-NEXT:  "range": {
// CHECK-NEXT:   "begin": {
// CHECK-NEXT:    "offset": 225,
// CHECK-NEXT:    "col": 1,
// CHECK-NEXT:    "tokLen": 7
// CHECK-NEXT:   },
// CHECK-NEXT:   "end": {
// CHECK-NEXT:    "offset": 239,
// CHECK-NEXT:    "col": 15,
// CHECK-NEXT:    "tokLen": 19
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "name": "TestElaboratedType2",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "desugaredQualType": "NS::S",
// CHECK-NEXT:   "qualType": "NS::S"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "ElaboratedType",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "NS::S"
// CHECK-NEXT:    },
// CHECK-NEXT:    "qualifier": "NS::",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "RecordType",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "NS::S"
// CHECK-NEXT:      },
// CHECK-NEXT:      "decl": {
// CHECK-NEXT:       "id": "0x{{.*}}",
// CHECK-NEXT:       "kind": "CXXRecordDecl",
// CHECK-NEXT:       "name": "S"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:  "kind": "TypedefDecl",
// CHECK-NEXT:  "loc": {
// CHECK-NEXT:   "offset": 319,
// CHECK-NEXT:   "file": "{{.*}}",
// CHECK-NEXT:   "line": 16,
// CHECK-NEXT:   "col": 22,
// CHECK-NEXT:   "tokLen": 22
// CHECK-NEXT:  },
// CHECK-NEXT:  "range": {
// CHECK-NEXT:   "begin": {
// CHECK-NEXT:    "offset": 298,
// CHECK-NEXT:    "col": 1,
// CHECK-NEXT:    "tokLen": 7
// CHECK-NEXT:   },
// CHECK-NEXT:   "end": {
// CHECK-NEXT:    "offset": 319,
// CHECK-NEXT:    "col": 22,
// CHECK-NEXT:    "tokLen": 22
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "name": "TestMacroQualifiedType",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "CDECL void (*)()"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "PointerType",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "CDECL void (*)()"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "MacroQualifiedType",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "CDECL void ()"
// CHECK-NEXT:      },
// CHECK-NEXT:      "macroName": "CDECL",
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "AttributedType",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "void () __attribute__((cdecl))"
// CHECK-NEXT:        },
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ParenType",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "void ()"
// CHECK-NEXT:          },
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "FunctionProtoType",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "void ()"
// CHECK-NEXT:            },
// CHECK-NEXT:            "cc": "cdecl",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "BuiltinType",
// CHECK-NEXT:              "type": {
// CHECK-NEXT:               "qualType": "void"
// CHECK-NEXT:              }
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "ParenType",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "void ()"
// CHECK-NEXT:          },
// CHECK-NEXT:          "inner": [
// CHECK-NEXT:           {
// CHECK-NEXT:            "id": "0x{{.*}}",
// CHECK-NEXT:            "kind": "FunctionProtoType",
// CHECK-NEXT:            "type": {
// CHECK-NEXT:             "qualType": "void ()"
// CHECK-NEXT:            },
// CHECK-NEXT:            "cc": "cdecl",
// CHECK-NEXT:            "inner": [
// CHECK-NEXT:             {
// CHECK-NEXT:              "id": "0x{{.*}}",
// CHECK-NEXT:              "kind": "BuiltinType",
// CHECK-NEXT:              "type": {
// CHECK-NEXT:               "qualType": "void"
// CHECK-NEXT:              }
// CHECK-NEXT:             }
// CHECK-NEXT:            ]
// CHECK-NEXT:           }
// CHECK-NEXT:          ]
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:  "kind": "TypedefDecl",
// CHECK-NEXT:  "loc": {
// CHECK-NEXT:   "offset": 366,
// CHECK-NEXT:   "file": "{{.*}}",
// CHECK-NEXT:   "line": 18,
// CHECK-NEXT:   "col": 20,
// CHECK-NEXT:   "tokLen": 29
// CHECK-NEXT:  },
// CHECK-NEXT:  "range": {
// CHECK-NEXT:   "begin": {
// CHECK-NEXT:    "offset": 347,
// CHECK-NEXT:    "col": 1,
// CHECK-NEXT:    "tokLen": 7
// CHECK-NEXT:   },
// CHECK-NEXT:   "end": {
// CHECK-NEXT:    "offset": 397,
// CHECK-NEXT:    "col": 51,
// CHECK-NEXT:    "tokLen": 1
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "name": "TestMemberFunctionPointerType",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "void (T::*)()"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "MemberPointerType",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "void (T::*)()"
// CHECK-NEXT:    },
// CHECK-NEXT:    "isFunction": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "RecordType",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "T"
// CHECK-NEXT:      },
// CHECK-NEXT:      "decl": {
// CHECK-NEXT:       "id": "0x{{.*}}",
// CHECK-NEXT:       "kind": "CXXRecordDecl",
// CHECK-NEXT:       "name": "T"
// CHECK-NEXT:      }
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "ParenType",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "void ()"
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "FunctionProtoType",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "void ()"
// CHECK-NEXT:        },
// CHECK-NEXT:        "cc": "cdecl",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "BuiltinType",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "void"
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:  "kind": "TypedefDecl",
// CHECK-NEXT:  "loc": {
// CHECK-NEXT:   "offset": 416,
// CHECK-NEXT:   "file": "{{.*}}",
// CHECK-NEXT:   "line": 19,
// CHECK-NEXT:   "col": 17,
// CHECK-NEXT:   "tokLen": 25
// CHECK-NEXT:  },
// CHECK-NEXT:  "range": {
// CHECK-NEXT:   "begin": {
// CHECK-NEXT:    "offset": 400,
// CHECK-NEXT:    "col": 1,
// CHECK-NEXT:    "tokLen": 7
// CHECK-NEXT:   },
// CHECK-NEXT:   "end": {
// CHECK-NEXT:    "offset": 416,
// CHECK-NEXT:    "col": 17,
// CHECK-NEXT:    "tokLen": 25
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "name": "TestMemberDataPointerType",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int T::*"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "MemberPointerType",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int T::*"
// CHECK-NEXT:    },
// CHECK-NEXT:    "isData": true,
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "RecordType",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "T"
// CHECK-NEXT:      },
// CHECK-NEXT:      "decl": {
// CHECK-NEXT:       "id": "0x{{.*}}",
// CHECK-NEXT:       "kind": "CXXRecordDecl",
// CHECK-NEXT:       "name": "T"
// CHECK-NEXT:      }
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "BuiltinType",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      }
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:  "kind": "TypedefDecl",
// CHECK-NEXT:  "loc": {
// CHECK-NEXT:   "offset": 456,
// CHECK-NEXT:   "file": "{{.*}}",
// CHECK-NEXT:   "line": 21,
// CHECK-NEXT:   "col": 13,
// CHECK-NEXT:   "tokLen": 20
// CHECK-NEXT:  },
// CHECK-NEXT:  "range": {
// CHECK-NEXT:   "begin": {
// CHECK-NEXT:    "offset": 444,
// CHECK-NEXT:    "col": 1,
// CHECK-NEXT:    "tokLen": 7
// CHECK-NEXT:   },
// CHECK-NEXT:   "end": {
// CHECK-NEXT:    "offset": 490,
// CHECK-NEXT:    "col": 47,
// CHECK-NEXT:    "tokLen": 1
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "name": "TestQualTypePrinting",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int (const char *)"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "FunctionProtoType",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int (const char *)"
// CHECK-NEXT:    },
// CHECK-NEXT:    "cc": "cdecl",
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "BuiltinType",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "int"
// CHECK-NEXT:      }
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x{{.*}}",
// CHECK-NEXT:      "kind": "PointerType",
// CHECK-NEXT:      "type": {
// CHECK-NEXT:       "qualType": "const char *"
// CHECK-NEXT:      },
// CHECK-NEXT:      "inner": [
// CHECK-NEXT:       {
// CHECK-NEXT:        "id": "0x{{.*}}",
// CHECK-NEXT:        "kind": "QualType",
// CHECK-NEXT:        "type": {
// CHECK-NEXT:         "qualType": "const char"
// CHECK-NEXT:        },
// CHECK-NEXT:        "qualifiers": "const",
// CHECK-NEXT:        "inner": [
// CHECK-NEXT:         {
// CHECK-NEXT:          "id": "0x{{.*}}",
// CHECK-NEXT:          "kind": "BuiltinType",
// CHECK-NEXT:          "type": {
// CHECK-NEXT:           "qualType": "char"
// CHECK-NEXT:          }
// CHECK-NEXT:         }
// CHECK-NEXT:        ]
// CHECK-NEXT:       }
// CHECK-NEXT:      ]
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:  "kind": "TypedefDecl",
// CHECK-NEXT:  "loc": {
// CHECK-NEXT:   "offset": 506,
// CHECK-NEXT:   "file": "{{.*}}",
// CHECK-NEXT:   "line": 23,
// CHECK-NEXT:   "col": 13,
// CHECK-NEXT:   "tokLen": 23
// CHECK-NEXT:  },
// CHECK-NEXT:  "range": {
// CHECK-NEXT:   "begin": {
// CHECK-NEXT:    "offset": 494,
// CHECK-NEXT:    "col": 1,
// CHECK-NEXT:    "tokLen": 7
// CHECK-NEXT:   },
// CHECK-NEXT:   "end": {
// CHECK-NEXT:    "offset": 506,
// CHECK-NEXT:    "col": 13,
// CHECK-NEXT:    "tokLen": 23
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "name": "TestUsingShadowDeclType",
// CHECK-NEXT:  "type": {
// CHECK-NEXT:   "qualType": "int"
// CHECK-NEXT:  },
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "BuiltinType",
// CHECK-NEXT:    "type": {
// CHECK-NEXT:     "qualType": "int"
// CHECK-NEXT:    }
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }


// CHECK:  "kind": "NamespaceDecl",
// CHECK-NEXT:  "loc": {
// CHECK-NEXT:   "offset": 541,
// CHECK-NEXT:   "file": "{{.*}}",
// CHECK-NEXT:   "line": 24,
// CHECK-NEXT:   "col": 11,
// CHECK-NEXT:   "tokLen": 32
// CHECK-NEXT:  },
// CHECK-NEXT:  "range": {
// CHECK-NEXT:   "begin": {
// CHECK-NEXT:    "offset": 531,
// CHECK-NEXT:    "col": 1,
// CHECK-NEXT:    "tokLen": 9
// CHECK-NEXT:   },
// CHECK-NEXT:   "end": {
// CHECK-NEXT:    "offset": 609,
// CHECK-NEXT:    "line": 26,
// CHECK-NEXT:    "col": 1,
// CHECK-NEXT:    "tokLen": 1
// CHECK-NEXT:   }
// CHECK-NEXT:  },
// CHECK-NEXT:  "name": "TestNamespaceWithUsingShadowType",
// CHECK-NEXT:  "inner": [
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "UsingDecl",
// CHECK-NEXT:    "loc": {
// CHECK-NEXT:     "offset": 584,
// CHECK-NEXT:     "line": 25,
// CHECK-NEXT:     "col": 9,
// CHECK-NEXT:     "tokLen": 23
// CHECK-NEXT:    },
// CHECK-NEXT:    "range": {
// CHECK-NEXT:     "begin": {
// CHECK-NEXT:      "offset": 576,
// CHECK-NEXT:      "col": 1,
// CHECK-NEXT:      "tokLen": 5
// CHECK-NEXT:     },
// CHECK-NEXT:     "end": {
// CHECK-NEXT:      "offset": 584,
// CHECK-NEXT:      "col": 9,
// CHECK-NEXT:      "tokLen": 23
// CHECK-NEXT:     }
// CHECK-NEXT:    },
// CHECK-NEXT:    "name": "::TestUsingShadowDeclType"
// CHECK-NEXT:   },
// CHECK-NEXT:   {
// CHECK-NEXT:    "id": "0x{{.*}}",
// CHECK-NEXT:    "kind": "UsingShadowDecl",
// CHECK-NEXT:    "loc": {
// CHECK-NEXT:     "offset": 584,
// CHECK-NEXT:     "col": 9,
// CHECK-NEXT:     "tokLen": 23
// CHECK-NEXT:    },
// CHECK-NEXT:    "range": {
// CHECK-NEXT:     "begin": {
// CHECK-NEXT:      "offset": 584,
// CHECK-NEXT:      "col": 9,
// CHECK-NEXT:      "tokLen": 23
// CHECK-NEXT:     },
// CHECK-NEXT:     "end": {
// CHECK-NEXT:      "offset": 584,
// CHECK-NEXT:      "col": 9,
// CHECK-NEXT:      "tokLen": 23
// CHECK-NEXT:     }
// CHECK-NEXT:    },
// CHECK-NEXT:    "isImplicit": true,
// CHECK-NEXT:    "target": {
// CHECK-NEXT:     "id": "0x{{.*}}",
// CHECK-NEXT:     "kind": "TypedefDecl",
// CHECK-NEXT:     "name": "TestUsingShadowDeclType"
// CHECK-NEXT:    },
// CHECK-NEXT:    "inner": [
// CHECK-NEXT:     {
// CHECK-NEXT:      "id": "0x0"
// CHECK-NEXT:     }
// CHECK-NEXT:    ]
// CHECK-NEXT:   }
// CHECK-NEXT:  ]
// CHECK-NEXT: }
