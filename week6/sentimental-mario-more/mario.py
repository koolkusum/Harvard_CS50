# TODO
from cs50 import get_int

# Prompt the user for a valid height value
height = 0
while height < 1 or height > 8:
    height = get_int("Height: ")

i = int(1)

while i <= height:
    print((" " * (height-i)) + ("#" * i) + "  " + ("#"*i))
    i += 1