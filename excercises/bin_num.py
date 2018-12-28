first_number = '11011011'
second_number = '01110011'

# Initial state
number = first_number + second_number
print(number)

for i in range(0, 16):
    tmp = int(number, 2)

    res = tmp >> 1
    print(res)
    number = str(bin(res))

