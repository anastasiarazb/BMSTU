program hello(output);

begin

    if (5 > 4) & (5 > 3)
    then
        writeln('5 > 4 & 5 > 3');
    if (5 < 4) | (5 > 3)
    then
        writeln('5 < 4 | 5 > 3');
    if ~ (5 < 4)
    then
        writeln('~ (5 < 4)');
    if (5 > 4) and (5 > 3)
    then
        writeln('5 > 4 and 5 > 3');
    if (5 < 4) or (5 > 3)
    then
        writeln('5 < 4 or 5 > 3');
    if not (5 < 4)
    then
        writeln('not (5 < 4)');
    if ~ (5 > 4)
    then
        writeln('~ (5 > 4)')
    else
        writeln('alternative branch of ~(5 > 4)');
end.
