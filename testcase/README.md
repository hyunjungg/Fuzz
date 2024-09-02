## Json Format 

```json
[
  {
    "name": "(str) NT API name, used for debugging. (optional)",
    "sysnum": "(int) System call ID. (required)",
    "argnum": "(int) Number of arguments the API takes. (required)",
    "idx": "(int) Order of API invocation; currently unused. (optional)",
    "arg%n": {
      "kind": "(str) Type of the argument. Includes 'inptr', 'string', 'struct', 'funcptr', 'byte', 'word', 'dword', 'qword'. (required)",
      "id": "(int) ID referencing a value in a memory array, used if argument is dependent. (required for: 'inptr(out)', 'retval')",
      "size": "(int) Size of the argument in bytes. (required for: 'outptr', 'inptr')",
      "val": "(varies) Actual value of the argument, format depends on 'kind'. (required for: 'struct', 'inptr', 'primitive types')",
      "offset": "(int) Offset within a structure, if applicable. (required for: 'struct')"
    }
    // Additional arguments can follow the same structure.
  }
]

```



**kind: inptr**

- required fields: `size`

- optional fields : `val`, `id`

- `id` field is required only if the data will be used in the next API call

- `val` field is required only if data needs to be filled in that argument.

  Example:

  ```
  "kind": "inptr",
  "size": 48,
  "val": {
      "kind": "dword",
      "val": 3
  	}
  ```

**kind: struct**

- required fields: `val`

- `val` must be a JSON list

- Each element in the JSON list must include an offset field

  Example:

  ```
  "kind": "struct",
  "val": [
    {
      "offset": 0,
      "kind": "dword",
      "val": 2622347264
    },
    {
      "offset": 4,
      "kind": "dword",
      "val": 30859141
    }
  ]
  ```

**kind: string**

- required fields: `val`

- A parent `kind: inptr` must exist

  Example:

  ```
  "kind": "inptr",
  "size": 42,
  "val": {
    "kind": "str",
    "val": "\\??\\C:\\Temp\\test.txt"
  }
  ```
