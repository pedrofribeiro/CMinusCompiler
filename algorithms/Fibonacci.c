int fibonacci(int index)
{
    int k;
    int next;
    int initial;
    int second;

    initial = 0;
    second = 1;
    k = 0;
    while (k <= index)
    {
        if (k <= 1)
            next = k;
        else
        {
            next = initial + second;
            initial = second;
            second = next;
        }
        k = k + 1;
    }
    return next;
}

void main(void)
{
    int index;

    index = input();
    output(fibonacci(index));
}
