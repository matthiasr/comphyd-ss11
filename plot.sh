#!/bin/sh

gnuplot <<EOF
set term 'pdf' size 21cm,29.7cm
set output '$2'
set multiplot layout 2,1

set title "$1 - Results"
plot '$1' using 1:2 with lines title "Exact", '' using 1:3 with lines title 'Euler', '' using 1:4 with lines title "Heun", '' using 1:5 with lines title "Runge-Kutta(4)"

set title "$1 - Errors"
plot '$1' using 1:(\$3-\$2) with lines title 'Euler', '' using 1:(\$4-\$2) with lines title "Heun", '' using 1:(\$5-\$2) with lines title "Runge-Kutta(4)"

unset multiplot
unset output

EOF
