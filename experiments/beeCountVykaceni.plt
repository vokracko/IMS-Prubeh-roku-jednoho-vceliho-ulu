set xlabel 'Den'
set ylabel 'Počet včel v úlu'
set title 'Vývoj možství včel v úlu'
set terminal png
set autoscale xfix
set output 'vykaceni_pocet_vcel.png'
plot 'experiments/beeCountVykaceni.dat' using 1:2 smooth csplines title "Počet včel celkem", 'experiments/beeCountZimovani.dat' using 1:3 smooth csplines title "Dlouhověké", 'experiments/beeCountZimovani.dat' using 1:4 smooth csplines title "Létavky"
