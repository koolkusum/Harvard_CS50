# TODO
from cs50 import get_string

# converted my c code into python


def letter_count(text):
    # Calculate the length of the text
    length = len(text)

    # Count the alphabetic characters
    letters = 0

    for char in text:
        if char.isalpha():
            letters += 1

    return letters


def word_count(text):
    # Count the spaces to determine the number of words
    words = 0
    for char in text:
        if char.isspace():
            words += 1

    return words + 1


def sentence_count(text):
    # Count the sentence-ending punctuation marks
    sent = 0
    for char in text:
        if char in ['!', '.', '?']:
            sent += 1

    return sent


def main():
    # Prompt the user for input
    text = get_string("Text: ")

    # Get the counts
    letter_cnt = letter_count(text)
    word_cnt = word_count(text)
    sent_cnt = sentence_count(text)

    # Calculate average sentence length and average letters per word
    avg_s = 100.0 * (sent_cnt / word_cnt)
    avg_l = 100.0 * (letter_cnt / word_cnt)

    # Calculate the Coleman-Liau index
    index = 0.0588 * avg_l - 0.296 * avg_s - 15.8

    # Print the reading level
    if index < 1:
        print("Before Grade 1")
    elif index >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {round(index)}")


if __name__ == "__main__":
    main()
