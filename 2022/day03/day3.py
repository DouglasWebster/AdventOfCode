import string
import time

start = time.time()

LETTERS = string.ascii_letters


def common_letter_value(groups):
    common_letter = set(LETTERS)
    for group in groups:
        common_letter = common_letter & {c for c in group}
    return LETTERS.index(list(common_letter)[0]) + 1


with open("input.txt", "r") as f:
    rucksacks = f.read().splitlines()
    rucksacks_compartmented = [
        [rucksack[: len(rucksack) // 2], rucksack[len(rucksack) // 2 :]]
        for rucksack in rucksacks
    ]
    print(
        sum([common_letter_value(group) for group in rucksacks_compartmented])
    )  # part 1
    group_of_rucksacks = [
        rucksacks[i * 3 : (i + 1) * 3] for i in range(len(rucksacks) // 3)
    ]
    print(sum([common_letter_value(group) for group in group_of_rucksacks]))  # part 2

end = time.time()

print(
    "The time of execution of above program is :",
    (end - start) * 10**6,
    "micro seconds",
)
