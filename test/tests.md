
## Bug Tracking

Bug List:
--------
[x] - [5/0],    <== Has an exception. This should abort operation, not divide-by-zero exception.
				<== It should return a single [NaN] number
[ ] - [1in::cm] <== Parsing error

Bug Tests:
---------
1) "args": [132in*55::ft],  // (infinite loop and exception)
2) "args": [2pi::deg],      // exception; "units" and "format" are not passed in multiplication/function
3) "args": ["45-6"],        // Was returning '45'

## Input Tests:

	// "args": ["25C::F"],
	// "args": ["y=3.2 2pi*y+y=y 5.2-y"],
	// "args": ["y=3.2 2pi*y+y=pi 5.2-y"],
	// "args": ["2pi/180"],
	// "args": ["1in::cm"],
	// "args": ["4-(.5/(3+4))"],     // Should return 3.92857...
	// "args": ["4-(.5/7)"],     // Should return 3.92857...
	// "args": ["4-.5/7"],    // Should return .5
	// "args": ["30 sin"],
	// "args": ["0.5rad asin"],
	// "args": ["sin(60)","5.6","*", "cos(30)"],
	// "args": ["6.*(9-1)/inv(10.0+2)-4.2"],
	// "args": ["3.4","*","5.3"],   // Funkiness on linux
	// "args": ["3.4 5.3"],         // default multiply
	// "args": ["0x5A * 0x48"]
	// "args": ["2y"],

Tests:
-----
1) "args": ["4-.5/7"],  // This looks like 4 and -.5, but '-' followed is followed by '.' which does not make it a number
2) "args": ["4 -5/7"],  // This looks like 4 and -5, if parsed in number sequence. This should parse as '4-5'
3) "args": ["sin(60)","5.6","*", "cos(30)"], // due to lack of func between sin(60) and 5.6, it is pushed on the stack
4) "args": ["6.*(9-1)/inv(10.0+2)-4.2"],
5) "args": ["2pi/180"],
6) "args": ["25C::F"],
7) "args": ["1in::mm"],

