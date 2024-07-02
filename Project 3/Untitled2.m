Array=csvread('datalog-420.csv');
col1 = Array(:, 1);
col2 = Array(:, 2);
%plot(col1, col2)
plot(col1, 3 * round(col2 / max(col2)))