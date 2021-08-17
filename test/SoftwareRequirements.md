# Functional Requirements

- shall be a console application
- shall input formula and equation directly as an argument
- shall have an "interactive mode" to enter equations per line
- shall have basic functions:
	- math functions: (+,-,*,/,power,modulus)
	- unary functions: (-,sqrt,cubert)
	- transcendental functions: (sin, cos, tan, asin, acos, atan)
	- hyperbolic functions: (sinh, cosh, tanh, asinh, acosh, atanh)
	- unit conversion: (length, area, volume, speed, acceleration)
- shall be able to enter
	- integer number
	- double floating number (with '.' or ',' decimals)
	- date-time format
	- fractional (used for inches, for example)
- shall be able to output numbers:
	- decimal, cleaned (no trailing '0')
	- formatted decimal (for example: '%.2f')
	- string formats (such as '$###.##')
	- fractional format (such as '2 1/4')

# Design Requirements

- shall have an 'execution' class that handles all entered strings.
- shall have its own 'string' manipulation class
- shall have a 'number' class to handle numbers with:
	- double and integer expressions (handled separately and intrinsically)
	- shall contain 'units' class to associate number with units
	- shall contain 'format string' (as part of 'string' class) to allow numeric expression, as inputs or outputs
- shall have a 'function' class used to apply numeric functions to numeric class
