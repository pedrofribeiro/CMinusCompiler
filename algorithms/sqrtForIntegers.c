int floorSqrt(int n)
{
    int initial;
    int finish;
    int segment;
    int result;

    if (n == 0 || n == 1)
        return n;

    initial = 1;
    finish = n / 2;
    while (initial <= finish)
    {
        segment = initial + ((finish - initial) / 2);

        if (segment * segment == n)
            return segment;

        if (segment * segment < n)
        {
            initial = segment + 1;
            result = segment;
        }
        else
            finish = segment - 1;
    }
    return result;
}

void main(void)
{
    int number;

    number = input();
    output(floorSqrt(number));
}
