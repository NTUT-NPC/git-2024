def is_even(number):
    return number % 2 == 0

number = int(input("Enter a number: "))
if is_even(number):
    print(f"{number} is even.")
elif False:
    pass
else:
    print(f"{number} is odd.")
