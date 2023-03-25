# calc

This is a simple calculator that can handle simple math, complex math and binary math. The source code is highly extensible and it might get more features soon. You may also add features (go down for the source code tour)

## Features

- Simple math and binary and complex math using standard C syntax (with some extensions, XOR is missing)
- Hidden operator: Expressions like `2a` or `2(3 + 4)` are converted to `2*a` and `2*(3 + 4)` respectively
- Commands: Call commands using the # symbol (available commands can be shown using `#help`)
- Different output modes (binary, hexadecimal, octal, decimal); change the output mode using `#output bin|hex|dec|oct`
- Variable creation: `a=3` `b=2*a`
- Static variable creation: `a=3` `b=2*a` (b is now equal to 6 whereas the above b is stored as `2*a`
- Function declaration: `f(x) = x^2` `f(x, y) = (x + y)^2`
- System functions: System functions are implemented using C code and implemented in the calculator internally
- Register: Access a register via `$n` where `n` is a valid number. Register are used to store past results of computations

## How it works
Inside main, command line arguments like `-b` are parsed to change output mode and when an expression is supplied, this expression is executed. When not, the main calculator starts up and expressions, commands or assigns can be typed.<br>Next, the line which was written by the user is tokenized (`token.c`) and then either sent off to the parser (`parser.c`) or to the command parser (`commands.c`) if the line starts with a hash.<br>If there is an equal sign(=) or assign operator(:=) anywhere, the parser parses the variable or function name and then continues onwards after the operator. Then the parser transforms the tokens into an internal data structure called `GROUP` which is just a number. But groups like the add group expect two more groups to follow in the group array. Group arrays are used internally to hold the value of variables and functions and the currently parsed expression. <br> If no errors occured, the last step takes place which is computing. The `compute` function takes in a context variable which holds the working group (this function is recursive) and a destination value. As of now, there are three values: `VTYPE_INT`, `VTYPE_FLOAT` and `VTYPE_COMPLEX`. The combinations define which value types can be operated upon and which function to use for that. The operations table is giving the offsets of the operations (this table should probably generated on runtime because changing one table entry of combinations may cause all offsets to change).

## Contribution
If you want to use the calculator for personal use and want to make it better, consider contributing by making commands, internal functions or, more advanced, add syntax rules for special constructs. I also wanted to add a plugin manager to load commands or internal functions on runtime but haven't gotten around to do so. For shared object loading, there is some code hanging around in `commands.c`. Thank you for your time!
