import more_numbers as mn

number1 = 1354354534534534532453423415345645645337374534534782223453440
number2 = 3545347897233435454534564534584646345555555242782727275435435

bases = [10, 2, 16]

for base in bases:
    
    a = mn.new_number(number1, base)
    b = mn.new_number(number2, base)

    print(f"base = {base}\n")

    print(-a)
    print(abs(a))
    print(len(a))
    print(a + b)
    print(a - b)
    print(a > b)
    print(a < b)
    print(a >= b)
    print(a <= b)
    print(a == b)
    print(a != b)
    print(a == a)
    print("\n")

