```json
[
  {
    "name": "(str) NT API name, used for debugging. (optional)",
    "sysnum": "(int) System call ID. (required)",
    "argnum": "(int) Number of arguments the API takes. (required)",
    "idx": "(int) Order of API invocation; currently unused. (optional)",
    "arg%n": {
      "kind": "(str) Type of the argument. Includes 'outptr', 'inptr', 'struct', 'funcptr', 'byte', 'word', 'dword', 'qword'. (required)",
      "id": "(int) ID referencing a value in a memory array, used if argument is dependent. (required for: 'outptr', 'retval')",
      "size": "(int) Size of the argument in bytes. (required for: 'outptr', 'inptr', 'struct')",
      "val": "(varies) Actual value of the argument, format depends on 'kind'. (required for: 'struct', 'inptr', 'primitive types')",
      "offset": "(int) Offset within a structure, if applicable. (required for: 'struct')"
    }
    // Additional arguments can follow the same structure.
  }
]

```
