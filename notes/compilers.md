# VMPI
VMPI is a Message Passing Interface library written by Valve, it is used to handle compile clusters for maps.
Its original code is mostly broken and/or lost, which means it needs to go; although we still want this functionality.
It will be replaced by a custom implementation, which hopefully will not be abandoned like its Valve cousin.
Additionally, VMPI also handled a MySQL database with statistics on compile times and general custer info, would be nice to still have it.


# Additional compiler args file (`cfg/commandline.cfg`)
All the map compilers accept additional arguments via this file, which should be located inside the mod's `cfg` directory.
It consists in kv1-formatted data, with a main key `CommandLine` with child kv-pairs with the compiler's name as key and additional args as value.

### Example
```kdl
// mod_hl2mp/cfg/commandline.cfg
"CommandLine" {
    "vvis" "-fast"
    "vbsp" "-game hl2"
}
```
In this example, we added the `-fast` flag to the vvis compiler, and a `-game hl2` arg to vbsp,
which will cause for vvis to compile in fast mode and for vbsp to add hl2 as a search path respectively.


# Exception handling
On windows, Source handles exceptions (see table bellow) with a custom message box (confirm?).
But when tools are used, there is an additional minidump step before it,
which still uses the same [Structured Exception Handling](https://learn.microsoft.com/en-us/windows/win32/debug/structured-exception-handling) system, which needs reimplementation for Linux.

| Exception type                     | Description                                                                                                                                                                                                            |
|------------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| EXCEPTION_ACCESS_VIOLATION         | The thread tried to read from or write to a virtual address for which it does not have the appropriate access.                                                                                                         |
| EXCEPTION_ARRAY_BOUNDS_EXCEEDED    | The thread tried to access an array element that is out of bounds and the underlying hardware supports bounds checking.                                                                                                |
| EXCEPTION_BREAKPOINT               | A breakpoint was encountered.                                                                                                                                                                                          |
| EXCEPTION_DATATYPE_MISALIGNMENT    | The thread tried to read or write data that is misaligned on hardware that does not provide alignment. For example, 16-bit values must be aligned on 2-byte boundaries; 32-bit values on 4-byte boundaries, and so on. |
| EXCEPTION_FLT_DENORMAL_OPERAND     | One of the operands in a floating-point operation is denormal. A denormal value is one that is too small to represent as a standard floating-point value.                                                              |
| EXCEPTION_FLT_DIVIDE_BY_ZERO       | The thread tried to divide a floating-point value by a floating-point divisor of zero.                                                                                                                                 |
| EXCEPTION_FLT_INEXACT_RESULT       | The result of a floating-point operation cannot be represented exactly as a decimal fraction.                                                                                                                          |
| EXCEPTION_FLT_INVALID_OPERATION    | This exception represents any floating-point exception not included in this list.                                                                                                                                      |
| EXCEPTION_FLT_OVERFLOW             | The exponent of a floating-point operation is greater than the magnitude allowed by the corresponding type.                                                                                                            |
| EXCEPTION_FLT_STACK_CHECK          | The stack overflowed or underflowed as the result of a floating-point operation.                                                                                                                                       |
| EXCEPTION_FLT_UNDERFLOW            | The exponent of a floating-point operation is less than the magnitude allowed by the corresponding type.                                                                                                               |
| EXCEPTION_ILLEGAL_INSTRUCTION      | The thread tried to execute an invalid instruction.                                                                                                                                                                    |
| EXCEPTION_IN_PAGE_ERROR            | The thread tried to access a page that was not present, and the system was unable to load the page. For example, this exception might occur if a network connection is lost while running a program over the network.  |
| EXCEPTION_INT_DIVIDE_BY_ZERO       | The thread tried to divide an integer value by an integer divisor of zero.                                                                                                                                             |
| EXCEPTION_INT_OVERFLOW             | The result of an integer operation caused a carry out of the most significant bit of the result.                                                                                                                       |
| EXCEPTION_INVALID_DISPOSITION      | An exception handler returned an invalid disposition to the exception dispatcher. Programmers using a high-level language such as C should never encounter this exception.                                             |
| EXCEPTION_NONCONTINUABLE_EXCEPTION | The thread tried to continue execution after a noncontinuable exception occurred.                                                                                                                                      |
| EXCEPTION_PRIV_INSTRUCTION         | The thread tried to execute an instruction whose operation is not allowed in the current machine mode.                                                                                                                 |
| EXCEPTION_SINGLE_STEP              | A trace trap or other single-instruction mechanism signaled that one instruction has been executed.                                                                                                                    |
| EXCEPTION_STACK_OVERFLOW           | The thread used up its stack.                                                                                                                                                                                          |
