import more_numbers as mn

a = mn.new_number(-1, 2)

for i in range(16):
    b = mn.new_number(-i-1, 2)
    print(b)
    print(a + b)

