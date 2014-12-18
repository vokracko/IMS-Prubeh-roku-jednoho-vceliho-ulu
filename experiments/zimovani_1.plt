set xlabel 'Den'
set ylabel 'Množství medu [kg]'
set title 'Množství medu v úlu'
set key off
set autoscale xfix
set terminal png
set output 'zimovani.png'
plot 'experiments/honeyZimovani.dat' smooth csplines
