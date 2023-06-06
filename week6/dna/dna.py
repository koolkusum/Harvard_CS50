import csv
import sys


def main():

    # TODO: Check for command-line usage  check for correct number inputs
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    # TODO: Read database file into a variable
    database = []
    csvFile = sys.argv[1]
    with open(csvFile, "r") as csvfile:
        reader = csv.DictReader(csvfile)
        # use this ref https://courses.cs.washington.edu/courses/cse140/13wi/csv-parsing.html
        for row in reader:
            database.append(row)

    # TODO: Read DNA sequence file into a variable   one long string
    seqFile = sys.argv[2]
    with open(seqFile, "r") as sequence:
        seq = sequence.read()

    # TODO: Find longest match of each STR in DNA sequence
    # more on keys https://docs.python.org/3/library/stdtypes.html#dict-views
    # not comparing names, str is key and stored in a dictionary

    longestMatchcounts = {}
    for key in database[0].keys():
        if key == "name":
            continue
        longestMatchcounts[key] = longest_match(seq, key)

    # TODO: Check database for matching profiles
    for person in database:
        matchingseq = True
        for key in person.keys():
            if key == "name":
                continue
            if int(person[key]) != longestMatchcounts[key]:
                matchingseq = False
                break
        if matchingseq:
            print(person["name"])
            return

    print("No Match")


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
