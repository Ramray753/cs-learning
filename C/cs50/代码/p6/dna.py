import csv
import sys


def main():

    # TODO: Check for command-line usage

    if len(sys.argv) < 3:
        print("Usage: python dna.py data.csv sequence.txt")
        return
    data_file = sys.argv[1]
    seq_file = sys.argv[2]

    # TODO: Read database file into a variable
    data = {}
    with open(data_file, "r") as fp:
        reader = csv.reader(fp)
        STR_s = next(reader)[1:]
        for row in reader:
            data[row[0]] = [int(n) for n in row[1:]]

    # TODO: Read DNA sequence file into a variable
    with open(seq_file, "r") as fp:
        seq = fp.readline()

    # TODO: Find longest match of each STR in DNA sequence
    STR_count = []
    for STR in STR_s:
        STR_count.append(longest_match(seq, STR))

    # TODO: Check database for matching profiles
    for key in data:
        match = True
        for n1, n2 in zip(data[key], STR_count):
            if n1 != n2:
                match = False
                break
        if match:
            print(key)
            return
    print("No match")
    return


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
