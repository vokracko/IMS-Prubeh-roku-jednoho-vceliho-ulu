set xlabel 'Den'
set ylabel 'Počet včel v úlu'
set title 'Vývoj možství včel v úlu'
set terminal png
set autoscale xfix
set output 'beeCount.png'
plot 'beeCount.dat' using 1:2 smooth csplines title "Počet včel celkem", 'beeCount.dat' using 1:3 smooth csplines title "Dlouhověké", 'beeCount.dat' using 1:4 smooth csplines title "Létavky"
