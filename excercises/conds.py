def less(a, b):
    smaller = a
    greater = b

    if b < a:
        smaller, greater = greater, smaller

    smaller += 1
    print()

less(10, 5)
