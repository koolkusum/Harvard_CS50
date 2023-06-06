# TODO

# literally just copied my C code and converted to python
def get_credit():
    # Can assume the input is numeric
    number = int(input("Number: "))
    return number


def check_sum(credit_num, length):
    # Using to add every other digit the way it needs to be added
    alternate = 0
    total_sum = 0
    while credit_num > 0:
        digit = credit_num % 10
        if alternate == 0:
            total_sum += digit
            alternate = 1
        else:
            digit *= 2
            total_sum += digit % 10 + digit // 10
            alternate = 0
        credit_num //= 10
    last_digit = total_sum % 10
    return last_digit


def check_card(credit_num):
    str_num = str(credit_num)
    length = len(str_num)

    # Length check
    if length != 15 and length != 16 and length != 13:
        print("INVALID")
    else:
        # Performing the checksum
        sum_result = check_sum(credit_num, length)
        if sum_result != 0:
            print("INVALID")
        # Now checking if it's one of the following 3 cards using length
        elif length == 15:
            first_digit = credit_num // 10000000000000
            # AMEX CARD
            if first_digit == 34 or first_digit == 37:
                print("AMEX")
            else:
                print("INVALID")
        elif length == 16:
            # Mastercard OR Visa
            first_digit = credit_num // 100000000000000
            if 51 <= first_digit <= 55:
                print("MASTERCARD")
            elif first_digit // 10 == 4:
                print("VISA")
            else:
                print("INVALID")
        elif length == 13:
            # VISA
            first_digit = credit_num // 100000000000
            if first_digit // 10 == 4:
                print("VISA")
            else:
                print("INVALID")


def main():
    # American Express 15-digit: 34 / 37
    # Mastercard 16-digit: 51 52 53 54 55
    # Visa uses 13- and 16-digit: 4
    credit_num = get_credit()
    check_card(credit_num)


if __name__ == "__main__":
    main()
