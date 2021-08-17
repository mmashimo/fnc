# fnc - Yet Another Console Calculator

fnc (pronouced "funk") is yet another console calculator program equivalent to 'dc' (RPN) or 'bc'. Hopefully, this does not replace other great GUI based calculator programs out there. I just needed a simple command line calculator to simplify calculations on the fly.

### Target (desired/required) features

- Simple expression calculator
    - Process expression computation both interactive or command line.
    - Expressions requiring variables can be added when running.
- Unit-conversions; formulas that can specify units and convert. Such as (for starters):
	- Temperature conversion (ex. F<>C)
	- Conversion of degrees into radian for transcendental calculations (degrees<>radians)
	- Length conversion (ex. in<>mm or in<>cm)
	- Unit conversions using the same simple expressions parser
- Variable parsing (definition) and saving (assignment)

### Feature details
- Non-hierarchical expression (read from left-to-right like 'bc') - similar to punching number into a calculator. Explicit hierarchy (using parentheses to establish precedence and nesting) is intuitive for calculators when punching numbers and functions in series. Few things to make life easier:
	- Unary functions can be expressed either way: (ex.) _60 sin_ or _sin(60)_ - allowing for functions to work left-to-right, again similar to punching numbers into a calculator.
	- Algebraic multiplication of variables (ex. _2pi_ or _pi 2_ - note space to differentiate variable named _pi2_ which can be part of a variable constant)
- Do metric and/or angle conversions on the fly. Units entered after numbers.
	- Implied angle is "Degrees" (is configurable)
	- Have degree-radian convert to radians when doing transcendentals
	- automatically adjust for units algebraically (TBD)
- Resolve variables - parsed or on-the-fly 
	- Variables define a number when parsing (ex. _y=3.2_ ). (NOTE must be a number only)
	- If a undefined variable is in an expression, _fnc_ prompts for a number (or an expression that returns a number)
	- Save current (or last) result in a variable (ex. _4.3*y=y_ - this will ask for _y_ and replace it in _y_)
	- Create/insert formulas (from text files) where variables are added (TBD)
	- Save and recall functions(equations) and variables/numbers stored in straight text (TBD).
- Calculate using various floating point resolution including decimal-64 as well as binary formats (binary-64) - seamlessly by defining numeric representation (similar to rad/degrees) (TBD)
	- Convert/resolve using long-doubles (binary-128/deciml-128), also

### Some quirks of command line processing
These are specific key entries you may already know (or will need to know):<br>
- _bash_ has argument processing so it is recommended to put all equations in quotes (ex. _./fnc "4 * 5"_).
	- If you want to avoid double-quotes on multiplication, attach to number or variable (ex _./fnc 4*5_) or when using variables, use algebraic expression (ex. 2pi pr)
	- Standalone multiplication function ('*') will need to be in attached befefore or after number/variable. This is because an asterick returns all the file/subdirectories in current directory.
	- '(...)' have functional replacements in _bash_.
	- same with '>' and '|'.
- Windows10 may need to have elevated terminals to allow access when internal files are used in protected areas.
- Beware of using environment variables - it works but may behave funny. In Windows, UNICODE has a very funky behavior with fnc.

--------------------------------
# Builds
## Prerequisites:
- Boost library - required (math and properties) - NOTE: if CMake cannot find it, set INCLUDE

BUILD on Linux:
- md build
- cd build
- cmake ..
- make

BUILD on Windows (MSVC solution)
- md vc
- cd vc
- cmake ..
- Run (MSVC 2015) or better on 'fnc.sln'
- NOTE: if you have 'boost' installed locally, it is best to add an environment "INCLUDE" to the directory (set INCLUDE=<dir>). Otherwise CMake cannot find boost.

--------------------------------
# Functions:
## Basic Rules

Generally, the script works in the following manner:

- Parsing happens from left to right. Case is important.
- Numberic or '-' followed by numerics are 'numbers'.
	- A number can have units immediately after the number or by adding ':' and units the number (ex. _25F_ is the same as _25:F_ for degrees Fahrenheit). Units are alpha-numeric (but starts with an alphabet).
	- Number formatting is similar to Units, but is always enclosed in single quotes. It is important to make sure there is an ending quote unless using escape (\') for feet or minutes,
	for example. 5'6" (5-feet, 6-inches) is good enough. 5 feet cannot use 5:' otherwise parsing
	error will ensue.
	- Time string (TBD) is differentiated with units/format by ##:##. Numericall, saved in the
	in seconds by default unless explicitly set. 12:56 is 12-minutes, 56-seconds. 12:56m is
	12-hours 56-minutes. 12:25d will format December 25 in days from January 1st (minus one).
	- Time-Date format is separated by underscore (#). 1999-12-31#23:59:59 is stored as J2000,
	but in seconds rather than fractional part of a day
	- Differentiate units from variables if not sure. Add a space in front of variables - especially if a variable name exists as a unit name (ex. _2m_ is 2 meters, _2 m_ will multiply _2_ to _m_)
	- You also add C-style formatting by adding ":'%.9f'" after the number. (ex. _25:F:'%0.1f'_)
	- Hexidecimal expression starts with "0x" - ends when alpha-numeric is no longer 0-9 and A-F (case not important). It is always best to add a space if not sure. Note: _0x19C_ is not the same as _0x19:C_. When computing with hexadecimal, it may help to have unit-less conversions
- Variable names must start with alphabet (a-z, A-Z). Variables can have alpha-numerics (TBD-bugs exists with alpha with numbers). Note that units are also alphanumerics starting with an alphabet. Be careful that undefined units will become a variable.
	- Variable definition (_<var>=###_) or variable assignment (_=<var>_) cannot have spaces between "=" and _<var>_ name. Parsing "=" cannot recognize the start/end of variable name.
	- Variable definition (_<var>=###_) is processed during parsing.
	- Variable assignment (_=<var>_) is processed when executing code.
- Parenthesis is necessary to establish hierarchical structure.
	- Left-to-right representations do not require parenthesis on left of a function. Function processing with right function with parenthesis will be processed first. This allows ex. 30sin to be the same as sin(30).
	- Binary functions have left to right entries. Note the implementation is algebraic so _3.4 5.2_ is the same as _3.4 * 5.2_ - allowing variables to be multiplied correctly: _3.4y_ (may have bugs if deep in code)
	- Unary functions are performed by function (ex. sin(30)) or sequential (ex. 30sin)

## Binary (common)

- +, -, *, and /
- ^ (power)
- % (modulus)
- :: (convert units) - second parameter can include formatting (if ':%' set - TBD)

## Unary

- sqrt (square root), cbrt (cube root)
- inv (1/x - invert)
- exp (e^x), ln (natural log)
- e10x (10^x), log (log-base 10)
- abs (absolute)
- sin, cos and tan - assumed degree unless unit is rad
- sinh, cosh and tanh

## Parametrizing

- (...) - parenthesis function containment
- |...| - absolute value (TBD - '|' pipe is problematic on linux)
- [<fileName>] - Formula retrieval (TBD - esp. CSV files)

## Memory
- <_var_>=### - applies number ### to <var> name.
- =<_var_> - saves current number (results) to named variable <_var_>.
- <_var_> - retrieves saved value <_var_>. If not set, 'fnc' will request input.

## Examples

- In command line argument, simple calculation:
	> fnc 12.3+4.56<br>
	> **16.860000**<br>
- Use of unit-conversions (converts known units like temperature):
	> fnc 25C::F   (NOTE case sensitivity)<br>
	> **77F**
- Use of constants (such as 'pi'):
	> fnc 180/2pi<br>
	> **282.7433**
- Use built-in functions (assumes base as degrees - if set in INI file):
	> fnc sin(30)<br>
	> **0.5**<br>
	> fnc sin(2pi)  (NOTE: numbers with 'pi' use radians automatically)<br>
	> **0**<br>
	> fnc pi:deg sin   (or 'sin(pi:deg)' converts converts back to deg)<br>
	> **0.054803665**<br>
	> fnc asin(0.5deg)  (assumed to be in 'degrees' as configured)<br>
	> **30**<br>
- View of multiple results within a program (note each result is saved):
	> fnc sin(30) 4pi   (NOTE separate function after processing 'sin(60)')<br>
	> **0.5**<br>
	> **12.566371**<br>


# How this works
'fnc' has two (2) internal parsers to make sure functions, numbers and variables are separated correctly. The parser works on the principle of use-case driven and syntactical approximation. Input breakdown has a general form that users are likely to input functions by general mathematical syntax: for example: binary functions (+,-,*,/) would be: ###-function-### and unary functions are either ###-function (as if to key in on a calculator) or algebraically, function(###) - using parentheses. Algebraic functions are similar using variables: x-function-y or algebraically, function(x,y)=... where the algebraic terms use x and y (in ...). Finally, syntax allows for stack oriented programming (also known as RPN or Reverse Polish Notation) where, like micropressor or ALU functions, it follows ###-###-function. The latter is possible through syntax since 'fnc' processes functions on a stack. The orientation is always the same after it has been parsed. Sub-functioning, or better illustrate as expanding a stack to include sub-stack, is done through parenthesis. In this structure, however, implicit hierarchy cannot established (for example: 1 + 2 * 3 will yield 9 rather than 7 - since it reads left to right). The reason hierarchy is not set is because we are prioritizing by stack orientation.

Few notes about number entry: Numbers are read in sequence and very sensitive to spaces. All numbers start with check for '-' (negative sign) immediately followed by a number. Please note at a negative fractional decimal value must have a '0' in front of the decimal-point (-0.1 rather than -.1) otherwise it is not possible to distinguish subtraction with decimal numbers (-.1 will subtract 0.1 from previous value).