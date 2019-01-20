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
