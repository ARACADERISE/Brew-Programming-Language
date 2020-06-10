varconst{WhatIsThis}: "Think of varconst as a constant in C!";
make [S]WhatIsThis_: "make basically says 'Hey we are making a variable.' [S] is the type, S meaning String. Then after the type, the variable name followed by : declaring the start of the value, then the value. Types include S(STRING) I(INTEGER) C(CHAR) A(ANY). Or, you can use the ~ to declare the variable can be of any type. The ~ syntax does not require you to suround it with square brackets."

print[]("print[]() is the function used to print make variables. Inside the [] you can tell the print function what type you strictly want to print. Or leave it like [] to declare type of any.")
Print("varconst declarations are printed with the Print() function");