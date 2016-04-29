set pagination off
set logging overwrite on
# set logging file gdb_app.log
set logging on
set $m_adr = malloc
set $f_adr = free
set $c_adr = calloc
break malloc
break free
break calloc

while 1
  echo =====================================================\n
  echo CAPTURE BEGIN\n
  cont
  where

  if $rip == $c_adr
    set $a_size = $rdi * $rsi
    finish
    printf "alloc addr %x size %d\n", $rax, $a_size
  end

  if $rip == $m_adr
    set $a_size = $rdi
    finish
    printf "alloc addr %x size %d\n", $rax, $a_size
  end

  if $rip == $f_adr
    printf "free addr %x\n", $rdi
  end

  echo CAPTURE END\n
end
