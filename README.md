<!--
# Simple Calculator
-->

Supports basic arithmetic operations and decimal number input.  
Operations are performed on the value in a single register (also the result receiver) and, if the operation is binary, the second operand entered after the operator.

<!--
## Limitations
-->

The size of input numbers is limited to 10 decimal digits.

<!--
## Operations
-->

* Addition: `+`
* Subtraction: `-`
* Multiplication: `*`
* Division: `/`
* Modulus: `%`
* Sign inversion: `_`
* Exponentiation: `^`
* Square root: `SQRT`

<!--
## User Interface
-->

User input is read line-by-line from standard input, with one operation or number (without a sign) expected per line:  
```
op [arg]

number
```

The result of each operation is output to standard output, and error messages are sent to standard error output.

<!--
## Idea
-->

A fold is the sequential application of the same binary operation to a sequence of values.

For example, given a sequence of numbers `1 2 3 4 5` and the operation `+`, you can "fold" the sequence over this operation by specifying an initial value (e.g., `0`):

1. `((((0 + 1) + 2) + 3) + 4) + 5` - left fold
2. `1 + (2 + (3 + (4 + (5 + 0))))` - right fold

<!--
## Task
-->

Expand the calculator implementation to support **left folding** for all binary operations (excluding assignment). The initial value will be the current value in the calculator register, and the sequence of values will be provided by the user via space-separated input.

Syntax:

```
(+) 1 2 34567.8345 9
```
```
(^) 1 2 3
```

The programmatic interface should not change and must still provide `double process_line(double, const std::string &)`.


Let me know if you need further assistance with the content or the implementation of this calculator!
