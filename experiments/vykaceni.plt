set xlabel 'Den'
set ylabel 'Množství medu [kg]'
set title 'Množství medu v úlu'
set autoscale xfix
set terminal png
set output 'vykaceni.png'
plot 'experiments/honeyVykaceni.dat' using 1:2 smooth csplines title 'Po vykácení', 'experiments/honeyZimovani.dat' using 1:2 smooth csplines title 'Před vykácením'
