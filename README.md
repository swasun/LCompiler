A simple compiler for the toy language L. It converts .l files to MIPS (old school project, Compilation class, 2016).

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
        return fact( $f - 1 ) * $f ;
    }
}

```

Sort function:
```c
integer $tab[ 10 ];

initialize()
{
    $tab[0] = 8;    $tab[1] = 6;    $tab[2] = 9;
    $tab[3] = 9;    $tab[4] = 4;    $tab[5] = 2;
    $tab[6] = 3;    $tab[7] = 1;    $tab[8] = 4;
    $tab[9] = 5;    
}

display( integer $n )
integer $i;
{
    $i = 0;
    while $i < $n do {
        write( $tab[ $i ] );
        $i = $i + 1;
    }    
    write( 0 );
}

switch( integer $i, integer $j )
integer $temp;
{
    $temp = $tab[ $j ];
    $tab[ $j ] = $tab[ $i ];
    $tab[ $i ] = $temp;
}

sort( integer $n )
integer $swap, integer $j, integer $m;
{
    $m = $n;
    $swap = 1;
    while $swap = 1 do
    {
        $swap = 0;
        $j = 0;
        while $j < $m - 1 do 
        {
            if $tab[ $j + 1 ] < $tab[ $j ] then {
                switch( $j, $j + 1);
                $swap = 1;
            }
            $j = $j + 1;
        }
        $m = $m - 1;        
    }    
}

main()
{
    initialize();
    display( 10 );
    sort( 10 );
    display( 10 );
}
```

## Authors

* Charly LAMOTHE
* Stéphane ARCELLIER
