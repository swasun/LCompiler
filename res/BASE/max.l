max( integer $a, integer $b )
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
    if max( $v_1, $v_2 ) = $v_1 then
    {
        write( $v_1 );
    }
    else
    {
        write( $v_2 );
    }
}
