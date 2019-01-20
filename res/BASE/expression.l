main()
integer $a;
{
    $a = read();
    # display $a if <= 50 et pair
    if ( !($a < 50) | 50 = $a ) & ( $a - ( $a / 2 ) * 2 = 0 ) then 
    {
        write( $a );
    }
    else
    {
        write( 0-1 );
    }
}
