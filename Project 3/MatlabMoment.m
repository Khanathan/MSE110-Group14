Array=csvread('datalog-420.csv');
index = Array(:, 1);
reading = Array(:, 2);
%plot(col1, col2)
ws = 10;
for i = 1:length(reading) - (ws - 1)
    OneLineDataAve(i) = sum(reading(i:i + (ws - 1))) / ws;
end
hold on
plot(OneLineDataAve)
%plot(index, 3 * round(reading / max(reading)))
for i = 1 : length(OneLineDataAve) - 1
    DataAveDiff(i) = abs(OneLineDataAve(i + 1) - OneLineDataAve(i));
end
plot(DataAveDiff);
hold on
[pks, locs] = findpeaks(DataAveDiff, 'MinPeakHeight', 1, 'MinPeakDistance', 5);
plot(locs, pks, 'or');

LOOKUPTABLE = [311113113; %A
113113113; %B
313113111; %C
111133113; %D
311133111; %E
113133111; %F
111113313; %G
311113311; %H
113113311; %I
111133311; %J
311111133; %K
113111133; %L
313111131; %M
111131133; %N
311131131; %O
113131131; %P
111111333; %Q
311111331; %R
113111331; %S
111131331; %T
331111113; %U
133111113; %V
333111111; %W
131131113; %X
331131111; %Y
133131111]; %Z

widths = (locs(2:end) - locs(1:end - 1));
widths = round(widths / min(widths));
scannedPattern = str2num(strrep(num2str(widths), ' ', ''))
charVal = find(LOOKUPTABLE == scannedPattern);
Letter = char(64 + charVal)