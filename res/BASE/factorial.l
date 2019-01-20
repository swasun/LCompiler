fact( integer $f ) 
{
  if $f = 1 | $f = 0 then {
    return 1;
  }
  else {
    return fact( $f - 1 ) * $f ;
  }
}

main() 
integer $a;
{
  $a = read();
  write( fact( $a ) );
}
