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
(+) 1 2 34567.8345 9
```
```
(^) 1 2 3
```

The programmatic interface should not change and must still provide `double process_line(double, const std::string &)`.


Let me know if you need further assistance with the content or the implementation of this calculator!
