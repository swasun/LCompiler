A simple compiler for the toy language L. It converts .l files to MIPS (old school project, Compilation class, 2016).

The goal of the project is the implementation of the "Language L", a fictive programming language, resembling C language, with for main functionalities the integer and array manipulations.

The lexical/syntactic/semantic analysis works correctly, but the MIPS part only works partially (only the file with the main function and using exclusively global variables).

## Usage

```
mkdir obj && mkdir bin && make
./bin/l_compiler --help

Usage: ./bin/l_compiler -f <source_file_name> | --file <source_file_name> | -d <source_dir_name> --dir <source_dir_name> [--lex | --synt | --asynt | --symb | --stack | --tests]
-f: Mandatory argument. Spécifie le fichier source .l.
--lex: Optional argument. Create a file 'source_file_name.lex' that contains the detail of the lexical analysis.
--synt: Optional argument. Create a file 'source_file_name.synt' that contains the detail of the syntactic analysis.
--asynt: Optional argument. Create a file 'source_file_name.asynt' that contains the detail of the abstract syntax.
--symb: Optional argument. Create a file 'source_file_name.symb' that contains the detail of the symbol table.
--stack: Optional argument. Create a file 'stacktrace' that contains the evantual internal errors of the compiler.
--tests: Optional argument. Create a file 'tests' that contains the detail of the executation of the compilation tests, as well as eventual errors.
```

## Features

* integer
* assignment
* function
* if
* else
* elseif
* for
* while
* array
* recursion
* stdin read
* stdout write
* arithmetics
* local vs global scope

## Error detection

* excepted 'x' before 'y'
* excepted correct statement before 'x'
* wrong assignement to integer 'x'
* wrong assignement to array 'x'
* 'x' undeclared (first use in this function)
* redefinition of 'x'
* undefined reference to 'x'
* array cannot be declared in local scope 
* implicit declaration of function 'x'
* variable 'x' already declared
* variable 'x' already declared in global scope
* too many arguments to function 'x'
* too few arguments to function 'x'

## Examples of L files

Factorial function:
```c
fact(integer $f)
{
    if $f = 1 | $f = 0 then {
        return 1;
    }
    else {
        return fact($f - 1) * $f ;
    }
}

```

Max function:
```c
max(integer $a, integer $b)
{
    if $a < $b then {
        return $b;
    }
    return $a;
}

main()
integer $v_1, integer $v_2;
{
    $v_1 = read();
    $v_2 = read();
    if max($v_1, $v_2) = $v_1 then
    {
        write($v_1);
    }
    else
    {
        write($v_2);
    }
}

```

## Authors

* Charly LAMOTHE
* Stéphane ARCELLIER
